#include <cstring>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
// using namespace std;

#include "utils.h"

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
