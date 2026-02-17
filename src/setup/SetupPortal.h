#pragma once
#include <M5EPD.h>
#include <WiFi.h>
#include <WebServer.h>
#include "AppConfig.h"
#include "storage/config/NvsConfigProvider.h"

class SetupPortal
{
   public:
    SetupPortal(M5EPD_Canvas& canvas, NvsConfigProvider& nvs)
        : canvas_(canvas), nvs_(nvs), server_(80)
    {
    }

    // Blocking: runs the portal until save/clear triggers ESP.restart().
    void run();

   private:
    static constexpr int MAX_COURSES = 8;

    M5EPD_Canvas& canvas_;
    NvsConfigProvider& nvs_;
    WebServer server_;

    String apSsid_;
    IPAddress apIp_;

    void startAp_();
    void drawSetupScreen_();

    void registerRoutes_();
    void handleIndex_();
    void handleStatus_();
    void handleSave_();
    void handleClear_();

    static String chipSuffix_();
};