#include "FsAssignmentSnapshotStore.h"

bool FsAssignmentSnapshotStore::saveSnapshot(const String& /*payload*/, int64_t /*savedAtEpoch*/)
{
    // Not implemented yet (no behavior change).
    // Implement later with LittleFS/SPIFFS.
    return false;
}

bool FsAssignmentSnapshotStore::loadSnapshot(String& /*outPayload*/, int64_t& /*outSavedAtEpoch*/)
{
    // Not implemented yet (no behavior change).
    return false;
}

bool FsAssignmentSnapshotStore::clearSnapshot()
{
    // Not implemented yet.
    return false;
}