#include <colors.h>
#include <modes.h>
#include <utils.h>

#include <unity.h>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;
using namespace colors;
#define millis() (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())

#include <gtest/gtest.h>

TEST(Utils, Concat) {
    const char* s1 = "hello";
    const char* s2 = " world";
    const char* expected = "hello world";

    EXPECT_STREQ(expected, concat(s1, s2));
    EXPECT_STREQ(expected, concat("hello", s2));
    EXPECT_STREQ(expected, concat(s1, " world"));
}

TEST(Utils, AtWithOverflow) {
    vector<int> v = {0, 1, 2, 3, 4};

    EXPECT_EQ(0, atWithOverflow(v, 0));
    EXPECT_EQ(2, atWithOverflow(v, 2));
    EXPECT_EQ(0, atWithOverflow(v, 5));
    EXPECT_EQ(1, atWithOverflow(v, 11));
}

TEST(Utils, Split) {
    string s("Ciao mamma ciao");
    vector<string> expected = {"Ciao", "mamma", "ciao"};

    EXPECT_EQ(expected, split(s));
}

TEST(Color, StringConstructor) {
    Color c(string("ff7700"));
    EXPECT_EQ(255, c.red);
    EXPECT_EQ(119, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, StringConstructorHashtag) {
    Color c(string("#ff7700"));
    EXPECT_EQ(255, c.red);
    EXPECT_EQ(119, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, CharConstructorHashtag) {
    Color c("#ff7700");
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
    EXPECT_STREQ("rgb(255, 119, 0)", c1.toString().c_str());
}

TEST(Color, Stream) {
    Color c1(0xFF7700);
    EXPECT_STREQ(
        "rgb(255, 119, 0)", c1.toString().c_str());
}

TEST(Color, brightness) {
    Color c = brightness(Color(0xFF7700), 0.3);
    EXPECT_EQ(76, c.red);
    EXPECT_EQ(35, c.green);
    EXPECT_EQ(0, c.blue);
}

TEST(Color, GammaCorrection) {
    channels g = gamma(Color(0xFF7700));
    EXPECT_EQ(255, g.red);
    EXPECT_EQ(30, g.green);
    EXPECT_EQ(0, g.blue);
}

TEST(Color, string2vector) {
    string s("#FF0000,#00FF00,#0000FF");
    vector<Color> expected = {
        Color (0xFF0000),
        Color (0x00FF00),
        Color (0x0000FF)
    };
    EXPECT_EQ(expected, string2vector(s));
}



TEST(Modes, SolidColor) {
    unique_ptr<ProgramMode> pm(new SolidColor(ORANGE));
    EXPECT_EQ(ORANGE, pm->nextColor(10));
    EXPECT_EQ(ORANGE, pm->nextColor(1000));
}

TEST(Modes, Jump) {
    vector<Color> cols {ORANGE, BLACK};
    unique_ptr<ProgramMode> pm(new Jump(cols));
    unsigned long millis = millis();
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(BLACK, pm->nextColor(millis + 1000));
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 2000));
}

TEST(Modes, BlinkColor1) {
    unique_ptr<ProgramMode> pm(new BlinkColor(ORANGE));
    unsigned long millis = millis();
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(BLACK, pm->nextColor(millis + 1000));
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 2000));
}

TEST(Modes, BlinkColor2) {
    unique_ptr<ProgramMode> pm(new BlinkColor(ORANGE, 100));
    unsigned long millis = millis();
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(BLACK, pm->nextColor(millis + 100));
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 200));
}

TEST(Modes, JumpRainbow1) {
    unique_ptr<ProgramMode> pm(new JumpRainbow());
    unsigned long millis = millis();
    EXPECT_EQ(RED, pm->nextColor(millis + 0));
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 1000));
    EXPECT_EQ(BLUE, pm->nextColor(millis + 4000));
    EXPECT_EQ(RED, pm->nextColor(millis + 6000));
}

TEST(Modes, JumpRainbow2) {
    unique_ptr<ProgramMode> pm(new JumpRainbow(100));
    unsigned long millis = millis();
    EXPECT_EQ(RED, pm->nextColor(millis + 0));
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 100));
    EXPECT_EQ(BLUE, pm->nextColor(millis + 400));
    EXPECT_EQ(RED, pm->nextColor(millis + 600));
}

TEST(Modes, Fade) {
    vector<Color> cols {ORANGE, BLACK};
    unique_ptr<ProgramMode> pm(new Fade(cols));
    unsigned long millis = millis();
    EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
    EXPECT_EQ(brightness(ORANGE, 0.5), pm->nextColor(millis + 500));
    EXPECT_EQ(brightness(ORANGE, 0.25), pm->nextColor(millis + 750));
    EXPECT_EQ(BLACK, pm->nextColor(millis + 1000));
}


// TEST(Modes, FadeColor1) {
//     unique_ptr<ProgramMode> pm(new FadeColor(ORANGE));
//     unsigned long millis = millis();
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
//     EXPECT_EQ(BLACK, pm->nextColor(millis + 500));
//     EXPECT_EQ(fade(ORANGE, 0.5), pm->nextColor(millis + 750));
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 1000));
// }

// TEST(Modes, FadeColor2) {
//     unique_ptr<ProgramMode> pm(new FadeColor(ORANGE, 100));
//     unsigned long millis = millis();
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 0));
//     EXPECT_EQ(BLACK, pm->nextColor(millis + 50));
//     EXPECT_EQ(fade(ORANGE, 0.5), pm->nextColor(millis + 75));
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 100));
// }

// TEST(Modes, BlinkRainbow1) {
//     unique_ptr<ProgramMode> pm(new BlinkRainbow());
//     unsigned long millis = millis();
//     EXPECT_EQ(RED, pm->nextColor(millis + 0));
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 1000));
//     EXPECT_EQ(BLUE, pm->nextColor(millis + 4000));
//     EXPECT_EQ(RED, pm->nextColor(millis + 6000));
// }

// TEST(Modes, BlinkRainbow2) {
//     unique_ptr<ProgramMode> pm(new BlinkRainbow(100));
//     unsigned long millis = millis();
//     EXPECT_EQ(RED, pm->nextColor(millis + 0));
//     EXPECT_EQ(ORANGE, pm->nextColor(millis + 100));
//     EXPECT_EQ(BLUE, pm->nextColor(millis + 400));
//     EXPECT_EQ(RED, pm->nextColor(millis + 600));
// }
