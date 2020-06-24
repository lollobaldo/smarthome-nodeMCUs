#pragma once

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

extern WiFiClient wifiClient;

void wifiSetup(const char* hostname);
void wifiLoop();