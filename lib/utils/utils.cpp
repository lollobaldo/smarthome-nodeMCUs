#include <cstring>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
// using namespace std;

#include "utils.h"

const float scale(float x, float in_min, float in_max, float out_min /*-0*/, float out_max /*=100*/) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

const char* copyFromBytes(const byte* bytes, unsigned int length) {
    char* message = new char[length + 1];
    memcpy(message, bytes, length);
    message[length] = '\0';
    return message;
}

// IMPORTANT: need to allocate memory for the array
const char* concat(const char* s1, const char* s2) {
    std::string s = std::string(s1) + s2;
    char* cstr = new char[s.length()+1];
    strcpy(cstr, s.c_str());
    return cstr;
}

const char* concat(const char* s1, const char* s2, const char* s3) {
    std::string s = std::string(s1) + s2 + s3;
    char* cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    return cstr;
}

std::vector<std::string> split(const std::string& s) {
    std::istringstream iss(s);
    return {
        std::istream_iterator<std::string>{iss},
        std::istream_iterator<std::string>{}};
}
