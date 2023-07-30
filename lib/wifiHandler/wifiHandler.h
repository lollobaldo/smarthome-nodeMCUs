#pragma once

#include <Arduino.h>
#include <IPAddress.h>

namespace wifi {
    IPAddress ip();
    String mac();
    void setup(const char* hostname);
}
