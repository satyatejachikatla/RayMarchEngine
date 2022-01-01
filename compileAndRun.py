#!./venv/bin/python3
from shaderGeneration.gen import RayMarchGLSLGenerator

import argparse
import os 

COMPILE_FILE_STAGE_LOCATION = './openGLRenderingEngine/shader/staged.shader'

def compileWindowCode(force=False):
    if force:
        os.system('make -C ./openGLRenderingEngine clean')
    os.system('make -C ./openGLRenderingEngine')

def getArgs():
    parser = argparse.ArgumentParser(description='Compiles and runs shader code')
    parser.add_argument('-m','--mainFile', type=str, nargs=1,
                        help='File containing the main function of the fragment shader')
    parser.add_argument('-r','--recompileWindow', action='store_true',
                        help='Forces re compile window code ')

    args = parser.parse_args()
    return args

def main():
    args = getArgs()

    # Compile window code
    compileWindowCode(args.recompileWindow)

    # Generate shader code
    genObj = RayMarchGLSLGenerator(args.mainFile[0])

    # Save shader code
    open(COMPILE_FILE_STAGE_LOCATION,'w').write(genObj.getFinalProcessedCode())

    # Run code
    os.system('cd ./openGLRenderingEngine ; ./window ; cd -')


if __name__ == '__main__':
    main()