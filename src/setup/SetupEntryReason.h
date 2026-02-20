#pragma once
#include <stdint.h>

enum class SetupEntryReason : uint8_t
{
    NoConfig = 0,
    WifiFailed,
    NtpFailed,
    UserRequested
};
