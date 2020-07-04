#include <colors.h>
#include <modes.h>
#include <utils.h>

#include <unity.h>

#include <iostream>
#include <string>
using namespace std;


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
    Color c(0xFF7700);
    TEST_ASSERT_EQUAL_UINT16(255, c.red);
    TEST_ASSERT_EQUAL_UINT16(119, c.green);
    TEST_ASSERT_EQUAL_UINT16(0, c.blue);
}

void test_modes__solid_color() {
    ProgramMode* pm = new SolidColor(colors::ORANGE);
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
}


int main() {
    UNITY_BEGIN();

    RUN_TEST(test_concat);

    RUN_TEST(test_color__string_constructor);
    RUN_TEST(test_color__char_constructor);
    RUN_TEST(test_color__long_constructor);

    RUN_TEST(test_modes__solid_color);


    UNITY_END();
}
