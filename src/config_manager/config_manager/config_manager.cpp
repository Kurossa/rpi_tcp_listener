#include "config_manager.h"
#include <app_config/app_constants.h>
#include <utilities/logger.h>
#include <utilities/zip.h>
#include <pugixml/pugixml.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

using std::vector;
using std::string;

using namespace utils;
using namespace config;

ConfigManager::ConfigManager()
{
    default_configuration_m = {
        "127.0.0.1",
        "255.255.255.0",
        "",
        2055,
        std::vector<std::string>(),
        std::vector<std::string>()};
    configuration_m = default_configuration_m;
}

config::Status ConfigManager::ParseConfigFile(const char* file_name)
{
    //TODO: add chceking values of ipv4
    return ParseXml(file_name);
}

config::Status ConfigManager::ParseXml(const char* file_name)
{
    Status parse_status = Status::OK;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file_name);
    if (result.status != pugi::status_ok)
    {
        logPrintf(LogLevel::ERROR, "File does not exists\n");
        return FILE_NOT_FOUND;
    }

    pugi::xml_node sounds = doc.child("audio_app_cfg").child("sounds");
    for (pugi::xml_node sound = sounds.child("sound"); sound; sound = sound.next_sibling("sound"))
    {
        configuration_m.compressed_files.push_back(sound.child_value());
        string file_in_ram(string(ram_g) + sound.child_value());
        ZipStatus status = utils::ZipUncompress(sound.child_value(), file_in_ram.c_str());
        if (ZipStatus::OK != status) {
            logPrintf(LogLevel::WARNING, "Error decoding file: %s, no file append to list.\n",sound.child_value());
            file_in_ram.clear();
            parse_status = Status::NOT_ALL_NODES_PARSED;
        }
        configuration_m.sound_files.push_back(file_in_ram);
    }

    pugi::xml_node net_ip = doc.child("audio_app_cfg").child("network").child("ip");
    pugi::xml_node net_mask = doc.child("audio_app_cfg").child("network").child("mask");
    pugi::xml_node net_gateway = doc.child("audio_app_cfg").child("network").child("gateway");
    pugi::xml_node net_port = doc.child("audio_app_cfg").child("network").child("port");
    //TODO: Add checking ip values;
    configuration_m.ip_str = net_ip.child_value();
    configuration_m.mask_str = net_mask.child_value();
    configuration_m.gateway_str = net_gateway.child_value();
    std::istringstream(net_port.child_value()) >> std::skipws >> configuration_m.port;

    return parse_status;
}

config::Status ConfigManager::GetConfigFromFile(std::string& stream) const {
    char buffer[FILE_BLOCK];
    FILE* source = fopen(config_file_g, "rb");
    if (source) {
        while (fread(buffer, 1, FILE_BLOCK, source)) {
            memset(&buffer, 0, FILE_BLOCK);
            stream.append(std::string(buffer));
        }
        fclose(source);
        return config::OK;
    }
    return config::FILE_NOT_FOUND;
}

config::Status ConfigManager::SaveConfigToFile(std::string& stream) const
{
    FILE* dest = fopen(config_file_g, "wb");
    if (dest)
    {
        fwrite(stream.c_str(), 1, stream.size(), dest);
        fclose(dest);
        return config::OK;
    }
    return config::FILE_NOT_FOUND;
}

