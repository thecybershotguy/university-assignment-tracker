#include "setup/SetupPortal.h"
#include "setup/SetupPages.h"
#include "ui/SetupScreenRenderer.h"
#include <ArduinoJson.h>

static void safePushCanvas(M5EPD_Canvas& canvas)
{
    canvas.pushCanvas(0, 0, UPDATE_MODE_GC16);
}

String SetupPortal::chipSuffix_()
{
    uint64_t mac = ESP.getEfuseMac();
    uint16_t tail = (uint16_t)(mac & 0xFFFF);
    char buf[8];
    snprintf(buf, sizeof(buf), "%04X", tail);
    return String(buf);
}

void SetupPortal::startAp_()
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

void SetupPortal::drawSetupScreen_()
{
    drawSetupScreen(canvas_, apSsid_, apIp_);
}

void SetupPortal::registerRoutes_()
{
    server_.on("/", HTTP_GET, [this]() { handleIndex_(); });
    server_.on("/status", HTTP_GET, [this]() { handleStatus_(); });
    server_.on("/save", HTTP_POST, [this]() { handleSave_(); });
    server_.on("/clear", HTTP_POST, [this]() { handleClear_(); });

    server_.onNotFound([this]() { server_.send(404, "text/plain", "Not Found"); });
}

void SetupPortal::handleIndex_()
{
    server_.send_P(200, "text/html", SETUP_INDEX_HTML);
}

void SetupPortal::handleStatus_()
{
    StaticJsonDocument<512> doc;
    doc["apSsid"] = apSsid_;
    doc["apIp"] = apIp_.toString();
    doc["nvsHasConfig"] = nvs_.hasConfig();

    String out;
    serializeJson(doc, out);
    server_.send(200, "application/json", out);
}

void SetupPortal::handleSave_()
{
    // Basic validation
    String ssid = server_.arg("ssid");
    String pass = server_.arg("pass");
    String tzStr = server_.arg("tz");

    ssid.trim();
    pass.trim();
    tzStr.trim();

    if (ssid.length() == 0 || pass.length() == 0)
    {
        server_.send(400, "text/plain", "SSID and password are required.");
        return;
    }

    long tzOff = -25200;  // default -7h
    if (tzStr.length() > 0)
        tzOff = tzStr.toInt();

    AppConfig cfg;
    cfg.wifiSsid = ssid;
    cfg.wifiPass = pass;
    cfg.timezoneOffsetSec = tzOff;
    cfg.courses.clear();

    for (int i = 0; i < MAX_COURSES; i++)
    {
        String nameKey = "c" + String(i) + "n";
        String urlKey = "c" + String(i) + "u";

        String name = server_.arg(nameKey);
        String url = server_.arg(urlKey);

        name.trim();
        url.trim();

        // Ignore blank rows
        if (url.length() == 0)
            continue;

        Course c;
        c.name = (name.length() ? name : String("Course ") + String(i + 1));
        c.url = url;
        cfg.courses.push_back(c);
    }

    if (cfg.courses.empty())
    {
        server_.send(400, "text/plain", "At least one course URL is required.");
        return;
    }

    Serial.printf("[SETUP] Saving config: ssid=%s, courses=%d, tz=%ld\n", cfg.wifiSsid.c_str(),
                  (int)cfg.courses.size(), cfg.timezoneOffsetSec);

    bool ok = nvs_.save(cfg);
    if (!ok)
    {
        server_.send(500, "text/plain", "Failed to save config to NVS.");
        return;
    }

    server_.send(200, "text/plain", "Saved! Rebooting...");
    delay(400);
    ESP.restart();
}

void SetupPortal::handleClear_()
{
    Serial.println("[SETUP] Clearing NVS config...");
    bool ok = nvs_.clear();

    server_.send(ok ? 200 : 500, "text/plain", ok ? "Cleared. Rebooting..." : "Clear failed.");
    delay(400);
    ESP.restart();
}

void SetupPortal::run()
{
    startAp_();
    drawSetupScreen_();

    registerRoutes_();
    server_.begin();

    Serial.printf("[SETUP] Portal running. Open http://%s/\n", apIp_.toString().c_str());

    // Block forever until user saves/clears (which restarts the device)
    while (true)
    {
        server_.handleClient();
        delay(5);
    }
}