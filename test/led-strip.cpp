#include <colors.h>
#include <modes.h>
#include <utils.h>

#include <unity.h>

#include <chrono>
#include <iostream>
#include <string>
using namespace std;
using namespace std::chrono;
#define millis() (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())

#include <gtest/gtest.h>

TEST(Concat, ConcatWorks) {
    const char* s1 = "hello";
    const char* s2 = " world";
    const char* expected = "hello world";

    EXPECT_STREQ(expected, concat(s1, s2));
    EXPECT_STREQ(expected, concat("hello", s2));
    EXPECT_STREQ(expected, concat(s1, " world"));
}

TEST(Color, StringConstructor) {
    Color c(string("ff7700"));
    EXPECT_EQ(255, c.red);
    EXPECT_EQ(119, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, CharConstructor) {
    Color c("ff7700");
    EXPECT_EQ(255, c.red);
    EXPECT_EQ(119, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, LongConstructor) {
    Color c(0xFF7700);
    EXPECT_EQ(255, c.red);
    EXPECT_EQ(119, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, EqualityOperator) {
    Color c1(0xFF7700);
    Color c2(0xFF0000);
    Color c3(0xFF7700);
    EXPECT_TRUE(c1 == c3);
    EXPECT_FALSE(c1 == c2);
}

TEST(Color, InequalityOperator) {
    Color c1(0xFF7700);
    Color c2(0xFF0000);
    Color c3(0xFF7700);
    EXPECT_TRUE(c1 != c2);
    EXPECT_FALSE(c1 != c3);
}

TEST(Color, ToString) {
    Color c1(0xFF7700);
    EXPECT_STREQ("Color: rgb(255, 119, 0).", c1.toString().c_str());
}

TEST(Color, Stream) {
    Color c1(0xFF7700);
    EXPECT_STREQ(
        "Color: rgb(255, 119, 0).", c1.toString().c_str());
}

TEST(Color, Fade) {
    Color c = colors::fade(Color(0xFF7700), 0.3);
    EXPECT_EQ(76, c.red);
    EXPECT_EQ(35, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, GammaCorrection) {
    colors::channels g = colors::gamma(Color(0xFF7700));
    EXPECT_EQ(255, g.red);
    EXPECT_EQ(30, g.green);
    EXPECT_EQ(0, g.blue);
}



TEST(Modes, SolidColor) {
    ProgramMode* pm = new SolidColor(colors::ORANGE);
    EXPECT_EQ(colors::ORANGE, pm->nextColor(10));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(1000));
}

TEST(Modes, BlinkColor1) {
    ProgramMode* pm = new BlinkColor(colors::ORANGE);
    unsigned long millis = millis();
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::BLACK, pm->nextColor(millis + 1000));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 2000));
}

TEST(Modes, BlinkColor2) {
    ProgramMode* pm = new BlinkColor(colors::ORANGE, 100);
    unsigned long millis = millis();
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::BLACK, pm->nextColor(millis + 100));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 200));
}

TEST(Modes, FadeColor1) {
    ProgramMode* pm = new FadeColor(colors::ORANGE);
    unsigned long millis = millis();
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::BLACK, pm->nextColor(millis + 500));
    EXPECT_EQ(colors::fade(colors::ORANGE, 0.5), pm->nextColor(millis + 750));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 1000));
}

TEST(Modes, FadeColor2) {
    ProgramMode* pm = new FadeColor(colors::ORANGE, 100);
    unsigned long millis = millis();
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::BLACK, pm->nextColor(millis + 50));
    EXPECT_EQ(colors::fade(colors::ORANGE, 0.5), pm->nextColor(millis + 75));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 100));
}

TEST(Modes, BlinkRainbow1) {
    ProgramMode* pm = new BlinkRainbow();
    unsigned long millis = millis();
    EXPECT_EQ(colors::RED, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 1000));
    EXPECT_EQ(colors::BLUE, pm->nextColor(millis + 4000));
    EXPECT_EQ(colors::RED, pm->nextColor(millis + 7000));
}

TEST(Modes, BlinkRainbow2) {
    ProgramMode* pm = new BlinkRainbow(100);
    unsigned long millis = millis();
    EXPECT_EQ(colors::RED, pm->nextColor(millis + 0));
    EXPECT_EQ(colors::ORANGE, pm->nextColor(millis + 100));
    EXPECT_EQ(colors::BLUE, pm->nextColor(millis + 400));
    EXPECT_EQ(colors::RED, pm->nextColor(millis + 700));
}
