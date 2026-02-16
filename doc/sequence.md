# System Sequence: Operational Modes

This document details the communication flow between the User, the internal M5Paper hardware components, and external cloud services. It outlines how the system transitions from its initial unconfigured state to a fully automated tracking dashboard.

## 1. Interaction Sequence Diagram

The following diagram visualizes the three primary modes of operation: Initial Setup, Automated Sync, and the Interactive Session.

```mermaid
sequenceDiagram
    actor User
    participant Web as Web Portal
    participant Store as Memory (NVS)
    participant Main as Main Controller
    participant Net as Connectivity
    participant Cloud as External Cloud
    participant Screen as Display

    rect rgba(227, 242, 253, 0.6)
    Note over User,Screen: Mode 1: Initial Setup (Cold Boot)

    User->>Main: Power On (First Time)
    Main->>Store: Check for WiFi/URLs
    Store-->>Main: None found
    Main->>Web: Start Captive Portal
    Web-->>User: Serve Setup Page
    User->>Web: Submit Credentials & iCal URLs
    Web->>Store: Save to NVS Flash
    Store-->>Web: Success
    Web->>Main: Signal Configuration Complete
    end

    rect rgba(241, 248, 233, 0.6)
    Note over User,Screen: Mode 2: Runtime Sync (The Loop)

    loop Daily Operation
        Main->>Net: Connect to WiFi
        Net->>Cloud: Fetch .ics Data
        Cloud-->>Net: ICS stream
        Net->>Main: Parsed Assignments
        Main->>Screen: Render Dashboard

        rect rgba(241, 248, 233, 0.35)
        Note over User,Screen: Mode 3: Active Interaction Window

        Main->>Main: Start 5-minute timer

        loop While Timer > 0
            User->>Main: Tap Screen (Arrow or Gear)

            alt Tapped Navigation
                Main->>Screen: Render Next/Prev Page
                Main->>Main: Reset 5-minute timer
            else Tapped Gear (Re-Config)
                Main->>Web: Open Web Portal
                User->>Web: Update Settings
                Web->>Store: Update NVS
                Web->>Main: Signal New Config
            end
        end
        end

        Main->>Main: Configure Touch Wake-up (GPIO 36)
        Main->>Main: Enter Deep Sleep (2 hours)
        Note over Main: Sleep period...
        User->>Main: OR Manual Touch Wake-up
    end
    end