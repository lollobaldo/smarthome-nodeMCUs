#pragma once

#include <string>
#include <vector>

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #define byte uint8_t
#endif

#if defined MYDEBUG && defined ARDUINO
    #include <Arduino.h>
    #define DebugPrint(a) (Serial.print(a))
    #define DebugPrintln(a) (Serial.println(a))
#elif defined UNIT_TEST
    #include <iostream>
    #define DebugPrint(a) (std::cout << (a))
    #define DebugPrintln(a) (std::cout << (a) << std::endl)
#else
    #define DebugPrint(a)
    #define DebugPrintln(a)
#endif

const char* concat(const char* s1, const char* s2);
const char* concat(const char* s1, const char* s2, const char* s3);


template<typename T, typename A>
T atWithOverflow (const std::vector<T, A>& v, int i) {
    int s = v.size();
    return v[i % s];
}

std::vector<std::string> split(const std::string& s);