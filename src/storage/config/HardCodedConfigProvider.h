#pragma once
#include "IConfigProvider.h"

class HardCodedConfigProvider : public IConfigProvider
{
   public:
    bool load(AppConfig& out) override;
};