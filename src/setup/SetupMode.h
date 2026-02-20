#pragma once
#include <M5EPD.h>
#include "storage/config/NvsConfigProvider.h"
#include "setup/SetupEntryReason.h"

class SetupMode
{
   public:
    SetupMode(M5EPD_Canvas& canvas, NvsConfigProvider& nvs);
    void run(SetupEntryReason reason);  // blocking until reboot

   private:
    M5EPD_Canvas& canvas_;
    NvsConfigProvider& nvs_;

    String apSsid_;
    IPAddress apIp_;

    void startAp_();
    static String chipSuffix_();
};
