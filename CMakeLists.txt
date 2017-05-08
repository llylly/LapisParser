cmake_minimum_required(VERSION 3.6)
project(VParser)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp error/Error.cpp error/Error.h error/YAMLFormatError.cpp error/YAMLFormatError.h doc/DocElement.cpp doc/DocElement.h doc/DocScalarElement.cpp doc/DocScalarElement.h doc/DocObjectElement.cpp doc/DocObjectElement.h doc/DocSequenceElement.cpp doc/DocSequenceElement.h doc/YAMLAdapter.cpp doc/YAMLAdapter.h doc/XMLAdapter.cpp doc/XMLAdapter.h error/XMLFormatError.cpp error/XMLFormatError.h error/YAMLParserInitError.cpp error/YAMLParserInitError.h error/RefNotFoundError.cpp error/RefNotFoundError.h error/RefRecursiveError.cpp error/RefRecursiveError.h RefExpand/RefExpand.cpp RefExpand/RefExpand.h)

add_executable(VParser ${SOURCE_FILES})

link_libraries(parser/libyaml/)
target_link_libraries(VParser yaml)