#include "storage/config/NvsConfigProvider.h"
#include "AppConfig.h"
#include "domain/Course.h"
#include "util/Log.h"

bool NvsConfigProvider::hasConfig() const
{
    Preferences prefs;
    if (!prefs.begin(ns_, true))
    {
        CFG_LOG("hasConfig: prefs.begin failed");
        return false;
    }

    uint32_t magic = prefs.getUInt(K_MAGIC, 0);
    prefs.end();

    CFG_LOG("hasConfig: magic=0x%08X (expected 0x%08X)", magic, MAGIC_V1);
    return magic == MAGIC_V1;
}

bool NvsConfigProvider::load(AppConfig& out)
{
    CFG_LOG("Attempting to load config from NVS");

    Preferences prefs;
    if (!prefs.begin(ns_, true))
        return false;

    uint32_t magic = prefs.getUInt(K_MAGIC, 0);
    if (magic != MAGIC_V1)
    {
        prefs.end();
        return false;
    }

    out.wifiSsid = prefs.getString(K_SSID, "");
    out.wifiPass = prefs.getString(K_PASS, "");
    out.timezoneOffsetSec = prefs.getLong(K_TZ, 0);

    int count = prefs.getInt(K_COURSE_COUNT, 0);
    if (out.wifiSsid.length() == 0 || out.wifiPass.length() == 0 || count <= 0)
    {
        prefs.end();
        return false;
    }

    out.courses.clear();
    out.courses.reserve(count);

    CFG_LOG("Loaded SSID: %s", out.wifiSsid.c_str());
    CFG_LOG("Timezone offset: %ld", out.timezoneOffsetSec);
    CFG_LOG("Course count (stored): %d", count);

    for (int i = 0; i < count; i++)
    {
        String url = prefs.getString(courseUrlKey_(i).c_str(), "");
        if (url.length() == 0)
            continue;

        String name = prefs.getString(courseNameKey_(i).c_str(), "");
        Course c;
        c.name = (name.length() ? name : String("Course ") + String(i + 1));
        c.url = url;

        out.courses.push_back(c);
    }

    CFG_LOG("Final course count loaded: %d", (int)out.courses.size());

    prefs.end();

    bool hasCourses = !out.courses.empty();
    if (!hasCourses)
        CFG_LOG("NVS load FAILED — invalid or incomplete data");

    return hasCourses;
}

bool NvsConfigProvider::save(const AppConfig& cfg)
{
    CFG_LOG("Saving config to NVS...");
    CFG_LOG("SSID: %s", cfg.wifiSsid.c_str());
    CFG_LOG("Timezone offset: %ld", cfg.timezoneOffsetSec);
    CFG_LOG("Course count: %d", (int)cfg.courses.size());

    Preferences prefs;
    if (!prefs.begin(ns_, false))
        return false;

    // Save core fields
    prefs.putString(K_SSID, cfg.wifiSsid);
    prefs.putString(K_PASS, cfg.wifiPass);
    prefs.putLong(K_TZ, cfg.timezoneOffsetSec);

    int count = (int)cfg.courses.size();
    prefs.putInt(K_COURSE_COUNT, count);

    for (int i = 0; i < count; i++)
    {
        prefs.putString(courseNameKey_(i).c_str(), cfg.courses[i].name);
        prefs.putString(courseUrlKey_(i).c_str(), cfg.courses[i].url);
    }

    // Mark valid LAST
    prefs.putUInt(K_MAGIC, MAGIC_V1);

    CFG_LOG("NVS save SUCCESS (magic written)");

    prefs.end();
    return true;
}

bool NvsConfigProvider::clear()
{
    Preferences prefs;
    if (!prefs.begin(ns_, false))
        return false;
    bool ok = prefs.clear();
    prefs.end();

    CFG_LOG("Clearing NVS config");
    return ok;
}