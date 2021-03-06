#pragma once

#include <Arduino.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

namespace wifi {
    void setup(const char* hostname);
    void loop();
}
