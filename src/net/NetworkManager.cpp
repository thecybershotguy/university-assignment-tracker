#include "NetworkManager.h"
#include <WiFi.h>
#include <time.h>

NetworkManager::NetworkManager(const AppConfig& cfg) : cfg_(cfg)
{
}

bool NetworkManager::begin()
{
    if (!connectWifi_())
        return false;
    if (!syncTime_())
        return false;
    return true;
}

bool NetworkManager::connectWifi_()
{
    WiFi.begin(cfg_.wifiSsid, cfg_.wifiPass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\nWifi Connected");
    return true;
}

bool NetworkManager::waitForValidTime_(int timeoutMs)
{
    struct tm now_tm;
    int waited = 0;

    while (waited < timeoutMs)
    {
        getLocalTime(&now_tm);
        if (now_tm.tm_year >= 120)
        {  // 120 == year 2020 (tm_year is since 1900)
            return true;
        }
        delay(500);
        waited += 500;
        Serial.print(".");
    }
    return false;
}

bool NetworkManager::syncTime_()
{
    configTime(cfg_.timezoneOffsetSec, 0, NTP_SERVER);
    Serial.print("Syncing NTP");

    // Keep identical behavior: block until time looks valid
    bool ok = waitForValidTime_(15000);
    if (ok)
    {
        Serial.println("\nTime Synced!");
        return true;
    }

    Serial.println("\nTime Sync Failed!");
    return false;
}
