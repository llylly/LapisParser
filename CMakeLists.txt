cmake_minimum_required(VERSION 3.6)
project(VParser)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp error/Error.cpp error/Error.h error/YAMLFormatError.cpp error/YAMLFormatError.h doc/DocElement.cpp doc/DocElement.h doc/DocScalarElement.cpp doc/DocScalarElement.h doc/DocObjectElement.cpp doc/DocObjectElement.h doc/DocSequenceElement.cpp doc/DocSequenceElement.h doc/YAMLAdapter.cpp doc/YAMLAdapter.h doc/XMLAdapter.cpp doc/XMLAdapter.h error/XMLFormatError.cpp error/XMLFormatError.h error/YAMLParserInitError.cpp error/YAMLParserInitError.h error/RefNotFoundError.cpp error/RefNotFoundError.h error/RefRecursiveError.cpp error/RefRecursiveError.h ref_expand/RefExpand.cpp ref_expand/RefExpand.h error/YAMLEmitterInitError.cpp error/YAMLEmitterInitError.h error/YAMLEmitterError.cpp error/YAMLEmitterError.h error/XMLEmitterError.cpp error/XMLEmitterError.h schema/BaseObject.cpp schema/BaseObject.h schema/Controller.cpp schema/Controller.h schema/InfoObject.cpp schema/InfoObject.h schema/Utility.cpp schema/Utility.h schema/SchemesObject.cpp schema/SchemesObject.h schema/EnumObject.cpp schema/EnumObject.h schema/APIPool.cpp schema/APIPool.h schema/DataSchemaPool.cpp schema/DataSchemaPool.h schema/ParameterPool.cpp schema/ParameterPool.h schema/ResponsePool.cpp schema/ResponsePool.h schema/TagsObject.cpp schema/TagsObject.h schema/ExternalDocObject.cpp schema/ExternalDocObject.h error/InvalidSwaggerSymbolError.cpp error/InvalidSwaggerSymbolError.h error/InvalidXVeeSymbolError.cpp error/InvalidXVeeSymbolError.h error/RootFieldDuplicateError.cpp error/RootFieldDuplicateError.h error/InvalidRootNodeError.cpp error/InvalidRootNodeError.h error/FieldMissError.cpp error/FieldMissError.h error/FieldInvalidError.cpp error/FieldInvalidError.h error/InvalidSchemeError.cpp error/InvalidSchemeError.h schema/ScenarioPool.cpp schema/ScenarioPool.h)

add_executable(VParser ${SOURCE_FILES})

add_library(VParserPy ${SOURCE_FILES})

link_libraries(parser/libyaml/)
target_link_libraries(VParser yaml)
target_link_libraries(VParserPy yaml)

link_libraries(parser/libxml/)
target_link_libraries(VParser xml2)
target_link_libraries(VParserPy xml2)