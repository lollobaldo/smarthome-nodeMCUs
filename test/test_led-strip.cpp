#include <colors.h>
#include <utils.h>

#include <unity.h>

#include <iostream>
#include <string>
using namespace std;

const char* hardcoded;

void test_concat() {
    const char* s1 = "hello";
    const char* s2 = " world";
    const char* expected = "hello world";

    const char* actual1 = concat(s1, s2);
    TEST_ASSERT_EQUAL_STRING(expected, actual1);
    const char* actual2 = concat("hello", s2);
    TEST_ASSERT_EQUAL_STRING(expected, actual2);
    const char* actual3 = concat(s1, " world");
    TEST_ASSERT_EQUAL_STRING(expected, actual3);
}

void test_color__string_constructor() {
    Color c(string("ff7700"));
    TEST_ASSERT_EQUAL_UINT16(255, c.red);
    TEST_ASSERT_EQUAL_UINT16(119, c.green);
    TEST_ASSERT_EQUAL_UINT16(0, c.blue);
}

void test_color__char_constructor() {
    Color c("ff7700");
    TEST_ASSERT_EQUAL_UINT16(255, c.red);
    TEST_ASSERT_EQUAL_UINT16(119, c.green);
    TEST_ASSERT_EQUAL_UINT16(0, c.blue);
}

void test_color__long_constructor() {
    Color c(strtol("ff7700", NULL, 16));
    TEST_ASSERT_EQUAL_UINT16(255, c.red);
    TEST_ASSERT_EQUAL_UINT16(119, c.green);
    TEST_ASSERT_EQUAL_UINT16(0, c.blue);
}


int main() {
    UNITY_BEGIN();

    RUN_TEST(test_concat);

    RUN_TEST(test_color__string_constructor);
    RUN_TEST(test_color__char_constructor);
    RUN_TEST(test_color__long_constructor);

    uint8_t* payload = (uint8_t*) "ciaone";

    switch(*payload++) {
    case '#':
        DebugPrintln("1");
        break;
    case 'c':
        DebugPrintln("2");
        break;
    default:
        DebugPrintln("0");
        break;
    }
    DebugPrint(payload);


    UNITY_END();
}
