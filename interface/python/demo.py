"""
    A tiny script to test the LapisParser Python interface
    llylly
"""
import os
import shutil


def init_module(dll_path='../../cmake-build-debug/libVParserPy.dylib'):
    """
    Copy compiled dylib to this directory and rename, to prepare for use
    :return: None
    """
    absPath = os.path.abspath('.')
    dlibPath = os.path.join(absPath, dll_path)
    shutil.copy(dlibPath, os.path.join(absPath, 'LapisParser.so'))

if __name__ == '__main__':
    searchPaths = ['../../cmake-build-debug/libVParserPy.dylib', '../../libVParserPy.dylib']
    for path in searchPaths:
        if os.path.exists(path):
            init_module(path)


    from LapisParser import *

    absPath = os.path.abspath('.')

    addDocFromFile('../../official_demo.yaml', 'default')
    print(getDocList())
    print(parseAPI())
    print(getInfo())
    print(getHost())
    print(getBasePath())
    print(getSchemes())
    print(getConsumes())
    print(getProduces())
    print(getAPINames())
    print(getDataSchemaNames())
    print(getParameterNames())
    print(getResponseNames())
    print(getTags())
    print(getExternalDocs())

    api_name = getAPINames()[3]
    print(getAPI(api_name['name'], api_name['method']))

    print(generateRandomDataFromDataSchema('Error'))

    print(getInfo())
    info = getInfo()
    info['description'] = 'I modified it!'
    print(setInfo(info))
    print(parseAPI())
    print(getInfo())

    print(getErrors())


