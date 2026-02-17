# NVS Config Provider Smoke Tests

## Purpose
Verify NVS-backed AppConfig is correct, persistent, and fails safely.

## Preconditions
- CompositeConfigProvider is wired in main.cpp
- AppConfig owns strings + course list (String + vector<Course>)
- NetworkManager uses cfg.wifiSsid.c_str() / cfg.wifiPass.c_str()
- AssignmentService iterates cfg.courses.size()

---

## Test 1 — First Boot Seeds NVS
- [ ] Ensure NVS is empty (clear once or erase flash)
- [ ] Enable `seedFromHardcodedIfEmpty()`
- [ ] Flash firmware
- [ ] Serial: confirms seeding occurred
- [ ] WiFi connects
- [ ] Assignments load + dashboard renders

Expected: device works, config written to NVS.

---

## Test 2 — Reboot Loads From NVS (No Seeding)
- [ ] Disable `seedFromHardcodedIfEmpty()`
- [ ] Reboot / power-cycle
- [ ] Serial: indicates config loaded from NVS
- [ ] WiFi connects
- [ ] Courses count correct
- [ ] Assignments load

Expected: no dependence on hardcoded config.

---

## Test 3 — Persistence Across Multiple Power Cycles
- [ ] Seeding disabled
- [ ] Power-cycle 3–5 times
- [ ] Each boot loads from NVS
- [ ] No random strings / corruption

Expected: stable persistent behavior.

---

## Test 4 — Clear NVS Forces Fallback
- [ ] Temporarily call `cfgProvider.nvs().clear()` once
- [ ] Flash/run once, then remove clear call
- [ ] Reboot
- [ ] Falls back to hardcoded (or re-seeds if seeding enabled)

Expected: clearing NVS resets persistent config.

---

## Test 5 — Partial/Corrupt Save Does NOT Brick System
Method A (safe simulation):
- [ ] Temporarily disable writing `magic` in NvsConfigProvider::save()
- [ ] Enable seeding and run once (creates partial config)
- [ ] Disable seeding and reboot
- [ ] System treats NVS invalid and falls back to hardcoded

Expected: magic-last strategy prevents invalid config from being used.

---

## Test 6 — Course List Correctness
- [ ] After load, print course count
- [ ] Print each course name + URL (URL can be truncated)
- [ ] Verify AssignmentService fetches across all courses

Expected: no missing courses, URLs valid.

---

## Test 7 — WiFi Actually Uses NVS (not secrets)
- [ ] Change `secerts.h` SSID temporarily to incorrect value
- [ ] Ensure seeding is disabled
- [ ] Reboot
- [ ] Device still connects (meaning NVS is used)

Expected: NVS config is the source of truth.

---

## Test 8 — Timezone Offset Persists
- [ ] Seed with a distinct offset (e.g., -6*3600)
- [ ] Reboot with seeding off
- [ ] Printed timezone offset matches
- [ ] Deadlines shift accordingly

Expected: timezoneOffsetSec stored and applied.

---

## Test 9 — Stability Regression Check
- [ ] Run multiple cycles / redraws
- [ ] No crashes / resets
- [ ] No obvious memory weirdness

Expected: stable runtime after refactor.

---

## Notes / Results
- Date tested:
- Firmware hash/commit:
- Pass/Fail summary:
- Observations: