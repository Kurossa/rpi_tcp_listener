#ifndef SOURCES_APP_CONFIG_MANAGER_HPP
#define SOURCES_APP_CONFIG_MANAGER_HPP

#include "config_parser.h"
#include <map>
#include <list>
#include <string>

typedef enum eConfigManager {
    CONFIG_MANAGER_OK,
    CONFIG_MANAGER_NOK
}eConfigManager_t;

struct sDeviceConfig {
    std::string ipString;
    std::string maskString;
    std::string gatewayString;
    int port;
    std::vector<std::string> sounds;
    std::vector<std::string> soundsInRam;
};


class cConfigManager: public cConfigParser {
public:
    cConfigManager(const char* fileName) : cConfigParser(fileName) {}
    ~cConfigManager() {}

    eConfigManager_t init();
    eConfigManager_t getConfigFile(char* stream, size_t& size);
    std::list<sDeviceConfig>* get_devicesConfigList() { return &deviceConfigList_m; }

private:
    eConfigManager_t get_ipStr(std::string descr, std::string& ipString);
    eConfigManager_t get_maskStr(std::string descr, std::string& ipString);
    eConfigManager_t get_gatewayStr(std::string descr, std::string& gatewayString);
    eConfigManager_t get_port(std::string descr, int& port);
    eConfigManager_t get_sounds(std::string descr, std::vector<std::string>& sounds, std::vector<std::string>& soundsInRam);
    std::list<sDeviceConfig> deviceConfigList_m;

};

#endif /*SOURCES_APP_CONFIG_MANAGER_HPP*/
