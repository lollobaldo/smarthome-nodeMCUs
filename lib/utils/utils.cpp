#include <cstring>
#include <string>
#include<vector>
using namespace std;

#include "utils.h"

// IMPORTANT: need to allocate memory for the array
const char* concat(const char* s1, const char* s2) {
    string s = string(s1) + s2;
    char* cstr = new char[s.length()+1];
    strcpy(cstr, s.c_str());
    return cstr;
}
