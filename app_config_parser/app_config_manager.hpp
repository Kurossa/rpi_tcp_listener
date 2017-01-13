#ifndef SOURCES_APP_CONFIG_MANAGER_HPP
#define SOURCES_APP_CONFIG_MANAGER_HPP

#include <app_config_parser.hpp>
#include <termios.h>
#include <map>
#include <list>

using namespace std;

typedef enum eConfigManager {
    CONFIG_MANAGER_OK,
    CONFIG_MANAGER_NOK
}eConfigManager_t;

struct sDeviceConfig {
    string ipString;
    string maskString;
    string gatewayString;
    int port;
    vector<string> sounds;
    vector<string> soundsInRam;
};


class cConfigManager: public cConfigParser {
public:
    cConfigManager(const char* fileName) : cConfigParser(fileName) {}
    ~cConfigManager() {}

    eConfigManager_t init();
    eConfigManager_t getConfigFile(char* stream, size_t& size);
    list<sDeviceConfig>* get_devicesConfigList() { return &deviceConfigList_m; }

private:
    eConfigManager_t get_ipStr(string descr, string& ipString);
    eConfigManager_t get_maskStr(string descr, string& ipString);
    eConfigManager_t get_gatewayStr(string descr, string& gatewayString);
    eConfigManager_t get_port(string descr, int& port);
    eConfigManager_t get_sounds(string descr, vector<string>& sounds, vector<string>& soundsInRam);
    list<sDeviceConfig> deviceConfigList_m;

};

#endif /*SOURCES_APP_CONFIG_MANAGER_HPP*/
