#pragma once
#include <Preferences.h>
#include "storage/config/IConfigProvider.h"

class NvsConfigProvider : public IConfigProvider
{
   public:
    explicit NvsConfigProvider(const char* nvsNamespace = "appcfg") : ns_(nvsNamespace)
    {
    }

    bool load(AppConfig& out) override;
    bool NewFunction(AppConfig& out);
    bool save(const AppConfig& cfg);
    bool hasConfig() const;
    bool clear();

   private:
    const char* ns_;

    static constexpr const char* K_MAGIC = "magic";
    static constexpr uint32_t MAGIC_V1 = 0xC0FFEE01;

    static constexpr const char* K_SSID = "ssid";
    static constexpr const char* K_PASS = "pass";
    static constexpr const char* K_TZ = "tzOff";
    static constexpr const char* K_COURSE_COUNT = "cCount";

    static String courseNameKey_(int i)
    {
        return "c" + String(i) + "n";
    }
    static String courseUrlKey_(int i)
    {
        return "c" + String(i) + "u";
    }
};