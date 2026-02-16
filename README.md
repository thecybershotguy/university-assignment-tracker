# University Assignment Tracker (M5Stack Paper)

A localized E-ink dashboard for tracking McMaster University assignments via Avenue to Learn (D2L) iCal feeds.

## Hardware
* **Device:** M5Stack Paper v1.1 (ESP32)
* **Display:** 4.7" E-ink (960x540)
* **Connectivity:** WiFi (2.4GHz)

## Features
* **Deep Sleep:** Updates every 2 hours to conserve battery.
* **Timezone Aware:** Converts UTC/Eastern deadlines to **Mountain Standard Time (MST)**.
* **Dynamic UI:** Groups assignments by "Yesterday", "Today", and "Tomorrow".
* **Battery Monitor:** Displays real-time voltage percentage.

## Setup
1.  Clone this repository.
2.  Create a file `src/secrets.h` based on the example below:
    ```cpp
    #define WIFI_SSID "YOUR_WIFI_NAME";
    #define WIFI_PASS = "YOUR_WIFI_PASS";
    ```
3.  Add your course `.ics` URLs to the `myCourses` struct in `main.cpp`.
4.  Build and upload using PlatformIO.

## Roadmap
* [ ] Modularize parsing logic
* [ ] Add touch interaction for scrolling
* [ ] External config file for courses