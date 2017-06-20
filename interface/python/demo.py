"""
    A tiny script to test the LapisParser Python interface
    llylly
"""
import os
import shutil


def init_module():
    """
    Copy compiled dylib to this directory and rename, to prepare for use
    :return: None
    """
    absPath = os.path.abspath('.')
    dlibPath = os.path.join(absPath, '../../cmake-build-debug/libVParserPy.dylib')
    shutil.copy(dlibPath, os.path.join(absPath, 'LapisParser.so'))

if __name__ == '__main__':
    init_module()

    import LapisParser

    absPath = os.path.abspath('.')

    res = LapisParser.readYAML([os.path.join(absPath, '../../test.yaml')])
    print LapisParser.getErrors()
    res = LapisParser.translate2XML('transTest1.xml')

    res = LapisParser.readXML([os.path.join(absPath, '../../example-ossgroup.xml')])
    print LapisParser.getErrors()
    res = LapisParser.translate2XML('transTest2.yaml')