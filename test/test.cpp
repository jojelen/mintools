#include "argparse.hpp"
#include "tinytest.hpp"

TEST_CASE(basic_ok) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 6;
    const char *argv[] = {"bin", "-x", "100", "-y", "200", "-d"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret == 0);

    int x = parser.get<int>("x");
    int y = parser.get<int>("y");
    bool debug = parser.get<bool>("debug");

    ASSERT(x == 100);
    ASSERT(y == 200);
    ASSERT(debug);
}

TEST_CASE(wrong_x) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 6;
    const char *argv[] = {"bin", "-z", "100", "-y", "200", "-d"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret != 0);
}

TEST_CASE(wrong_y) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 6;
    const char *argv[] = {"bin", "-x", "100", "-z", "200", "-d"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret != 0);
}

TEST_CASE(wrong_x_value) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 6;
    const char *argv[] = {"bin", "-x", "abc", "-z", "200", "-d"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret != 0);
}

TEST_CASE(wrong_positional) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 7;
    const char *argv[] = {"bin", "-x", "123", "-y", "200", "-d", "positional"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret != 0);
}

TEST_CASE(basic_default) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);
    parser.addArg("y").optional("-y").defaultValue(180);
    parser.addArg("debug").optional("-d").defaultValue(false);

    int argc = 1;
    const char *argv[] = {"bin"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret == 0);

    int x = parser.get<int>("x");
    int y = parser.get<int>("y");
    bool debug = parser.get<bool>("debug");

    ASSERT(x == 320);
    ASSERT(y == 180);
    ASSERT(debug == false);
}

TEST_CASE(argc_mismatch) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);

    int argc = 1;
    const char *argv[] = {"bin", "-x", "123"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret == 0);

    int x = parser.get<int>("x");
    ASSERT(x == 320);
}

TEST_CASE(wrong_type_getter) {
    ArgParser parser("Test 1");
    parser.description("Test number 1");
    parser.addArg("x").optional("-x").defaultValue(320);

    int argc = 3;
    const char *argv[] = {"bin", "-x", "123"};

    int ret = parser.parse(argc, argv);
    ASSERT(ret == 0);

    try {
        auto x = parser.get<std::string>("x");
    } catch (const std::invalid_argument &e) {
    }
}
