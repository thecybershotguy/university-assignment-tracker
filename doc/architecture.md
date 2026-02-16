# System Architecture

This document outlines the high-level design and component responsibilities of the University Assignment Tracker.

## 1. High-Level Architecture Diagram

The following diagram visualizes the interaction between the user, external web services, and the internal hardware modules of the M5Paper.

```plantuml
@startuml
!theme vibrant
skinparam componentStyle rectangle
' Forces lines to use 90-degree angles for a "blueprint" look
skinparam linetype ortho 

title University Assignment Tracker High-level Architecture

package "User" as UserDevice {
  [Laptop / Phone]
} 

package "Web" as Cloud {
  [iCal Providers] 
  [NTP] as NTP
}

' Grouping internal hardware to stay together
package "M5Paper Hardware" {
  component "Connectivity" as Net
  component "Memory" as Store
  component "Management" as Main
  component "Display" as Screen
  
  ' Use notes attached to the bottom
  note bottom of Net: Web Server for Setup\n& iCal Data Fetching
  note bottom of Store: Persistent Storage (NVS)\nfor WiFi & URLs
  note bottom of Main: Handles Boot Logic\n& Power (Deep Sleep)
  note bottom of Screen: E-Ink Output\n(Assignments & UI)
}

' --- Strategic Line Routing ---

' 1. Move User and Cloud above the hardware
UserDevice -down-> Net : " Setup "
Cloud -down-> Net :  "  Data  " 

' 2. Keep Config horizontal between Net and Store
Net <--> Store :  " Config "

' 3. Put Management and Display on their own flow
Net -[hidden]right- Store
Store -[hidden]right- Main

' 4. Main to Screen flow
Main -down-> Screen : " Render "

@enduml