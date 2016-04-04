#ifndef SOURCES_APP_CONFIG_MANAGER_HPP
#define SOURCES_APP_CONFIG_MANAGER_HPP

#include "app_config_parser.hpp"
#include <termios.h>
#include <map>
#include <list>

using namespace std;

enum eConfigManager {
    CONFIG_MANAGER_OK,
    CONFIG_MANAGER_NOK
};

struct sPortConfig {

};

typedef enum eDeviceType {

}eDeviceType_t;

class cConfigManager: public cConfigParser {
public:
    cConfigManager(const char* fileName);
    ~cConfigManager();

    eConfigManager init();

    eConfigManager get_baudrateStr(string descr, string& brSpeed);
    eConfigManager get_baudrate(string descr, speed_t& brSpeed);
    eConfigManager get_data(string descr, sPortConfig& conf);
    eConfigManager get_parityFlags(string descr, sPortConfig& portCfg);
    eConfigManager get_stopFlags(string descr, sPortConfig& portCfg);
    eConfigManager get_flowctrFlags(string descr, sPortConfig& portCfg);
    eConfigManager get_path(string descr, string& path);
    eConfigManager get_targetAddress(string descr, int& devAddres);
    eConfigManager get_deviceType(string descr, eDeviceType_t& devType);

    //list<cDevice*>* get_deviceList() { return &deviceList_m; }

    sPortConfig niuBusCfg_m;
    string niuBusAddr_m;

private:
  //  list<cDevice*> deviceList_m;

};

#endif /*SOURCES_APP_CONFIG_MANAGER_HPP*/
