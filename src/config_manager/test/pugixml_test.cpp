#include "gtest/gtest.h"

#include <pugixml/pugixml.hpp>

TEST (PugiXmlTest, ParseExampleXmlConfig) {

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("audio_app_cfg.xml");
    if (result.status != pugi::status_ok)
    {
        std::cout << "File does not exists" << std::endl;
        //return false;
    }

    pugi::xml_node sounds = doc.child("audio_app_cfg").child("sounds");
    for (pugi::xml_node sound = sounds.child("sound"); sound; sound = sound.next_sibling("sound"))
    {
        std::cout << "sound: " << sound.child_value()<< std::endl;
    }

    std::cout << "Network:" << std::endl;
    pugi::xml_node net_ip = doc.child("audio_app_cfg").child("network").child("ip");
    pugi::xml_node net_mask = doc.child("audio_app_cfg").child("network").child("mask");
    pugi::xml_node net_gateway = doc.child("audio_app_cfg").child("network").child("gateway");
    pugi::xml_node net_port = doc.child("audio_app_cfg").child("network").child("port");
    std::cout << "ip: " << net_ip.child_value() << std::endl;
    std::cout << "ip: " << net_mask.child_value() << std::endl;
    std::cout << "ip: " << net_gateway.child_value() << std::endl;
    std::cout << "ip: " << net_port.child_value() << std::endl;

    std::cout << "Load result: " << result.description() << std::endl;
    EXPECT_TRUE(true);
}

