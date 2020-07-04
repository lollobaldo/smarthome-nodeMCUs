#include <colors.h>
#include <modes.h>
#include <utils.h>

#include <unity.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
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

void test_color__equality_constructor() {
    Color c1(0xFF7700);
    Color c2(0xFF0000);
    Color c3(0xFF7700);
    TEST_ASSERT_TRUE(c1 == c3);
    TEST_ASSERT_FALSE(c1 == c2);
}

void test_color__inequality_constructor() {
    Color c1(0xFF7700);
    Color c2(0xFF0000);
    Color c3(0xFF7700);
    TEST_ASSERT_TRUE(c1 != c2);
    TEST_ASSERT_FALSE(c1 != c3);
}

void test_color__gamma_correction() {
    colors::channels g = colors::gamma(Color(0xFF7700));
    TEST_ASSERT_EQUAL_UINT16(255, g.red);
    TEST_ASSERT_EQUAL_UINT16(30, g.green);
    TEST_ASSERT_EQUAL_UINT16(0, g.blue);
}


void test_modes__solid_color() {
    ProgramMode* pm = new SolidColor(colors::ORANGE);
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
}

void test_modes__blink_color__constructor1() {
    ProgramMode* pm = new BlinkColor(colors::ORANGE);
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
    this_thread::sleep_for(chrono::milliseconds(500));
    TEST_ASSERT_TRUE(pm->nextColor() == colors::BLACK);
    this_thread::sleep_for(chrono::milliseconds(500));
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
}

void test_modes__blink_color__constructor2() {
    ProgramMode* pm = new BlinkColor(colors::ORANGE, 100);
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
    this_thread::sleep_for(chrono::milliseconds(100));
    TEST_ASSERT_TRUE(pm->nextColor() == colors::BLACK);
    this_thread::sleep_for(chrono::milliseconds(100));
    TEST_ASSERT_TRUE(pm->nextColor() == colors::ORANGE);
}


int main() {
    UNITY_BEGIN();

    RUN_TEST(test_concat);

    RUN_TEST(test_color__string_constructor);
    RUN_TEST(test_color__char_constructor);
    RUN_TEST(test_color__long_constructor);
    RUN_TEST(test_color__equality_constructor);
    RUN_TEST(test_color__inequality_constructor);
    RUN_TEST(test_color__gamma_correction);

    RUN_TEST(test_modes__solid_color);
    RUN_TEST(test_modes__blink_color__constructor1);
    RUN_TEST(test_modes__blink_color__constructor2);

    UNITY_END();
}
