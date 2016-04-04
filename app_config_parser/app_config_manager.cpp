#include "app_config_manager.hpp"
#include <sstream>
#include "../utilities/app_logger.hpp"

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

eConfigManager  cConfigManager::init() {
    eConfigManager status = CONFIG_MANAGER_NOK;

    vector<string> serVec;
/*    if (PARSER_CONF_OK == getConfig("serial_description", serVec)) {
        for (vector<string>::iterator it = serVec.begin(); it != serVec.end(); ++it) {
            string brStr;

            int cmStatus = CONFIG_MANAGER_OK;
            cmStatus |= get_baudrateStr(*it, brStr);
            cmStatus |= get_baudrate(*it, niuBusCfg_m.baudrate);
            cmStatus |= get_data(*it, niuBusCfg_m);
            cmStatus |= get_path(*it,niuBusAddr_m);
            cmStatus |= get_parityFlags(*it, niuBusCfg_m);
            cmStatus |= get_stopFlags(*it, niuBusCfg_m);
            cmStatus |= get_flowctrFlags(*it, niuBusCfg_m);
            if (cmStatus == CONFIG_MANAGER_OK) {
                logPrintf(SCREEN_LOG, "UART conf: addres: %s, baudrate: %s\n", niuBusAddr_m.c_str(), brStr.c_str());

                vector<string> device_list_str_m;
                getConfig("device_description", device_list_str_m);
                for (vector<string>::iterator it = device_list_str_m.begin(); it != device_list_str_m.end(); ++it) {
                    string devDescr = removeSpaces(*it);
                    int targetAddress = 0;
                    eDeviceType_t deviceType = DEVICE_TYPE_NUM;
                    sPortConfig portCfg;

                    int devStatus = CONFIG_MANAGER_OK;
                    devStatus |= get_targetAddress(devDescr,targetAddress);
                    devStatus |= get_deviceType(devDescr, deviceType);
                    devStatus |= get_baudrate(devDescr, portCfg.baudrate);
                    devStatus |= get_data(devDescr, portCfg);
                    devStatus |= get_parityFlags(devDescr, portCfg);
                    devStatus |= get_stopFlags(devDescr, portCfg);
                    devStatus |= get_flowctrFlags(devDescr, portCfg);

                    if (devStatus == CONFIG_MANAGER_OK) {
                        struct timeval tv;
                        tv.tv_sec = 0;
                        tv.tv_usec = 5000;
                        cDevice* device = new cDevice(targetAddress, deviceType, devDescr, tv);
                        if(DEVICE_OK == device->init(portCfg)) {
                            deviceList_m.push_back(device);
                            status = CONFIG_MANAGER_OK;
                        }
                    } else {
                        logPrintf(ERROR_LOG, "BIU%d - Wrong parameters.\n",targetAddress);
                    }
                }
            } else {
                logPrintf(ERROR_LOG, "NIU UART - Wrong parameters.\n");
            }
        }
    }
    else {
        logPrintf(ERROR_LOG, "NIU UART - No configuration in config file. \n");
    }

    if (deviceList_m.empty()) {
        logPrintf(ERROR_LOG, "No BIU devices configured.\n");
    }

    return status;
}

eConfigManager cConfigManager::get_baudrateStr(string descr, string& brSpeed) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "baudrate")) {
        istringstream(val) >> skipws >> brSpeed;
        status = CONFIG_MANAGER_OK;
    }*/
    return status;
}

eConfigManager cConfigManager::get_baudrate(string descr, speed_t& brSpeed) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string val;
/*    if (PARSER_CONF_OK == getAttrVal(val, descr, "baudrate")) {
        istringstream(val) >> skipws >> val;
        brSpeed = sPortConfig::baudrate_map[val];
        status = CONFIG_MANAGER_OK;
    }*/
    return status;
}

eConfigManager cConfigManager::get_data(string descr, sPortConfig& portCfg) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string dataStr;
    string val;
/*    if (PARSER_CONF_OK == getAttrVal(val, descr, "data", 1)) {
        istringstream(val) >> skipws >> dataStr;
        if (!dataStr.empty()) {
            portCfg.data_flag_t = sPortConfig::data_map[dataStr];
        }
        status = CONFIG_MANAGER_OK;
    }*/
    return status;
}

eConfigManager cConfigManager::get_parityFlags(string descr, sPortConfig& portCfg) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string parityFlags;
/*    if (PARSER_CONF_OK == getAttrVal(parityFlags, descr, "parity", 1)) {
        if (!parityFlags.empty()) {
            if (parityFlags.find("INPCK") != string::npos)
                portCfg.INPCK_flag_t = sPortConfig::parity_map["INPCK"];
            if (parityFlags.find("PARENB") != string::npos)
                portCfg.PARENB_flag_t = sPortConfig::parity_map["PARENB"];
            if (parityFlags.find("PARODD") != string::npos)
                portCfg.PARODD_flag_t = sPortConfig::parity_map["PARODD"];
        }
        status = CONFIG_MANAGER_OK;
    }*/
    return status;
}

eConfigManager cConfigManager::get_stopFlags(string descr, sPortConfig& portCfg) {
    eConfigManager status = CONFIG_MANAGER_NOK;
    string stopflag;
/*    if (PARSER_CONF_OK == getAttrVal(stopflag, descr, "stop", 1)) {
        if (!stopflag.empty() && stopflag.find("CSTOPB") != string::npos) {
            portCfg.cstopb_flag_t = sPortConfig::cstopb_map["CSTOPB"];
        }
        status = CONFIG_MANAGER_OK;
    }*/
    return status;
}

eConfigManager cConfigManager::get_flowctrFlags(string descr, sPortConfig& portCfg) {
    eConfigManager status = CONFIG_MANAGER_NOK;
/*    string flowctrFlags;
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
    }*/
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
/*    if (PARSER_CONF_OK == getAttrVal(val, descr, "device_type")) {
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
    }*/
    return status;
}

cConfigManager::~cConfigManager() {
/*    while (!deviceList_m.empty()) {
        cDevice* device = deviceList_m.back();
        deviceList_m.pop_back();
        delete device;
    }*/

}
