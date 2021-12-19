#ifndef CONFIG_MANAGER_CONFIG_MANAGER_H_
#define CONFIG_MANAGER_CONFIG_MANAGER_H_

#include <utilities/logger.h>
#include <map>
#include <list>
#include <string>
#include <vector>

namespace config
{

enum Status {
    OK,
    MISSING_FIELDS_IN_CONFIG, //This is not a failure, default values will be taken
    FILE_NOT_FOUND,
    CONFIG_NUM,
};

struct Configuration {
    std::string ip_str;
    std::string gateway_str;
    std::string mask_str;
    int port;
    std::vector<std::string> compressed_files;
    std::vector<std::string> sound_files;
    int autoplay;
};


class ConfigManager {
public:
    ConfigManager(utils::Logger& logger);
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
    utils::Logger& logger_m;
};

} // config

#endif // CONFIG_MANAGER_CONFIG_MANAGER_H_
