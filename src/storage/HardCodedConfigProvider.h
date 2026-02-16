#pragma once
#include "IConfigProivder.h"


class HardCodedConfigProvider : public IConfigProvider {

    public:
        bool load(AppConfig& out) override;

};