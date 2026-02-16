"""# System Behavior & Execution Lifecycle

This document defines the operational states of the **M5Paper**, focusing on the transitions between **Deep Sleep (Power Off)** and the **Active Session (Power On)**.

## 1. Interaction & Power State Diagram

The following diagram illustrates how the system branches between configuration and runtime based on user input and memory state.

```plantuml
@startuml
!theme toy
skinparam backgroundColor #FFFFFF
skinparam activity {
  BackgroundColor<<Config>> #E3F2FD
  BorderColor<<Config>> #1565C0
  BackgroundColor<<Runtime>> #F1F8E9
  BorderColor<<Runtime>> #2E7D32
}

title System Execution Lifecycle

start

:Power On;
note left: Triggered by Timer\\nor First Touch

' Initial NVS Check
if (**NVS Config Valid?**) then (No)
  partition "Configuration Mode" <<Config>> {
    :Start Web Portal;
    :Wait for User 'Submit';
    :Save to NVS;
  }
else (Yes)
  :Load Config from NVS;
endif

' Main Application Loop (Handles "Refresh after Submit")
repeat
  partition "Runtime Mode" <<Runtime>> {
    :Connect to WiFi & Fetch .ics Data;
    :Convert UTC to Local (Calgary);
    :Draw Assignment Dashboard;
  }

  partition "Active Session (Powered ON)" <<Runtime>> {
    :Enable Touch Interrupts;
    :Start 5-Minute Safety Timer;
    
    ' Interaction Loop
    while (**Timer > 0?**) is (Yes)
      if (Screen Tapped?) then (Yes)
        if (Tapped Gear?) then (Yes)
          partition "Configuration Mode" <<Config>> {
            :Start Web Portal;
            :Wait for User 'Submit';
            :Save to NVS;
            ' Re-triggering the loop to show new data immediately
            detach
          }
        else (Tapped Navigation)
          :Update E-Ink Page;
          :Reset 5-Minute Timer;
        endif
      endif
      :Wait for next tap;
    endwhile
  }
' The loop only repeats if a config change was made via the Gear icon
repeat while (Config Changed?) is (Yes)

:Configure Touch Wake-up Pin;
:Enter Deep Sleep;
stop
@enduml
