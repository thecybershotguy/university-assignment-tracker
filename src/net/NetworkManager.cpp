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
    Serial.printf("Connecting to WiFi: %s\n", cfg_.wifiSsid.c_str());

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(100);

    WiFi.begin(cfg_.wifiSsid.c_str(), cfg_.wifiPass.c_str());

    const unsigned long timeoutMs = 15000;  // 15s
    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        if (millis() - start > timeoutMs)
        {
            Serial.println("\n[NET] WiFi connect TIMEOUT");
            WiFi.mode(WIFI_OFF);
            return false;
        }

        delay(500);
        Serial.print(".");
    }

    Serial.println("\n[NET] WiFi Connected");
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
    Serial.print("\nSyncing NTP");

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
