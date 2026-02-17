#pragma once
#include "storage/config/IConfigProvider.h"
#include "storage/config/NvsConfigProvider.h"
#include "storage/config/HardCodedConfigProvider.h"
#include "util/Log.h"

class CompositeConfigProvider : public IConfigProvider
{
   public:
    CompositeConfigProvider() : nvs_(), hard_()
    {
    }

    bool load(AppConfig& out) override
    {
        CFG_LOG("Composite: checking NVS...");

        if (nvs_.hasConfig() && nvs_.load(out))
        {
            CFG_LOG("Composite: USING NVS config");
            return true;
        }

        CFG_LOG("Composite: falling back to HARDCODED config");
        return hard_.load(out);
    }

    bool seedFromHardcodedIfEmpty()
    {
        if (nvs_.hasConfig())
        {
            CFG_LOG("Seed skipped: NVS already contains config");
            return true;
        }

        CFG_LOG("Seeding NVS from hardcoded config...");

        AppConfig cfg;
        if (!hard_.load(cfg))
        {
            CFG_LOG("Hardcoded load FAILED during seed");
            return false;
        }

        bool ok = nvs_.save(cfg);
        CFG_LOG("Seed result: %s", ok ? "SUCCESS" : "FAILED");
        return ok;
    }

    NvsConfigProvider& nvs()
    {
        return nvs_;
    }

   private:
    NvsConfigProvider nvs_;
    HardCodedConfigProvider hard_;
};