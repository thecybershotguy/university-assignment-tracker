#include "setup/SetupMode.h"
#include <WiFi.h>
#include "ui/SetupScreenRenderer.h"
#include "setup/SetupWebPortal.h"

SetupMode::SetupMode(M5EPD_Canvas& canvas, NvsConfigProvider& nvs) : canvas_(canvas), nvs_(nvs)
{
}

String SetupMode::chipSuffix_()
{
    uint64_t mac = ESP.getEfuseMac();
    uint16_t tail = (uint16_t)(mac & 0xFFFF);
    char buf[8];
    snprintf(buf, sizeof(buf), "%04X", tail);
    return String(buf);
}

void SetupMode::startAp_()
{
    apSsid_ = "EpaperSetup-" + chipSuffix_();

    WiFi.mode(WIFI_AP);
    // Open AP (no password) to keep setup friction low.
    // If you want a password, use: WiFi.softAP(apSsid_.c_str(), "setup1234");
    bool ok = WiFi.softAP(apSsid_.c_str());

    apIp_ = WiFi.softAPIP();

    Serial.printf("[SETUP] AP start: %s -> %s (%s)\n", apSsid_.c_str(), apIp_.toString().c_str(),
                  ok ? "OK" : "FAIL");
}

void SetupMode::run(SetupEntryReason reason)
{
    startAp_();
    drawSetupScreen(canvas_, apSsid_, apIp_, reason);

    SetupWebPortal portal(nvs_);
    portal.begin();

    while (true)
    {
        portal.loop();
        delay(5);
    }
}