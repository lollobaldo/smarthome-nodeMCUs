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
    #define DebugPrint(...) (Serial.print(__VA_ARGS__))
    #define DebugPrintln(...) (Serial.println(__VA_ARGS__))
#else
    #define DebugPrint(a)
    #define DebugPrintln(a)
#endif

const float scale(float x, float in_min, float in_max, float out_min = 0, float out_max = 100);

const char* copyFromBytes(const byte* bytes, unsigned int length);

const char* concat(const char* s1, const char* s2);
const char* concat(const char* s1, const char* s2, const char* s3);


template<typename T, typename A>
T atWithOverflow (const std::vector<T, A>& v, int i) {
    int s = v.size();
    return v[i % s];
}

std::vector<std::string> split(const std::string& s);