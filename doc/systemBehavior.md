# System Behavior & Execution Lifecycle

This document defines the operational states of the **M5Paper**, focusing on the transitions between **Deep Sleep (Power Off)** and the **Active Session (Power On)**.

## 1. Interaction & Power State Diagram

The following diagram illustrates how the system branches between configuration and runtime based on user input and memory state.

```mermaid
flowchart TD
  T["System Execution Lifecycle"] --> P["Power On\nTriggered by Timer\nor First Touch"]

  %% Initial NVS Check
  P --> N{NVS Config Valid?}
  N -- No --> C1
  N -- Yes --> L[Load Config from NVS]

  %% Configuration Mode (initial provisioning)
  subgraph CONFIG["Configuration Mode"]
    direction TB
    C1[Start Web Portal] --> C2[Wait for User Submit] --> C3[Save to NVS]
  end

  %% Main Application Loop (handles refresh after submit)
  C3 --> RT1
  L --> RT1

  %% Runtime Mode
  subgraph RUNTIME["Runtime Mode"]
    direction TB
    RT1[Connect to WiFi and Fetch ICS Data] --> RT2[Convert UTC to Local Calgary] --> RT3[Draw Assignment Dashboard]
  end

  %% Active Session
  RT3 --> AS0

  subgraph ACTIVE["Active Session Powered ON"]
    direction TB
    AS0[Enable Touch Interrupts] --> AS1[Start 5 Minute Safety Timer] --> TQ{Timer > 0?}

    %% Interaction loop
    TQ -- Yes --> TAP{Screen Tapped?}
    TAP -- No --> WAIT[Wait for next tap] --> TQ
    TAP -- Yes --> GEAR{Tapped Gear?}
    GEAR -- No --> NAV[Update E Ink Page] --> RESET[Reset 5 Minute Timer] --> TQ
    GEAR -- Yes --> CFG2
  end

  %% Re-enter Configuration Mode via Gear
  subgraph CONFIG2["Configuration Mode via Gear"]
    direction TB
    CFG2[Start Web Portal] --> CFG3[Wait for User Submit] --> CFG4[Save to NVS]
  end

  %% End of Active Session / Loop condition
  TQ -- No --> CHK{Config Changed?}
  CFG4 --> CHK

  %% Repeat only if config changed
  CHK -- Yes --> RT1

  %% Deep sleep path
  CHK -- No --> WU[Configure Touch Wake up Pin] --> DS[Enter Deep Sleep] --> END((Stop))