#include <app_config_manager.hpp>
#include <string.h>
#include <sstream>
#include <app_logger.hpp>
#include <app_consts.hpp>

static string removeSpaces(string input) {
    string output;
    for (string::iterator it = input.begin(); it != input.end(); ++it) {
        if (*it != ' ' && *it != '\t') {
            output.push_back(*it);
        }
    }
    return output;
}

eConfigManager_t cConfigManager::init() {
    eConfigManager_t status = CONFIG_MANAGER_NOK;

    vector<string> ipConfVect;
    if (PARSER_CONF_OK == getConfig("ip_config", ipConfVect)) {
        for (vector<string>::iterator it = ipConfVect.begin(); it != ipConfVect.end(); ++it) {

            sDeviceConfig devConfig;

            int cmStatus = CONFIG_MANAGER_OK;
            cmStatus |= get_ipStr(*it, devConfig.ipString);
            cmStatus |= get_maskStr(*it, devConfig.maskString);
            cmStatus |= get_port(*it, devConfig.port);

            if (cmStatus == CONFIG_MANAGER_OK) {
                logPrintf(SCREEN_LOG, "Ethernet configuration: ip address: %s:%d\n", devConfig.ipString.c_str(), devConfig.port);

                vector<string> soundsConfVect;
                getConfig("sounds_config", soundsConfVect);
                for (vector<string>::iterator it = soundsConfVect.begin(); it != soundsConfVect.end(); ++it) {
                    string soundsDescr = removeSpaces(*it);
                    int soundStatus = CONFIG_MANAGER_OK;
                    soundStatus = get_sounds(soundsDescr, devConfig.sounds, devConfig.soundsInRam);

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

eConfigManager_t cConfigManager::getConfigFile(char* stream, size_t& size) {
    eConfigManager_t status = CONFIG_MANAGER_NOK;
    bzero(stream, size);
    size = 0;
    FILE* source = fopen(fileName_m, "r");
    if (source) {
        size = fread(stream, 1, 2048, source);
        status = CONFIG_MANAGER_OK;
    }
    fclose(source);
    return status;
}

eConfigManager_t cConfigManager::get_ipStr(string descr, string& ipString) {
    eConfigManager_t status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "ip")) {
        istringstream(val) >> skipws >> ipString;
        status = CONFIG_MANAGER_OK;
    }
    return status;
}

eConfigManager_t cConfigManager::get_maskStr(string descr, string& ipString) {
    eConfigManager_t status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "mask")) {
        istringstream(val) >> skipws >> ipString;
        status = CONFIG_MANAGER_OK;
    }
    return status;
}

eConfigManager_t cConfigManager::get_port(string descr, int& port) {
    eConfigManager_t status = CONFIG_MANAGER_NOK;
    string val;
    if (PARSER_CONF_OK == getAttrVal(val, descr, "port")) {
        istringstream(val) >> skipws >> port;
        status = CONFIG_MANAGER_OK;
    }
    return status;
}

eConfigManager_t cConfigManager::get_sounds(string descr, vector<string>& sounds, vector<string>& soundsInRam) {
    eConfigManager_t status = CONFIG_MANAGER_NOK;
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
                string encodedFile(encoded);
                encodedFile.append(attrByNum);
                sounds.push_back(encodedFile);
                string ramFile(ram);
                ramFile.append(attrByNum);
                soundsInRam.push_back(ramFile);
                logPrintf(SCREEN_LOG,"Sound %d name: %s\n",i+1, attrByNum.c_str());
            }
        }

        if (CONFIG_MANAGER_OK == soundsStatus) {
            status = CONFIG_MANAGER_OK;
        }
    }
    return status;
}

