cmake_minimum_required(VERSION 3.6)
project(VParser)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-fPIC")
set(CMAKE_C_FLAGS "-fPIC")


set(SOURCE_FILES
        interface/python/PythonObjectAdapter.cpp interface/python/PythonObjectAdapter.h
        interface/python/PythonInterface.cpp interface/python/PythonInterface.h
        error/Error.cpp error/Error.h error/YAMLFormatError.h
        doc/DocElement.cpp doc/DocElement.h doc/DocScalarElement.cpp doc/DocScalarElement.h
        doc/DocObjectElement.cpp doc/DocObjectElement.h doc/DocSequenceElement.cpp
        doc/DocSequenceElement.h doc/YAMLAdapter.cpp doc/YAMLAdapter.h doc/XMLAdapter.cpp
        doc/XMLAdapter.h error/XMLFormatError.h
        error/YAMLParserInitError.h
        error/RefNotFoundError.h error/RefRecursiveError.h
        ref_expand/RefExpand.cpp ref_expand/RefExpand.h
        error/YAMLEmitterInitError.h error/YAMLEmitterError.h
        error/XMLEmitterError.h schema/BaseObject.cpp schema/BaseObject.h
        schema/Controller.cpp schema/Controller.h schema/InfoObject.cpp schema/InfoObject.h schema/Utility.cpp
        schema/Utility.h schema/SchemesObject.cpp schema/SchemesObject.h schema/EnumObject.cpp schema/EnumObject.h
        schema/APIPool.cpp schema/APIPool.h schema/DataSchemaPool.cpp schema/DataSchemaPool.h schema/ParameterPool.cpp
        schema/ParameterPool.h schema/ResponsePool.cpp schema/ResponsePool.h schema/TagsObject.cpp schema/TagsObject.h
        schema/ExternalDocObject.cpp schema/ExternalDocObject.h
        error/InvalidSwaggerSymbolError.h error/InvalidXVeeSymbolError.h
        error/RootFieldDuplicateError.h
        error/InvalidRootNodeError.h error/FieldMissError.h
        error/FieldInvalidError.h error/InvalidSchemeError.h data/BaseDataObject.cpp data/BaseDataObject.h
        schema/DataSchemaObject.cpp schema/DataSchemaObject.h data/NumberDataObject.cpp data/NumberDataObject.h
        data/IntegerDataObject.cpp data/IntegerDataObject.h data/BooleanDataObject.cpp data/BooleanDataObject.h
        data/StringDataObject.cpp data/StringDataObject.h data/FileDataObject.cpp data/FileDataObject.h
        data/ObjectDataObject.cpp data/ObjectDataObject.h data/CustomizedDataObject.cpp
        data/CustomizedDataObject.h data/SequenceDataObject.cpp data/SequenceDataObject.h
        schema/data_schema/XMLObject.cpp schema/data_schema/XMLObject.h
        data/DataObjectAdapter.cpp data/DataObjectAdapter.h error/FieldIllegalError.h
        schema/data_schema/StringSchema.cpp schema/data_schema/StringSchema.h
        schema/data_schema/NumberSchema.cpp schema/data_schema/NumberSchema.h schema/data_schema/IntegerSchema.cpp
        schema/data_schema/IntegerSchema.h schema/data_schema/BooleanSchema.cpp schema/data_schema/BooleanSchema.h
        schema/data_schema/ArraySchema.cpp schema/data_schema/ArraySchema.h schema/data_schema/FileSchema.cpp
        schema/data_schema/FileSchema.h schema/data_schema/ObjectSchema.cpp schema/data_schema/ObjectSchema.h
        schema/data_schema/CustomizedSchema.cpp schema/data_schema/CustomizedSchema.h
        error/IllegalEnumObjectError.h
        error/IllegalRegexError.h error/IllegalIntervalError.h
        schema/ParameterObject.cpp schema/ParameterObject.h schema/AbstractResponseObject.cpp schema/AbstractResponseObject.h
        schema/response/ResponseObject.cpp schema/response/ResponseObject.h
        schema/response/ResponseExtensionObject.cpp schema/response/ResponseExtensionObject.h schema/APIObject.cpp schema/APIObject.h
        schema/api/APIConstraintObject.cpp schema/api/APIConstraintObject.h error/IllegalPathParameterError.h
        error/IllegalConsumesForFormData.h error/FormDataAndBodyConflictError.h error/IllegalFileParameterError.h
        error/IllegalMIMEListError.h error/DuplicateParameterNameError.h error/TooManyBodyParamError.h interface/Interface.cpp
        interface/Interface.h error/FileNotExistError.h error/DocNodeNotExistError.h error/NotParsedError.h scenario/Scenarios.cpp
        scenario/Scenarios.h scenario/Scenario.cpp scenario/Scenario.h scenario/ScenarioInfoObject.cpp scenario/ScenarioInfoObject.h
        error/DuplicateScenarioNamesError.h error/NotScenarioParsedError.h scenario/ScenarioStateObject.cpp scenario/ScenarioStateObject.h error/DuplicateScenarioElementError.h error/ScenarioInitialStateError.h scenario/ScenarioSetObject.cpp scenario/ScenarioSetObject.h scenario/ScenarioModuleObject.cpp scenario/ScenarioModuleObject.h scenario/module/ModuleRepeatObject.cpp scenario/module/ModuleRepeatObject.h scenario/module/ModuleInputConstraintObject.cpp scenario/module/ModuleInputConstraintObject.h scenario/module/ModuleCheckpointObject.cpp scenario/module/ModuleCheckpointObject.h scenario/module/ModuleSetEffectObject.cpp scenario/module/ModuleSetEffectObject.h scenario/module/ModuleTransStateObject.cpp scenario/module/ModuleTransStateObject.h error/ScenarioStartModuleError.h)

add_executable(VParser ${SOURCE_FILES} interface/CXX/demo.cpp)

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


