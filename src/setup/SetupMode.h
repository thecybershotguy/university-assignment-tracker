#pragma once
#include <M5EPD.h>
#include "setup/SetupWebPortal.h"
#include "storage/config/NvsConfigProvider.h"
#include "setup/SetupEntryReason.h"

class SetupMode
{
   public:
    SetupMode(M5EPD_Canvas& canvas, NvsConfigProvider& nvs);
    void begin(SetupEntryReason reason);
    void tick();

   private:
    M5EPD_Canvas& canvas_;
    NvsConfigProvider& nvs_;
    SetupWebPortal portal_;

    String apSsid_;
    IPAddress apIp_;
    bool started_ = false;

    void startAp_();
    static String chipSuffix_();
};
