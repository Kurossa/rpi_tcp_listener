#include "app_config_parser.hpp"
#include "../utilities/app_logger.hpp"

cConfigParser::cConfigParser(const char* fileName) {
    fileName_m = fileName;
}

eParserStatus cConfigParser::getConfig(const char* descr, vector<string>& destVec) {
    eParserStatus status = PARSER_CONF_ERROR;
    fstream fs;
    char line[1024];

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
                    logPrintf(ERROR_LOG,"ConfigParser: No } for descriptor: %s \n", descr); //if no "}" till EOF
                }
            }
            if (l.find("{") != string::npos) {
                string cfg = l.substr(l.find("{"), l.rfind("}"));
                destVec.push_back(cfg);
            } else {
                logPrintf(ERROR_LOG,"ConfigParser: No { for descriptor: %s \n", descr);
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

eParserStatus cConfigParser::getAttrVal(string& output, string attrLst, string attr, int optional/* = 0*/) {
    eParserStatus status = PARSER_CONF_ERROR;
    size_t pos = attrLst.find(attr);
    if (pos != string::npos) {
        size_t endPos = attrLst.find(",", pos);
        if ((endPos != string::npos) || ((endPos = attrLst.find("}", pos)) != string::npos)) {
            output = attrLst.substr(pos + attr.length() + 1, endPos - pos - attr.length() - 1);
            status = PARSER_CONF_OK;
        } else {
            logPrintf(ERROR_LOG,"ConfigParser: No \",\" between attributes.\n");
        }
    } else if (optional) {
        output = "";
        status = PARSER_CONF_OK;
    } else {
        logPrintf(ERROR_LOG,"ConfigParser: No required attribute \"%s\" found.", attr.c_str());
    }
    return status;
}
