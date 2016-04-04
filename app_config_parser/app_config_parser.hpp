#ifndef SOURCES_APP_CONFIG_PARSER_HPP
#define SOURCES_APP_CONFIG_PARSER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;

enum eParserStatus {
    PARSER_CONF_OK = 0,
    PARSER_CONF_ERROR,
    PARSER_CONF_EMPTY,
    PARSER_CONF_NUM
};

class cConfigParser
{
    const char* fileName_m;
public:
    cConfigParser(const char* fileName);
    eParserStatus getConfig(const char* descr, vector<string>& destVec);
    eParserStatus getAttrVal(string& output, string attrLst, string attr, int optional = 0);
};


#endif /*SOURCES_APP_CONFIG_PARSER_HPP*/
