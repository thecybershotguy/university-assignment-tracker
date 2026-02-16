#pragma once
#include "AppConfig.h"

class IConfigProvider
{
   public:
    virtual ~IConfigProvider() = default;
    virtual bool load(AppConfig& out) = 0;
};