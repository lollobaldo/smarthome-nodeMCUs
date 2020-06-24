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

    hardcoded = concat(s1, s2);
    TEST_ASSERT_EQUAL_STRING(expected, hardcoded);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_concat);

    UNITY_END();
}
