#include <M5EPD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include "secerts.h"
#include "AppConfig.h"
#include "RunTimeSync.h"

// Add your specific course URLs here
Course myCourses[] = {
    {"Software Architecture", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=756156&token=ahng2k589kh86pee36c86"},
    {"Parallel Programming", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=760099&token=ahng2k589kh86pee36c86"},
    {"Lean Thinking", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=752846&token=ahng2k589kh86pee36c86"},
    {"Software Security", "https://avenue.cllmcmaster.ca/d2l/le/calendar/feed/user/feed.ics?feedOU=761367&token=ahng2k589kh86pee36c86"}
};

AppConfig appConfig = {
    WIFI_SSID, 
    WIFI_PASS, 
    -7 *3600,
    myCourses, 
    sizeof(myCourses) / sizeof(myCourses[0])
};

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
    M5.begin();
    Serial.begin(115200);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.createCanvas(540, 960);
    runRuntimeSync(appConfig, canvas);
    Serial.println("Updated. Sleeping...");
    delay(1000);
    M5.shutdown(7200); 
}

void loop() {}