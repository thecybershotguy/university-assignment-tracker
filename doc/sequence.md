# System Sequence: Operational Modes

This document details the communication flow between the User, the internal M5Paper hardware components, and external cloud services. It outlines how the system transitions from its initial unconfigured state to a fully automated tracking dashboard.

## 1. Interaction Sequence Diagram

The following diagram visualizes the three primary modes of operation: Initial Setup, Automated Sync, and the Interactive Session.

```plantuml
@startuml
!theme toy
autonumber
skinparam backgroundColor DDDDDD
skinparam shadowing false
skinparam RoundCorner 7

actor "User" as User
participant "Web Portal" as Web
participant "Memory (NVS)" as Store
participant "Main Controller" as Main
participant "Connectivity" as Net
participant "External Cloud" as Cloud
participant "Display" as Screen

== Mode 1: Initial Setup (Cold Boot) ==

User -> Main: Power On (First Time)
Main -> Store: Check for WiFi/URLs
Store --> Main: [None Found]
Main -> Web: Start Captive Portal
Web -> User: Serve Setup Page
User -> Web: Submit Credentials & iCal URLs
Web -> Store: Save to NVS Flash
Store --> Web: Success
Web -> Main: Signal Configuration Complete

== Mode 2: Runtime Sync (The Loop) ==

loop Daily Operation
    Main -> Net: Connect to WiFi
    Net -> Cloud: Fetch .ics Data
    Cloud --> Net: ICS Stream
    Net -> Main: Parsed Assignments
    Main -> Screen: Render Dashboard

    == Mode 3: Active Interaction Window ==
    
    Main -> Main: Start 5-Minute Timer
    
    loop While Timer > 0
        User -> Main: Tap Screen (Arrow or Gear)
        alt Tapped Navigation
            Main -> Screen: Render Next/Prev Page
            Main -> Main: Reset 5-Minute Timer
        else Tapped Gear (Re-Config)
            Main -> Web: Open Web Portal
            User -> Web: Update Settings
            Web -> Store: Update NVS
            Web -> Main: Signal New Config
        end
    end
    
    Main -> Main: Configure Touch Wake-up (GPIO 36)
    Main -> Main: Enter Deep Sleep (2 Hours)
    ... Sleep Period ...
    User -> Main: [OR] Manual Touch Wake-up
end
@enduml

## 2. Mode Descriptions

### 2.1 Mode 1: Initial Setup (The Captive Portal)

When the device is first powered on in Calgary, the Main Controller checks whether connection data exists in **Non-Volatile Storage (NVS)**.

- **Provisioning:** If no data is found, the Connectivity module launches a **captive portal**.
- **Local Handshake:** The user connects to the M5Paper’s local hotspot to provide:
  - WiFi credentials
  - Course iCal (`.ics`) URLs
- **Persistence:** Once submitted, these "secrets" are committed to **NVS Flash**, ensuring they survive:
  - power loss
  - deep sleep cycles

---

### 2.2 Mode 2: Runtime Sync (The Daily Loop)

This is the automated **heartbeat** of the system.

- **Efficiency:** The device wakes up every **2 hours** using the internal **RTC timer**.
- **Data Fetch:** It briefly enables WiFi to pull the latest assignment data from the cloud.
- **Visualization:** After parsing the `.ics` data and converting times to **local Calgary time**, the Display module refreshes the **E-Ink** screen with the updated list.

---

### 2.3 Mode 3: Active Interaction Window

To balance responsiveness with battery life, the device enters a high-power **Active Session** immediately after a screen refresh.

- **Safety Timer:** The touch digitizer is powered for **5 minutes**.
- **Keep-Alive:** Tapping navigation arrows (to flip through pages) resets the 5-minute timer, allowing uninterrupted access to multiple pages.
- **Re-Config:** Tapping the **Gear Icon** lets the user re-enter **Mode 1** to update:
  - iCal URLs
  - WiFi settings  
  without needing to plug the device into a computer.