#include <wifiHandler.h>

#include <Arduino.h>

#ifndef CLIENT_NAME
    #define CLIENT_NAME "ESP--anonymous"
#endif
const char* clientName = CLIENT_NAME;

void setup() {
    Serial.begin(115200);
    wifi::setup(clientName);
}

void loop() {
    wifi::loop();
}
