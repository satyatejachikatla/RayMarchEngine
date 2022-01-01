from shaderGeneration.gen import RayMarchGLSLGenerator

def main():
    genObj = RayMarchGLSLGenerator('./rayMarchUtils/test1.glsl')
    open('./foo.glsl','w').write(genObj.getFinalProcessedCode())

if __name__ == '__main__':
    main()