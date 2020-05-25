#pragma once

#include "include/hive.h"

namespace hive
{
    void sendMessage(StringHash64 system_name, StringHash64 message, char * data,
                     unsigned data_size)
    {
        std::cout << system_name << " " << message << std::endl;
        std::cout << (std::string)system_name << " " << (std::string)message << std::endl;
    };

    MessageBoss::MessageBoss(){};

    MessageBoss::~MessageBoss(){};

    void MessageBoss::setup(){

        // Aquire boss linkages.
    };

    void MessageBoss::update(float){};

    void MessageBoss::teardown(){};

} // namespace hive