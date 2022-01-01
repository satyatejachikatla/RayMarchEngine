import os
from posixpath import join
import re

class RayMarchGLSLGenerator(object):
    '''
    Does following
    1. Append core.glsl code to all the files
    2. Preprocess all the #include code
    3. Consider all the #pragma once code
    '''
    VALID_PATH_PATTERN = '\S+'
    INCLUDE_PATTERN = '#include\s*(?:"{VALID_PATH_PATTERN}"|<{VALID_PATH_PATTERN}>)\s*'.format(VALID_PATH_PATTERN=VALID_PATH_PATTERN)
    INCLUDE_PATTERN_QUOTES = '#include\s*"{VALID_PATH_PATTERN}"\s*'.format(VALID_PATH_PATTERN=VALID_PATH_PATTERN)
    INCLUDE_PATTERN_ANGLE_BRACKETS = '#include\s*<{VALID_PATH_PATTERN}>\s*'.format(VALID_PATH_PATTERN=VALID_PATH_PATTERN)

    PRAGMA_ONCE_PATTERN = '#pragma\s+once\s*'

    INCLUDE_TYPE_QUOTES = 0
    INCLUDE_TYPE_ANGLE_BRACKETS = 1

    CORE_GLSL_CODE_STRING = open(os.path.dirname(__file__)+'/core.glsl').read()

    FILES_PROCESSED_CACHE = {

    }

    @classmethod
    def __getCache(cls,filepath):
        if filepath in cls.FILES_PROCESSED_CACHE:
            return cls.FILES_PROCESSED_CACHE[filepath]
        return None

    def __new__(cls,filepath,*args,**kwargs):
        existing = cls.__getCache(filepath)
        if existing:
            if existing.containsPragmaOnce:
                return None
            else:
                return existing
        obj = super(RayMarchGLSLGenerator, cls).__new__(cls)
        return obj

    def __init__(self,filepath,includeSearchPath=[]):
        cls = __class__
        filepath = os.path.abspath(filepath)
        if filepath in cls.FILES_PROCESSED_CACHE:
            return

        cls.FILES_PROCESSED_CACHE[filepath] = self

        self.filepath = filepath
        self.processedCode = None

        self.includeSearchPath = ['.']
        self.includeSearchPath.extend(includeSearchPath)

        self.containsPragmaOnce = False
    
    def identifyIncludeType(self,includeString):
        cls = __class__
        if re.findall(cls.INCLUDE_PATTERN_QUOTES,includeString):
            return cls.INCLUDE_TYPE_QUOTES
        if re.findall(cls.INCLUDE_PATTERN_ANGLE_BRACKETS,includeString):
            return cls.INCLUDE_TYPE_ANGLE_BRACKETS

        raise Exception("Failed to identify Include type for include string :",includeString)

    def isIncludeTypeQuotes(self,includeString):
        cls = __class__
        return self.identifyIncludeType(includeString) == cls.INCLUDE_TYPE_QUOTES
    
    def isIncludeTypeAngleBrackets(self,includeString):
        cls = __class__
        return self.identifyIncludeType(includeString) == cls.INCLUDE_TYPE_ANGLE_BRACKETS

    def getFilePathInIncludeString(self,includeString):
        cls = __class__
        filepaths = re.findall('("{VALID_PATH_PATTERN}"|<{VALID_PATH_PATTERN}>)'.format(VALID_PATH_PATTERN=cls.VALID_PATH_PATTERN),includeString)
        if len(filepaths) != 1:
            raise Exception("Failed to parse includeString :",includeString)
        filepath = filepaths[0][1:-1]
        return filepath

    def searchFileInIncludeSearchPath(self,includeFilePath):
        for path in self.includeSearchPath:
            searchPath = path + '/' + includeFilePath
            if os.path.isfile(searchPath):
                return searchPath
        raise Exception("Failed to find includeFilePath:", includeFilePath, "in includeSearchPath list:",self.includeSearchPath)

    def getIncludeFilePath(self,includeString):

        folderForfilepath = os.path.dirname(self.filepath)
        includeFilePath = self.getFilePathInIncludeString(includeString)

        if self.isIncludeTypeQuotes(includeString):
            path = folderForfilepath + '/' + includeFilePath
        elif self.isIncludeTypeAngleBrackets(includeString):
            path = self.searchFileInIncludeSearchPath(includeFilePath)

        if not os.path.isfile(path):
            raise Exception("Failed to search for path :", path,"in file:",self.filepath)

        return os.path.abspath(path)

    def process(self,codeString):
        cls = __class__
        if re.findall(cls.PRAGMA_ONCE_PATTERN,codeString):
            self.containsPragmaOnce = True
            codeString = '\n'.join(re.split(cls.PRAGMA_ONCE_PATTERN,codeString))

        includeSplitPoints = re.findall(cls.INCLUDE_PATTERN,codeString)

        splitCode = re.split(cls.INCLUDE_PATTERN,codeString)


        joinSplitPoints = []

        for includeString in includeSplitPoints:
            includeFilePath = self.getIncludeFilePath(includeString)
            includeFileGenerator = RayMarchGLSLGenerator(includeFilePath,self.includeSearchPath)
            if includeFileGenerator == None:
                # Hit pragma once file again
                codeSegment = ''
            else:
                if self is includeFileGenerator:
                    raise Exception('Hitting recurssion issue in the same file : ',self.filepath)
                codeSegment = includeFileGenerator.getProcessedCode()
            joinSplitPoints.append(codeSegment)

        processedString = splitCode[0]
        for i in range(len(joinSplitPoints)):
            processedString += ('\n' + joinSplitPoints[i] + '\n' + splitCode[i+1])
        
        return processedString

    def setProcessedCode(self):
        self.processedCode = self.process(open(self.filepath).read())

    def getProcessedCode(self):
        if self.processedCode == None :
            self.setProcessedCode()
        return self.processedCode
    
    def attachCoreGLSL(self):
        cls = __class__
        self.processedCode = cls.CORE_GLSL_CODE_STRING + '\n' + self.processedCode

    def getFinalProcessedCode(self):
        self.setProcessedCode()
        self.attachCoreGLSL()

        return self.processedCode