#  University Assignment Tracker
## Project Tracker (V1 Scope)

This document tracks completed features, pending items, and release readiness.

Architecture reference:
- architecture.md
- sequence.md
- systemBehavior.md

---

# ✅ COMPLETED

## Core Refactor
- [x] Step 1: Introduced Course model + AppConfig
- [x] Step 2: Extracted RuntimeSync module
- [x] Step 3: Separated AssignmentService (data) + DashboardRenderer (UI)
- [x] Step 4: Sorting + date window filtering (yesterday onward)
- [x] Step 5: IConfigProvider + HardCodedConfigProvider
- [x] Step 6: NetworkManager (WiFi + NTP extraction)
- [x] Step 7: Graceful failure handling (offline / empty-state)

## Storage & Configuration
- [x] Step 9: NVS-backed config provider
- [x] CompositeConfigProvider
- [x] Dev seeding (APP_DEV_MODE)
- [x] Smoke tests for NVS load/save/clear

## Setup Portal
- [x] Captive AP mode
- [x] Setup web page
- [x] Save config → NVS → reboot
- [x] Setup screen UI polished (SSID/IP centered)
- [x] WiFi scan support

## Runtime Dashboard
- [x] ONLINE / OFFLINE header
- [x] Battery %
- [x] Group by date (Yesterday / Today / Tomorrow)
- [x] Highlight TODAY
- [x] Deep sleep cycle

## V1 Upgrade Decisions
- [x] Touch-based gear hitbox INCLUDED in V1 scope
- [x] Offline cache fallback REMOVED (product decision)

---

# 🔄 PENDING (V1 REQUIRED)

## Networking Robustness
- [x] 6.1 WiFi connection timeout
- [x] Disable WiFi radio after timeout

## Failure UX
- [ ] If WiFi fails → enter Setup Mode automatically
- [ ] Setup screen shows "WiFi connection failed"
- [ ] Setup portal preloads existing config (SSID + timezone + iCal rows)

## Setup Portal Improvements
- [ ] Dynamic HTML (replace static PROGMEM page)
- [ ] Prefill existing config (SSID + timezone + iCal rows)
- [ ] Allow blank password to retain existing value

## Power Mode Policy

### USB Mode (Powered)
- [ ] Detect USB power
- [ ] Stay awake
- [ ] Refresh every 2 hours without shutdown
- [ ] Display "Last Updated: HH:MM"

### Battery Mode
- [ ] Render dashboard
- [ ] Show "Last Updated: HH:MM"
- [ ] Shutdown / deep sleep for 2 hours

## Gear / Settings (V1 Feature)
- [ ] Add Gear button to dashboard top bar
- [ ] Touch detection hitbox
- [ ] Enter SetupPortal from runtime session
- [ ] Preload existing config when entering via Gear
- [ ] Reset active session timer after reconfiguration

## Production Build Sanity
- [ ] APP_DEV_MODE = 0 disables seeding
- [ ] Cold boot with no config → Setup Mode only
- [ ] Cold boot with config → Runtime Mode

---

# 🧪 TESTING CHECKLIST (V1)

## First Boot
- [ ] No NVS config → Setup Mode launches
- [ ] Save config → reboot → runtime

## Runtime
- [ ] Connect WiFi successfully
- [ ] Fetch ICS
- [ ] Render assignments
- [ ] Deep sleep triggers after session

## WiFi Failure
- [ ] Wrong password → timeout
- [ ] Setup Mode auto-launch
- [ ] Error banner visible

## Gear Reconfiguration
- [ ] Tap Gear → Setup Mode
- [ ] Existing iCals preloaded
- [ ] Save → reboot → runtime updated

---

# 📦 POST-V1 (Future Enhancements)

- [ ] Timezone dropdown (IANA tz + DST)
- [ ] Dynamic JS add/remove course rows
- [ ] Modular logging framework per component
- [ ] Power profiling + battery estimation
- [ ] Raspberry Pi / generic ESP32 port (POSIX-friendly abstraction)
- [ ] Documentation + screenshots for public release
- [ ] Reddit / LinkedIn post preparation

---

# 🎯 V1 Release Criteria

V1 is complete when:
- Device self-recovers from WiFi errors
- User can always re-enter setup
- No infinite WiFi hang
- No reflashing required for reconfiguration
- Stable 24h runtime testing

