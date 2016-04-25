#ifndef SOURCES_APP_CONFIG_MANAGER_HPP
#define SOURCES_APP_CONFIG_MANAGER_HPP

#include <app_config_parser.hpp>
#include <termios.h>
#include <map>
#include <list>

using namespace std;

enum eConfigManager {
    CONFIG_MANAGER_OK,
    CONFIG_MANAGER_NOK
};

struct sDeviceConfig {
    string ipString;
    int port;
    vector<string> sounds;
};

typedef enum eDeviceType {

}eDeviceType_t;

class cConfigManager: public cConfigParser {
public:
    cConfigManager(const char* fileName);
    ~cConfigManager();

    eConfigManager init();

    list<sDeviceConfig>* get_devicesConfigList() { return &deviceConfigList_m; }

private:
    eConfigManager get_ipStr(string descr, string& ipString);
    eConfigManager get_port(string descr, int& port);
    eConfigManager get_sounds(string descr, vector<string>& sounds);
    list<sDeviceConfig> deviceConfigList_m;

};

#endif /*SOURCES_APP_CONFIG_MANAGER_HPP*/
