// IAssignmentSnapshotStore.h
#pragma once
#include <Arduino.h>
#include <stdint.h>

// Stores/loads a serialized snapshot of the dashboard data.
// - "payload" format is intentionally opaque to could be JSON/CSV/etc).
// - "savedAtEpoch" tracks when the snapshot was created (optional but useful later).
class IAssignmentSnapshotStore
{
   public:
    virtual ~IAssignmentSnapshotStore() = default;
    virtual bool saveSnapshot(const String& payload, int64_t savedAtEpoch) = 0;
    virtual bool loadSnapshot(String& outPayload, int64_t& outSavedAtEpoch) = 0;
    virtual bool clearSnapshot() = 0;
};