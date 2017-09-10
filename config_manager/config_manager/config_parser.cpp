#include "config_parser.h"
#include <utilities/logger.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

cConfigParser::cConfigParser(const char* fileName) {
    fileName_m = fileName;
}

eParserStatus_t cConfigParser::getConfig(const char* descr, vector<string>& destVec) {
    eParserStatus_t status = PARSER_CONF_ERROR;
    fstream fs;
    char line[MAX_CONFIG_LINE_SIZE];

    fs.open(fileName_m, fstream::in);

    while (fs.getline(line, sizeof(line))) {
        string l = string(line);
        if (l.find(descr) != string::npos) {
            while (1) {
                size_t rDel = l.rfind("}");
                if (rDel != string::npos) { //no "}" sign in this line
                    break;
                } else if (fs.getline(line, sizeof(line))) { // if no } in this line read next line, contract to old one
                    l += string(line);
                } else {
                    logPrintf(ERROR_LOG, "ConfigParser: No } for descriptor: %s \n", descr); //if no "}" till EOF
                }
            }
            if (l.find("{") != string::npos) {
                string cfg = l.substr(l.find("{"), l.rfind("}"));
                destVec.push_back(cfg);
            } else {
                logPrintf(ERROR_LOG, "ConfigParser: No { for descriptor: %s \n", descr);
            }
        }
    }
    fs.close();
    if (destVec.empty()) {
        logPrintf(ERROR_LOG, "ConfigParser: Configuration empty for descriptor: %s \n", descr);
        status = PARSER_CONF_EMPTY;
    } else {
        status = PARSER_CONF_OK;
    }
    return status;
}

eParserStatus_t cConfigParser::getAttrVal(string& output, string attrLst, string attr, int optional/* = 0*/) {
    eParserStatus_t status = PARSER_CONF_ERROR;
    size_t pos = attrLst.find(attr);
    if (pos != string::npos) {
        size_t endPos = attrLst.find(",", pos);
        if ((endPos != string::npos) || ((endPos = attrLst.find("}", pos)) != string::npos)) {
            output = attrLst.substr(pos + attr.length() + 1, endPos - pos - attr.length() - 1);
            status = PARSER_CONF_OK;
        } else {
            logPrintf(ERROR_LOG, "ConfigParser: No \",\" between attributes.\n");
        }
    } else if (optional) {
        output = "";
        status = PARSER_CONF_OK;
    } else {
        logPrintf(ERROR_LOG, "ConfigParser: No required attribute \"%s\" found.", attr.c_str());
    }
    return status;
}
eParserStatus_t cConfigParser::getAttrNum(int& attrNum, string attrVal) {
    eParserStatus_t status = PARSER_CONF_ERROR;

    attrNum = 0;
    size_t pos = 0;
    do {
        pos = attrVal.find("|", pos + 1);
        ++attrNum;

    } while (pos != string::npos);

    return status;
}

eParserStatus_t cConfigParser::getAttrByNum(string& attrByIndex, int attrIndex, string attrVal) {
    eParserStatus_t status = PARSER_CONF_ERROR;

    size_t pos = 0;
    for (int i = 0; i < attrIndex; ++i) {
        pos = attrVal.find("|", pos);
        if (pos == string::npos) {
            status = PARSER_CONF_NO_ATTR_BY_NUM;
            break;
        }
        ++pos;
    }

    if (status != PARSER_CONF_NO_ATTR_BY_NUM) {
        size_t pos_end = attrVal.find("|", pos);
        if (pos_end != string::npos) {
            attrByIndex = attrVal.substr(pos,pos_end-pos);
        } else {
            attrByIndex = attrVal.substr(pos);
        }

        if (!attrByIndex.empty()) {
            status = PARSER_CONF_OK;
        }
    }

    return status;
}

