#pragma once

#include "include/hive.h"

namespace hive
{
    MessageBoss * boss = nullptr;

    void sendMessage(StringHash64 system_name, StringHash64 message, char * data,
                     unsigned data_size){};

    MessageBoss::MessageBoss() : Boss(BOSS_IDENTIFIER_MESSAGE) { boss = this; };

    MessageBoss::~MessageBoss(){};

    void MessageBoss::setup(){

        // Aquire boss linkages.
    };

    void MessageBoss::update(float){};

    void MessageBoss::teardown(){};

} // namespace hive