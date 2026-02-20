#include "setup/SetupWebPortal.h"
#include "setup/SetupPages.h"
#include "ui/SetupScreenRenderer.h"
#include <ArduinoJson.h>
#include "SetupWebPortal.h"

SetupWebPortal::SetupWebPortal(NvsConfigProvider& nvs) : nvs_(nvs)
{
}

String SetupWebPortal::htmlEscape_(const String& s)
{
    String o = s;
    o.replace("&", "&amp;");
    o.replace("<", "&lt;");
    o.replace(">", "&gt;");
    o.replace("\"", "&quot;");
    o.replace("'", "&#39;");
    return o;
}

void SetupWebPortal::registerRoutes_()
{
    server_.on("/", HTTP_GET, [this]() { handleIndex_(); });
    server_.on("/save", HTTP_POST, [this]() { handleSave_(); });
    server_.on("/clear", HTTP_POST, [this]() { handleClear_(); });

    server_.onNotFound([this]() { server_.send(404, "text/plain", "Not Found"); });
}

String SetupWebPortal::renderIndexHtml_()
{
    AppConfig cfg;
    bool has = nvs_.hasConfig() && nvs_.load(cfg);

    const AppConfig* preload = has ? &cfg : nullptr;

    String page = FPSTR(SETUP_INDEX_HTML);

    page.replace("{{SSID}}", preload ? htmlEscape_(preload->wifiSsid.c_str()) : "");
    page.replace("{{TZ}}", preload ? String(preload->timezoneOffsetSec) : String(-25200));
    page.replace("{{COURSE_ROWS}}", buildCourseRowsHtml_(preload));

    return page;
}

String SetupWebPortal::buildCourseRowsHtml_(const AppConfig* preload)
{
    String out;

    for (int i = 0; i < MAX_COURSES; i++)
    {
        String name = "";
        String url = "";

        if (preload && i < (int)preload->courses.size())
        {
            name = preload->courses[i].name;
            url = preload->courses[i].url;  // <- match your Course field name
        }

        out += "<div class='row'>";
        out += "<div><label>Course " + String(i + 1) +
               " Name</label>"
               "<input name='c" +
               String(i) + "n' value='" + htmlEscape_(name) + "'></div>";
        out += "<div><label>Course " + String(i + 1) +
               " URL</label>"
               "<input name='c" +
               String(i) + "u' value='" + htmlEscape_(url) + "'></div>";
        out += "</div>";
    }

    return out;
}

void SetupWebPortal::handleIndex_()
{
    String page = renderIndexHtml_();
    server_.send(200, "text/html", page);
}

void SetupWebPortal::handleSave_()
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

void SetupWebPortal::handleClear_()
{
    Serial.println("[SETUP] Clearing NVS config...");
    bool ok = nvs_.clear();

    server_.send(ok ? 200 : 500, "text/plain", ok ? "Cleared. Rebooting..." : "Clear failed.");
    delay(400);
    ESP.restart();
}

void SetupWebPortal::begin()
{
    registerRoutes_();  // Attach HTTP handlers
    server_.begin();    // Start web server
}

void SetupWebPortal::loop()
{
    server_.handleClient();
}
