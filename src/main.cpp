#include <M5EPD.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
#include "AppConfig.h"
#include "RunTimeSync.h"
#include "storage/HardCodedConfigProvider.h"

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
    M5.begin();
    Serial.begin(115200);
    M5.EPD.SetRotation(90);
    M5.EPD.Clear(true);
    canvas.createCanvas(540, 960);

    HardCodedConfigProvider provider;
    AppConfig appConfig;
    provider.load(appConfig);

    runRuntimeSync(appConfig, canvas);
    Serial.println("Updated. Sleeping...");
    delay(1000);
    M5.shutdown(7200); 
}

void loop() {}