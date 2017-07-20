"""
    A tiny script to test the LapisParser Python interface
    llylly
"""
import os
import shutil


def init_module(dll_path):
    """
    Copy compiled dylib to this directory and rename, to prepare for use
    :return: None
    """
    absPath = os.path.abspath('.')
    dlibPath = os.path.join(absPath, dll_path)
    shutil.copy(dlibPath, os.path.join(absPath, 'LapisParser.so'))

if __name__ == '__main__':
    init_module('../../cmake-build-debug/libVParserPy.dylib')

    import LapisParser

    absPath = os.path.abspath('.')

    res = LapisParser.readYAML([os.path.join(absPath, '../../example-ossgroup.yaml')])
    if res == 1:
        print('Read failed.')
        print('Errors:')
        print(LapisParser.getErrors())
    else:
        print('Read succeed.')

        res = LapisParser.translate2XML('../../output.xml')
        if res == 1:
            print('Translate failed.')
        else:
            print('Translate succeed.')

        print('------')

        print("Info Object:")
        print(LapisParser.getInfo())
        print("Host:")
        print(LapisParser.getHost())
        print("BasePath:")
        print(LapisParser.getBasePath())
        print("Schemes:")
        print(LapisParser.getSchemes())
        print("Consumes:")
        print(LapisParser.getConsumes())
        print("Produces:")
        print(LapisParser.getProduces())
        print("Tags:")
        print(LapisParser.getTags())
        print("ExternalDocs:")
        print(LapisParser.getExternalDocs())


