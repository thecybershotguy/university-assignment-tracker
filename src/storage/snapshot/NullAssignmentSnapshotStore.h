// NullAssignmentSnapshotStore.h
#pragma once
#include "IAssignmentSnapshotStore.h"

// No-op store: preserves current behavior (no caching/offline fallback).
class NullAssignmentSnapshotStore : public IAssignmentSnapshotStore
{
   public:
    bool saveSnapshot(const String& /*payload*/, int64_t /*savedAtEpoch*/) override
    {
        return true;  // treat as success to avoid error paths later
    }

    bool loadSnapshot(String& /*outPayload*/, int64_t& /*outSavedAtEpoch*/) override
    {
        return false;  // no snapshot available
    }

    bool clearSnapshot() override
    {
        return true;
    }
};