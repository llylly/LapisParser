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
        error/DuplicateScenarioNamesError.h error/NotScenarioParsedError.h scenario/ScenarioStateObject.cpp
        scenario/ScenarioStateObject.h error/DuplicateScenarioElementError.h error/ScenarioInitialStateError.h
        scenario/ScenarioSetObject.cpp scenario/ScenarioSetObject.h scenario/ScenarioModuleObject.cpp
        scenario/ScenarioModuleObject.h scenario/module/ModuleRepeatObject.cpp scenario/module/ModuleRepeatObject.h
        scenario/module/ModuleInputConstraintObject.cpp scenario/module/ModuleInputConstraintObject.h
        scenario/module/ModuleCheckpointObject.cpp scenario/module/ModuleCheckpointObject.h scenario/module/ModuleSetEffectObject.cpp
        scenario/module/ModuleSetEffectObject.h scenario/module/ModuleTransStateObject.cpp scenario/module/ModuleTransStateObject.h
        error/ScenarioStartModuleError.h error/InvalidAPIReferenceError.h error/InvalidResponseTypeError.h error/InvalidExpressionError.h
        error/InvalidSetError.h error/InvalidStateError.h scenario/ScenarioConnectionObject.cpp scenario/ScenarioConnectionObject.h
        error/InvalidModuleError.h config/ConfigObject.cpp config/ConfigObject.h error/DuplicateTestConfigError.h
        error/NotConfigParsedError.h error/InvalidScenarioError.h exec/request/BaseRequester.cpp
        exec/request/BaseRequester.h exec/err/RuntimeError.h exec/err/RuntimeError.cpp exec/err/InvalidURLPatternError.h
        exec/err/IllegalResponseError.h exec/log/Logger.cpp exec/log/Logger.h exec/log/StrLog.h exec/log/DataLog.h
        exec/err/RequestTimeoutError.h exec/request/SingleRequester.cpp exec/request/SingleRequester.h
        exec/request/ModuleRequester.cpp exec/request/ModuleRequester.h exec/err/IllegalInputConstraintError.h
        exec/err/SetEmptyTerminate.h exec/transformer/ExecTransformer.cpp exec/transformer/ExecTransformer.h
        exec/err/UnknownRequestError.h exec/err/IllegalResponseCodeError.h exec/err/IllegalResponseFormatError.h
        exec/err/APINotParsedError.h exec/err/APINotFoundError.h exec/err/RequesterInitError.h exec/report/BaseReport.h
        exec/report/RequesterReport.cpp exec/report/RequesterReport.h exec/report/SingleAPIReport.cpp
        exec/report/SingleAPIReport.h exec/scenario/ScenarioController.cpp exec/scenario/ScenarioController.h
        exec/report/TestCaseReport.cpp exec/report/TestCaseReport.h exec/report/ScenarioReport.cpp
        exec/report/ScenarioReport.h exec/scenario/TestCaseController.cpp exec/scenario/TestCaseController.h
        exec/err/ScenarioConfigNotParsedError.h exec/err/CheckpointNotPassTerminate.h exec/err/EndModuleTerminate.h
        exec/err/NowayTerminate.h exec/err/LengthExceedTerminate.h exec/err/IllegalSetEffectError.h
        exec/middleware/AliMiddleware.cpp exec/middleware/AliMiddleware.cpp exec/middleware/AliMiddleware.h
        exec/middleware/HMAC_SHA1/HMAC_SHA1.cpp exec/middleware/HMAC_SHA1/base64.cpp exec/middleware/HMAC_SHA1/SHA1.cpp)

add_executable(VParser ${SOURCE_FILES} interface/CXX/demo.cpp)

add_library(VParserPy SHARED ${SOURCE_FILES})

link_libraries(lib/libyaml/)
target_link_libraries(VParser yaml)
target_link_libraries(VParserPy yaml)

link_libraries(lib/libxml/)
target_link_libraries(VParser xml2)
target_link_libraries(VParserPy xml2)

link_libraries(lib/libcurl/)
target_link_libraries(VParser curl)
target_link_libraries(VParserPy curl)
link_directories(lib/libcurl/curl)

target_link_libraries(VParser crypto)
target_link_libraries(VParserPy crypto)

include_directories(lib/libxml/)

link_libraries(usr/include/python2.7)
target_link_libraries(VParser python)
target_link_libraries(VParserPy python)


