cmake_minimum_required(VERSION 3.6)
project(VParser)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_C_FLAGS "-fPIC")


set(SOURCE_FILES
        main.cpp
        interface/python/PythonObjectAdapter.cpp interface/python/PythonObjectAdapter.h
        interface/python/PythonInterface.cpp interface/python/PythonInterface.h
        error/Error.cpp error/Error.h error/YAMLFormatError.cpp error/YAMLFormatError.h
        doc/DocElement.cpp doc/DocElement.h doc/DocScalarElement.cpp doc/DocScalarElement.h
        doc/DocObjectElement.cpp doc/DocObjectElement.h doc/DocSequenceElement.cpp
        doc/DocSequenceElement.h doc/YAMLAdapter.cpp doc/YAMLAdapter.h doc/XMLAdapter.cpp
        doc/XMLAdapter.h error/XMLFormatError.cpp error/XMLFormatError.h
        error/YAMLParserInitError.cpp error/YAMLParserInitError.h error/RefNotFoundError.cpp
        error/RefNotFoundError.h error/RefRecursiveError.cpp error/RefRecursiveError.h
        ref_expand/RefExpand.cpp ref_expand/RefExpand.h error/YAMLEmitterInitError.cpp
        error/YAMLEmitterInitError.h error/YAMLEmitterError.cpp error/YAMLEmitterError.h
        error/XMLEmitterError.cpp error/XMLEmitterError.h schema/BaseObject.cpp schema/BaseObject.h
        schema/Controller.cpp schema/Controller.h schema/InfoObject.cpp schema/InfoObject.h schema/Utility.cpp
        schema/Utility.h schema/SchemesObject.cpp schema/SchemesObject.h schema/EnumObject.cpp schema/EnumObject.h
        schema/APIPool.cpp schema/APIPool.h schema/DataSchemaPool.cpp schema/DataSchemaPool.h schema/ParameterPool.cpp
        schema/ParameterPool.h schema/ResponsePool.cpp schema/ResponsePool.h schema/TagsObject.cpp schema/TagsObject.h
        schema/ExternalDocObject.cpp schema/ExternalDocObject.h error/InvalidSwaggerSymbolError.cpp
        error/InvalidSwaggerSymbolError.h error/InvalidXVeeSymbolError.cpp error/InvalidXVeeSymbolError.h
        error/RootFieldDuplicateError.cpp error/RootFieldDuplicateError.h error/InvalidRootNodeError.cpp
        error/InvalidRootNodeError.h error/FieldMissError.cpp error/FieldMissError.h error/FieldInvalidError.cpp
        error/FieldInvalidError.h error/InvalidSchemeError.cpp error/InvalidSchemeError.h schema/ScenarioPool.cpp
        schema/ScenarioPool.h data/BaseDataObject.cpp data/BaseDataObject.h
        schema/DataSchemaObject.cpp schema/DataSchemaObject.h data/NumberDataObject.cpp data/NumberDataObject.h
        data/IntegerDataObject.cpp data/IntegerDataObject.h data/BooleanDataObject.cpp data/BooleanDataObject.h
        data/StringDataObject.cpp data/StringDataObject.h data/FileDataObject.cpp data/FileDataObject.h
        data/ObjectDataObject.cpp data/ObjectDataObject.h data/CustomizedDataObject.cpp
        data/CustomizedDataObject.h data/SequenceDataObject.cpp data/SequenceDataObject.h
        schema/data_schema/XMLObject.cpp schema/data_schema/XMLObject.h
        data/DataObjectAdapter.cpp data/DataObjectAdapter.h error/FieldIllegalError.cpp error/FieldIllegalError.h
        schema/data_schema/StringSchema.cpp schema/data_schema/StringSchema.h
        schema/data_schema/NumberSchema.cpp schema/data_schema/NumberSchema.h schema/data_schema/IntegerSchema.cpp
        schema/data_schema/IntegerSchema.h schema/data_schema/BooleanSchema.cpp schema/data_schema/BooleanSchema.h
        schema/data_schema/ArraySchema.cpp schema/data_schema/ArraySchema.h schema/data_schema/FileSchema.cpp
        schema/data_schema/FileSchema.h schema/data_schema/ObjectSchema.cpp schema/data_schema/ObjectSchema.h
        schema/data_schema/CustomizedSchema.cpp schema/data_schema/CustomizedSchema.h
        error/IllegalEnumObjectError.cpp error/IllegalEnumObjectError.h
        error/IllegalRegexError.cpp error/IllegalRegexError.h error/IllegalIntervalError.cpp error/IllegalIntervalError.h)

add_executable(VParser ${SOURCE_FILES})

add_library(VParserPy SHARED ${SOURCE_FILES})

link_libraries(parser/libyaml/)
target_link_libraries(VParser yaml)
target_link_libraries(VParserPy yaml)

link_libraries(parser/libxml/)
target_link_libraries(VParser xml2)
target_link_libraries(VParserPy xml2)

link_libraries(usr/include/python2.7)
target_link_libraries(VParser python)
target_link_libraries(VParserPy python)


