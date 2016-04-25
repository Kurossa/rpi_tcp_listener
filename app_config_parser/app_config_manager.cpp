#include <app_config_manager.hpp>
#include <sstream>
#include <app_logger.hpp>

string removeSpaces(string input) {
    string output;
    for (string::iterator it = input.begin(); it != input.end(); ++it) {
        if (*it != ' ' && *it != '\t') {
            output.push_back(*it);
        }
    }
    return output;
}

cConfigManager::cConfigManager(const char* fileName) :
        cConfigParser(fileName) {
}

eConfigManager cConfigManager::init() {
    eConfigManager status = CONFIG_MANAGER_NOK;

    vector<string> ipConfVect;
    if (PARSER_CONF_OK == getConfig("ip_config", ipConfVect)) {
        for (vector<string>::iterator it = ipConfVect.begin(); it != ipConfVect.end(); ++it) {

            sDeviceConfig devConfig;

            int cmStatus = CONFIG_MANAGER_OK;
            cmStatus |= get_ipStr(*it, devConfig.ipString);
            cmStatus |= get_port(*it, devConfig.port);

            if (cmStatus == CONFIG_MANAGER_OK) {
                logPrintf(SCREEN_LOG, "Ethernet configuration: ip address: %s:%d\n", devConfig.ipString.c_str(), devConfig.port);

                vector<string> soundsConfVect;
                getConfig("sounds_config", soundsConfVect);
                for (vector<string>::iterator it = soundsConfVect.begin(); it != soundsConfVect.end(); ++it) {
                    string soundsDescr = removeSpaces(*it);
                    int soundStatus = CONFIG_MANAGER_OK;
                    soundStatus = get_sounds(soundsDescr, devConfig.sounds);

                    if (soundStatus == CONFIG_MANAGER_OK) {
                        deviceConfigList_m.push_back(devConfig);
                        status = CONFIG_MANAGER_OK;
                    } else {
                        logPrintf(ERROR_LOG, "Config file - Wrong sounds parameters.\n");
                    }
                }
            } else {
                logPrintf(ERROR_LOG, "Config file - Wrong parameters.\n");
            }
        }
    } else {
        logPrintf(ERROR_LOG, "Config file - No configuration in config file. \n");
    }

    if (deviceConfigList_m.empty()) {
        logPrintf(ERROR_LOG, "Config file - No devices configured.\n");
    }

    return status;
}

eConfigManager cConfigManager::get_ipStr(string descr, string& ipString) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "ip")) {
        istringstream(val) >> skipws >> ipString;
        status = CONFIG_MANAGER_OK;
    }
    return status;
}

eConfigManager cConfigManager::get_port(string descr, int& port) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "port")) {
        istringstream(val) >> skipws >> port;
        status = CONFIG_MANAGER_OK;
    }
    return status;
}

eConfigManager cConfigManager::get_sounds(string descr, vector<string>& sounds) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "sound")) {
        istringstream(val) >> skipws >> val;
        int soundsNum = 0;
        getAttrNum(soundsNum,val);
        logPrintf(SCREEN_LOG,"Sound(s) number: %d\n",soundsNum);
        int soundsStatus = CONFIG_MANAGER_OK;
        for (int i = 0; i < soundsNum; ++i) {
            string attrByNum;
            soundsStatus |= getAttrByNum(attrByNum, i, val);
            if (CONFIG_MANAGER_OK == soundsStatus) {
                sounds.push_back(attrByNum);
                logPrintf(SCREEN_LOG,"Sound %d name: %s\n",i+1, attrByNum.c_str());
            }
        }

        if (CONFIG_MANAGER_OK == soundsStatus) {
            status = CONFIG_MANAGER_OK;
        }
    }
    return status;
}

/*
 eConfigManager cConfigManager::get_baudrateStr(string descr, string& brSpeed) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr, "baudrate")) {
 istringstream(val) >> skipws >> brSpeed;
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_baudrate(string descr, speed_t& brSpeed) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr, "baudrate")) {
 istringstream(val) >> skipws >> val;
 brSpeed = sPortConfig::baudrate_map[val];
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_data(string descr, sPortConfig& portCfg) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string dataStr;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr, "data", 1)) {
 istringstream(val) >> skipws >> dataStr;
 if (!dataStr.empty()) {
 portCfg.data_flag_t = sPortConfig::data_map[dataStr];
 }
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_parityFlags(string descr, sPortConfig& portCfg) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string parityFlags;
 if (PARSER_CONF_OK == getAttrVal(parityFlags, descr, "parity", 1)) {
 if (!parityFlags.empty()) {
 if (parityFlags.find("INPCK") != string::npos)
 portCfg.INPCK_flag_t = sPortConfig::parity_map["INPCK"];
 if (parityFlags.find("PARENB") != string::npos)
 portCfg.PARENB_flag_t = sPortConfig::parity_map["PARENB"];
 if (parityFlags.find("PARODD") != string::npos)
 portCfg.PARODD_flag_t = sPortConfig::parity_map["PARODD"];
 }
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_stopFlags(string descr, sPortConfig& portCfg) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string stopflag;
 if (PARSER_CONF_OK == getAttrVal(stopflag, descr, "stop", 1)) {
 if (!stopflag.empty() && stopflag.find("CSTOPB") != string::npos) {
 portCfg.cstopb_flag_t = sPortConfig::cstopb_map["CSTOPB"];
 }
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_flowctrFlags(string descr, sPortConfig& portCfg) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string flowctrFlags;
 if (PARSER_CONF_OK == getAttrVal(flowctrFlags, descr, "flowcontrol", 1)) {
 if (!flowctrFlags.empty()) {
 if (flowctrFlags.find("IXON") != string::npos)
 portCfg.IXON_flag_t = sPortConfig::flowctr_map["IXON"];
 if (flowctrFlags.find("IXOFF") != string::npos)
 portCfg.IXOFF_flag_t = sPortConfig::flowctr_map["IXOFF"];
 if (flowctrFlags.find("CRTSCTS") != string::npos) {
 portCfg.CRTSCTS_flag_t = sPortConfig::flowctr_map["CRTSCTS"];
 }
 }
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_path(string descr, string& path) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr, "path")) {
 istringstream(val) >> skipws >> path;
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_targetAddress(string descr, int& devAddres) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr,"address")) {
 istringstream(val) >> skipws >> devAddres;
 status = CONFIG_MANAGER_OK;
 }
 return status;
 }

 eConfigManager cConfigManager::get_deviceType(string descr, eDeviceType_t& devType) {
 eConfigManager status = CONFIG_MANAGER_NOK;
 string val;
 if (PARSER_CONF_OK == getAttrVal(val, descr, "device_type")) {
 istringstream(val) >> skipws >> val;
 eDeviceType_t deviceType;
 status = CONFIG_MANAGER_OK;
 if (val.compare("RS232") == 0)
 devType = DEVICE_TYPE_RS232;
 else if (val.compare("M_BUS") == 0)
 devType = DEVICE_TYPE_M_BUS;
 else if (val.compare("MODBUS_RS485") == 0)
 devType = DEVICE_TYPE_MODBUS_RS485;
 else if (val.compare("KNX") == 0)
 devType = DEVICE_TYPE_KNX;
 else if (val.compare("NUM") == 0)
 devType = DEVICE_TYPE_NUM;
 else {
 logPrintf(ERROR_LOG, "Unrecognized device type: %s\n", val.c_str());
 devType = DEVICE_TYPE_NUM;
 status = CONFIG_MANAGER_NOK;
 }
 }
 return status;
 }
 */

cConfigManager::~cConfigManager() {
    /*   while (!deviceList_m.empty()) {
     cDevice* device = deviceList_m.back();
     deviceList_m.pop_back();
     delete device;
     }*/
}
