#ifndef CONFIG_MANAGER_CONFIG_MANAGER_H_
#define CONFIG_MANAGER_CONFIG_MANAGER_H_

#include <map>
#include <list>
#include <string>
#include <vector>

namespace config
{

enum Status {
    OK,
    FILE_NOT_FOUND,
    NOT_ALL_NODES_PARSED, //This is not a failure, default values will be taken
    CONFIG_NUM,
};

struct Configuration {
    std::string ip_str;
    std::string gateway_str;
    std::string mask_str;
    int port;
    std::vector<std::string> compressed_files;
    std::vector<std::string> sound_files;
};


class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager() {}

    config::Status ParseConfigFile(const char* file_name);
    config::Configuration GetConfig() const { return configuration_m; }
    config::Configuration GetDefaultConfig() const { return default_configuration_m; }
    config::Status GetConfigFromFile(std::string& stream) const;
    config::Status SaveConfigToFile(std::string& stream) const;

private:
    config::Status ParseXml(const char* file_name);

    config::Configuration configuration_m;
    config::Configuration default_configuration_m;

};

} // config

#endif // CONFIG_MANAGER_CONFIG_MANAGER_H_
