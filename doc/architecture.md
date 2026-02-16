# System Architecture

This document outlines the high-level design and component responsibilities of the University Assignment Tracker.

## 1. High-Level Architecture Diagram

The following diagram visualizes the interaction between the user, external web services, and the internal hardware modules of the M5Paper.

```mermaid
flowchart TB
  %% Title node (Mermaid doesn't have a true title in GitHub Markdown)
  TITLE["University Assignment Tracker<br/>High-level Architecture"]

  TITLE --> U
  TITLE --> W
  TITLE --> H

  %% User
  subgraph U["User"]
    direction TB
    USERDEV[Laptop / Phone]
  end

  %% Web / Cloud
  subgraph W["Web"]
    direction TB
    ICAL[iCal Providers]
    NTP[NTP]
  end

  %% M5Paper Hardware
  subgraph H["M5Paper Hardware"]
    direction TB
    Net[Connectivity]
    Store[Memory]
    Main[Management]
    Screen[Display]
  end

  %% External interactions
  USERDEV -->|Setup| Net
  ICAL -->|Data| Net
  NTP -->|Time Sync| Net

  %% Internal interactions
  Net <--> |Config| Store
  Store --> Main
  Main -->|Render| Screen

  %% Responsibility "notes" (modeled as callouts)
  NetNote["Web Server for Setup<br/>& iCal Data Fetching"]
  StoreNote["Persistent Storage (NVS)<br/>for WiFi & URLs"]
  MainNote["Handles Boot Logic<br/>& Power (Deep Sleep)"]
  ScreenNote["E-Ink Output<br/>(Assignments & UI)"]

  Net -.-> NetNote
  Store -.-> StoreNote
  Main -.-> MainNote
  Screen -.-> ScreenNote