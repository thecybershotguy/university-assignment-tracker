#pragma once

#include "AppConfig.h"

class NetworkManager {

    public: 
        explicit NetworkManager(const AppConfig& cfg);

        bool begin();

    private:
        const AppConfig& cfg_;
        static constexpr const char* NTP_SERVER = "pool.ntp.org";
        
        bool connectWifi_();
        bool syncTime_();
        static bool waitForValidTime_(int timeoutMs);
};