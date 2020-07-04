#pragma once

#if defined MYDEBUG && defined ARDUINO
    #include <Arduino.h>
    #define DebugPrint(a) (Serial.print(a))
    #define DebugPrintln(a) (Serial.println(a))
#elif defined UNIT_TEST
    #include <iostream>
    #define DebugPrint(a) (cout << (a))
    #define DebugPrintln(a) (cout << (a) << endl)
#else
    #define DebugPrint(a)
    #define DebugPrintln(a)
#endif

const char* concat(const char* s1, const char* s2);