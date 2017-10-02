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
    print('DocList: {}'.format(getDocList()))
    print('Doc: {}'.format(getDoc()))
    print('-------')
    print('Parse API status: {}'.format(parseAPI()))
    print('-------')
    print('info: {}'.format(getInfo()))
    print('host: {}'.format(getHost()))
    print('basePath: {}'.format(getBasePath()))
    print('-------')
    print('errors: {}'.format(getErrors()))
    print('-------')
    print('consumes: {}'.format(getConsumes()))
    print('produces: {}'.format(getProduces()))
    print('API names: {}'.format(getAPINames()))
    print(getAPI('estimates/price', 'get')['parameters'])
    # parseAPI()
    #
    # print(getErrors())
    #
    # print(getBasePath())
    # print(getSchemes())
    # print(getConsumes())
    # print(getProduces())
    # print(getAPINames())
    # print(getDataSchemaNames())
    # print(getParameterNames())
    # print(getResponseNames())
    # print(getTags())
    # print(getExternalDocs())
    #
    # print(getAPINames())
    # print(getErrors())


