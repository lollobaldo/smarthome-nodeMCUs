#pragma once

#include <Arduino.h>

namespace commands {
    typedef void (*callbackPointer_t) ();

    void setup(const char* clientName);
    void registerCommand(const char* command, callbackPointer_t);
}
