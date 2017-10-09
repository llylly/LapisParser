//
// Created by lly on 23/04/2017.
//

#include "YAMLAdapter.h"
#include "../error/YAMLParserInitError.h"
#include "../error/YAMLFormatError.h"
#include "../error/YAMLEmitterInitError.h"
#include "../error/YAMLEmitterError.h"

DocElement *YAMLAdapter::parseStr(const unsigned char *string, int length) {
    yaml_parser_t parser;
    yaml_event_t event;

    DocElement *doc = NULL;
    YAMLFormatError *err = NULL;

    bool done = false;

    if (!yaml_parser_initialize(&parser)) {
        Error::addError(new YAMLParserInitError(1, 1));
        goto error;
        return NULL;
    }
    yaml_parser_set_input_string(&parser, string, (size_t)length);

    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            addFormatErrorFromParser(parser);
            goto error;
        }
        switch (event.type) {
            case YAML_STREAM_START_EVENT:
                break;

            case YAML_STREAM_END_EVENT:
                break;

            case YAML_DOCUMENT_START_EVENT:
                doc = parseDocument(parser, 0);
                if (doc == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                break;

                // Illegal States in this context
            case YAML_DOCUMENT_END_EVENT:
            case YAML_SCALAR_EVENT:
            case YAML_SEQUENCE_START_EVENT:
            case YAML_SEQUENCE_END_EVENT:
            case YAML_MAPPING_START_EVENT:
            case YAML_MAPPING_END_EVENT:
                err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                err->msg += "\n Root node parse error.";
                Error::addError(err);
                yaml_event_delete(&event);
                goto error;

                // Ignored Events
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            default:
                break;
        }
        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);

    return doc;

    error:
    if (doc != NULL) delete doc;
    yaml_parser_delete(&parser);
    /** Important: this function is used for parsing API response.
     * Therefore we should insure no errors added to the error list.
     */
    Error::getErrors()->clear();
    return NULL;
}

DocElement *YAMLAdapter::parseDoc(const char *fileName) {

    yaml_parser_t parser;
    yaml_event_t event;

    DocElement *doc = NULL;
    YAMLFormatError *err = NULL;

    bool done = false;

    FILE *fin = NULL;

    if (!yaml_parser_initialize(&parser)) {
        Error::addError(new YAMLParserInitError(1, 1));
        goto error;
        return NULL;
    }
    fin = fopen(fileName, "r");
    yaml_parser_set_input_file(&parser, fin);

    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            addFormatErrorFromParser(parser);
            goto error;
        }
        switch (event.type) {
            case YAML_STREAM_START_EVENT:
                break;

            case YAML_STREAM_END_EVENT:
                break;

            case YAML_DOCUMENT_START_EVENT:
                doc = parseDocument(parser, 0);
                if (doc == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                break;

            // Illegal States in this context
            case YAML_DOCUMENT_END_EVENT:
            case YAML_SCALAR_EVENT:
            case YAML_SEQUENCE_START_EVENT:
            case YAML_SEQUENCE_END_EVENT:
            case YAML_MAPPING_START_EVENT:
            case YAML_MAPPING_END_EVENT:
                err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                err->msg += "\n Root node parse error.";
                Error::addError(err);
                yaml_event_delete(&event);
                goto error;

            // Ignored Events
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            default:
                break;
        }
        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }

    yaml_parser_delete(&parser);
    fclose(fin);

    return doc;

error:
    if (doc != NULL) delete doc;
    yaml_parser_delete(&parser);
    fclose(fin);
    return NULL;
    
}

DocScalarElement* YAMLAdapter::parseScalar(yaml_parser_t &parser, yaml_event_t& event, int level) {
    if (event.type == YAML_SCALAR_EVENT) {
        return new DocScalarElement((int) parser.mark.line, (int) parser.mark.column, string((char*) event.data.scalar.value), level);
    } else {
        return NULL;
    }
}

DocObjectElement* YAMLAdapter::parseObject(yaml_parser_t& parser, int level) {
    yaml_event_t event;
    DocObjectElement *obj = new DocObjectElement((int) parser.mark.line, (int) parser.mark.column, level);
    bool outDone = false, done = false;
    unsigned char stat = 0;
    string tmpKey;

    while (!outDone) {
        if (stat == 0) {
            YAMLFormatError *err = NULL;
            DocScalarElement *ele = NULL;

            done = false;
            while (!done) {
                if (!yaml_parser_parse(&parser, &event)) {
                    addFormatErrorFromParser(parser);
                    goto error;
                }
                switch (event.type) {
                    // Illegal States in this context
                    // Key of type other than string is not allowed
                    case YAML_STREAM_START_EVENT:
                    case YAML_STREAM_END_EVENT:
                    case YAML_DOCUMENT_START_EVENT:
                    case YAML_DOCUMENT_END_EVENT:
                    case YAML_MAPPING_START_EVENT:
                    case YAML_SEQUENCE_START_EVENT:
                    case YAML_SEQUENCE_END_EVENT:
                        err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                        err->msg += "\n Key parse error.";
                        Error::addError(err);
                        yaml_event_delete(&event);
                        goto error;

                    // Only Scalar Key is permitted
                    case YAML_SCALAR_EVENT:
                        ele = parseScalar(parser, event, level + 1);
                        if (ele == NULL) {
                            yaml_event_delete(&event);
                            goto error;
                        }
                        tmpKey = ele->getValue();
                        delete ele;
                        break;

                    // end of mapping
                    case YAML_MAPPING_END_EVENT:
                        outDone = true;
                        break;

                    case YAML_NO_EVENT:
                    case YAML_ALIAS_EVENT:
                    default:
                        break;
                }
                if (outDone) break;
                done = (event.type == YAML_SCALAR_EVENT);
                yaml_event_delete(&event);
            }
            stat = 1;
        } else {
            DocScalarElement *ele = NULL;
            DocSequenceElement *elee = NULL;
            DocObjectElement *eleee = NULL;

            YAMLFormatError *err = NULL;

            done = false;
            while (!done) {
                if (!yaml_parser_parse(&parser, &event)) {
                    addFormatErrorFromParser(parser);
                    goto error;
                }
                switch (event.type) {
                    // Illegal States in this context
                    case YAML_STREAM_START_EVENT:
                    case YAML_STREAM_END_EVENT:
                    case YAML_DOCUMENT_START_EVENT:
                    case YAML_DOCUMENT_END_EVENT:
                    case YAML_MAPPING_END_EVENT:
                    case YAML_SEQUENCE_END_EVENT:
                        err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                        err->msg += "\n Value Parse Error.";
                        Error::addError(err);
                        yaml_event_delete(&event);
                        goto error;

                    case YAML_SCALAR_EVENT:
                        ele = parseScalar(parser, event, level + 1);
                        if (ele == NULL) {
                            yaml_event_delete(&event);
                            goto error;
                        }
                        obj->add(tmpKey, ele);
                        done = true;
                        break;

                    case YAML_SEQUENCE_START_EVENT:
                        elee = parseSequence(parser, level + 1);
                        if (elee == NULL) {
                            yaml_event_delete(&event);
                            goto error;
                        }
                        obj->add(tmpKey, elee);
                        done = true;
                        break;

                    case YAML_MAPPING_START_EVENT:
                        eleee = parseObject(parser, level + 1);
                        if (eleee == NULL) {
                            yaml_event_delete(&event);
                            goto error;
                        }
                        obj->add(tmpKey, eleee);
                        done = true;
                        break;

                    case YAML_NO_EVENT:
                    case YAML_ALIAS_EVENT:
                    default:
                        break;
                }
                yaml_event_delete(&event);
            }
            stat = 0;
        }
        yaml_event_delete(&event);
    }

    return obj;

error:
    delete obj;
    return NULL;
}

DocSequenceElement* YAMLAdapter::parseSequence(yaml_parser_t& parser, int level) {
    yaml_event_t event;
    DocSequenceElement *seq = new DocSequenceElement((int) parser.mark.line, (int) parser.mark.column, level);
    bool done = false;

    YAMLFormatError *err = NULL;
    DocScalarElement *ele = NULL;
    DocSequenceElement *elee = NULL;
    DocObjectElement *eleee = NULL;

    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            addFormatErrorFromParser(parser);
            goto error;
        }
        switch (event.type) {
            case YAML_STREAM_START_EVENT:
            case YAML_STREAM_END_EVENT:
            case YAML_DOCUMENT_START_EVENT:
            case YAML_DOCUMENT_END_EVENT:
            case YAML_MAPPING_END_EVENT:
                // Illegal States in this context
                err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                err->msg += "\n Sequence parse error.";
                Error::addError(err);
                yaml_event_delete(&event);
                goto error;

            case YAML_SCALAR_EVENT:
                ele = parseScalar(parser, event, level + 1);
                if (ele == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                seq->add(ele);
                break;

            case YAML_SEQUENCE_START_EVENT:
                elee = parseSequence(parser, level + 1);
                if (elee == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                seq->add(elee);
                break;

            case YAML_MAPPING_START_EVENT:
                eleee = parseObject(parser, level + 1);
                if (eleee == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                seq->add(eleee);
                break;

            case YAML_SEQUENCE_END_EVENT:
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            default:
                break;
        }
        done = (event.type == YAML_SEQUENCE_END_EVENT);
        yaml_event_delete(&event);
    }

    return seq;

error:
    delete seq;
    return NULL;
}

DocElement* YAMLAdapter::parseDocument(yaml_parser_t& parser, int level) {
    yaml_event_t event;
    DocElement *doc = NULL;
    YAMLFormatError *err = NULL;

    bool done = false;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            addFormatErrorFromParser(parser);
            goto error;
        }
        switch (event.type) {
            case YAML_STREAM_START_EVENT:
            case YAML_STREAM_END_EVENT:
            case YAML_DOCUMENT_START_EVENT:
            case YAML_MAPPING_END_EVENT:
            case YAML_SEQUENCE_END_EVENT:
                // Illegal States in this context
                err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
                err->msg += "\n Document parse error.";
                Error::addError(err);
                yaml_event_delete(&event);
                goto error;

            case YAML_SCALAR_EVENT:
                doc = parseScalar(parser, event, level);
                if (doc == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                break;

            case YAML_MAPPING_START_EVENT:
                doc = parseObject(parser, level);
                if (doc == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                break;

            case YAML_SEQUENCE_START_EVENT:
                doc = parseSequence(parser, level);
                if (doc == NULL) {
                    yaml_event_delete(&event);
                    goto error;
                }
                break;

            case YAML_DOCUMENT_END_EVENT:
            case YAML_NO_EVENT:
            case YAML_ALIAS_EVENT:
            default:
                break;
        }
        done = (event.type == YAML_DOCUMENT_END_EVENT);
        yaml_event_delete(&event);
    }
    return doc;
error:
    if (doc) delete doc;
    return NULL;
}

void YAMLAdapter::addFormatErrorFromParser(yaml_parser_t& parser) {
    if (parser.error != YAML_NO_ERROR) {
        YAMLFormatError *err = new YAMLFormatError((int) parser.mark.line, (int) parser.mark.column);
        err->msg = string(parser.problem);
        Error::addError(err);
    }
}

int YAMLAdapter::saveFile(const char *fileName, DocElement *root) {
    yaml_emitter_t emitter;
    yaml_event_t event;
    FILE *out = NULL;

    if (!yaml_emitter_initialize(&emitter)) {
        Error::addError(new YAMLEmitterInitError(1, 1));
        goto err;

    }

    out = fopen(fileName, "wb");
    if (out == NULL)
        goto err;

    yaml_emitter_set_output_file(&emitter, out);
    yaml_emitter_set_encoding(&emitter, YAML_UTF8_ENCODING);

    if (!yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter start event initialize error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_emitter_emit(&emitter, &event)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter start event emit error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_document_start_event_initialize(&event, NULL, NULL, NULL, true)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter document start event initialize error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_emitter_emit(&emitter, &event)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter document start event emit error.";
        Error::addError(err);
        goto err;
    }

    // ----- Main Start ------

    if (emitObject(&emitter, &event, root)) goto err;

    // ----- Main End -----

    if (!yaml_document_end_event_initialize(&event, true)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter document end event initialize error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_emitter_emit(&emitter, &event)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter document end event emit error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_stream_end_event_initialize(&event)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter end event initialize error.";
        Error::addError(err);
        goto err;
    }

    if (!yaml_emitter_emit(&emitter, &event)) {
        YAMLEmitterError *err = new YAMLEmitterError(1, 1);
        err->msg = "Emitter end event emit error.";
        Error::addError(err);
        goto err;
    }
    return 0;

err:
    if (out != NULL) fclose(out);
    yaml_emitter_delete(&emitter);
    return -1;
}

int YAMLAdapter::emitObject(yaml_emitter_t *emitter, yaml_event_t *event, DocElement *ele) {
    if (ele->type == DOC_SCALAR) {
        if (!yaml_scalar_event_initialize(event, NULL, NULL,
                                     (yaml_char_t*)((DocScalarElement*)ele)->getValue().c_str(),
                                     (int)((DocScalarElement*)ele)->getValue().length(),
                                     true, true, YAML_ANY_SCALAR_STYLE)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Scalar emit initialize error.";
            Error::addError(err);
            return -1;
        }
        if (!yaml_emitter_emit(emitter, event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Scalar emit error.";
            Error::addError(err);
            return -1;
        }
    }
    if (ele->type == DOC_SEQUENCE) {
        if (!yaml_sequence_start_event_initialize(event, NULL, NULL, true, YAML_ANY_SEQUENCE_STYLE)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Sequence start emit initialize error.";
            Error::addError(err);
            return -1;
        }
        if (!yaml_emitter_emit(emitter, event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Sequence start emit error.";
            Error::addError(err);
            return -1;
        }

        DocSequenceElement *now = (DocSequenceElement*)ele;
        int len = now->getLength();
        for (int i=0; i<len; ++i) {
            DocElement *cur = now->get(i);
            if (emitObject(emitter, event, cur)) return -1;
        }

        if (!yaml_sequence_end_event_initialize(event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Sequence end emit initialize error.";
            Error::addError(err);
            return -1;
        }
        if (!yaml_emitter_emit(emitter, event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Sequence end emit error.";
            Error::addError(err);
            return -1;
        }
    }
    if (ele->type == DOC_OBJECT) {
        if (!yaml_mapping_start_event_initialize(event, NULL, NULL, true, YAML_ANY_MAPPING_STYLE)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Mapping start emit initialize error.";
            Error::addError(err);
            return -1;
        }
        if (!yaml_emitter_emit(emitter, event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Mapping start emit error.";
            Error::addError(err);
            return -1;
        }

        DocObjectElement *now = (DocObjectElement*)ele;
        map<string, DocElement*> *map = now->getMemberMap();
        for (std::map<std::string, DocElement*>::iterator ite = map->begin(); ite != map->end(); ++ite) {
            // key

            if (!yaml_scalar_event_initialize(event, NULL, NULL,
                                              (yaml_char_t*)ite->first.c_str(),
                                              (int)(ite->first.length()),
                                              true, true, YAML_ANY_SCALAR_STYLE)) {
                YAMLEmitterError *err = new YAMLEmitterError(1, 1);
                err->msg = "Mapping key emit initialize error.";
                Error::addError(err);
                return -1;
            }
            if (!yaml_emitter_emit(emitter, event)) {
                YAMLEmitterError *err = new YAMLEmitterError(1, 1);
                err->msg = "Mapping key emit error.";
                Error::addError(err);
                return -1;
            }

            // value
            if (emitObject(emitter, event, ite->second)) return -1;
        }

        if (!yaml_mapping_end_event_initialize(event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Mapping end emit initialize error.";
            Error::addError(err);
            return -1;
        }
        if (!yaml_emitter_emit(emitter, event)) {
            YAMLEmitterError *err = new YAMLEmitterError(1, 1);
            err->msg = "Mapping end emit error.";
            Error::addError(err);
            return -1;
        }
    }
    return 0;
}