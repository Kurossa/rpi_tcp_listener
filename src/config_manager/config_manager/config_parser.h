#ifndef SOURCES_APP_CONFIG_PARSER_H
#define SOURCES_APP_CONFIG_PARSER_H

#include <string>
#include <vector>

#define MAX_CONFIG_LINE_SIZE (1024)

typedef enum eParserStatus {
    PARSER_CONF_OK = 0,
    PARSER_CONF_ERROR,
    PARSER_CONF_EMPTY,
    PARSER_CONF_NO_ATTR_BY_NUM,
    PARSER_CONF_NUM
}eParserStatus_t;

class cConfigParser
{
public:
    cConfigParser(const char* fileName);
protected:
    eParserStatus_t getConfig(const char* descr, std::vector<std::string>& destVec);
    eParserStatus_t getAttrVal(std::string& output, std::string attrLst, std::string attr, int optional = 0);
    eParserStatus_t getAttrNum(int& attrNum, std::string attrVal);
    eParserStatus_t getAttrByNum(std::string& attrByNum, int attrNum, std::string attrVal);
    const char* fileName_m;
};


#endif /*SOURCES_APP_CONFIG_PARSER_H*/
