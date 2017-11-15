"""
    A tiny script for copying the library to current dir
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
    if os.path.exists(dlibPath):
        shutil.copy(dlibPath, os.path.join(absPath, 'LapisParser.so'))

if __name__ == '__main__':
    init_module()
    init_module('../../libVParserPy.so')
