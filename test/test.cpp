#include "argparse.hpp"

#include <iostream>
#include <string>

#define ASSERT_EQ(equal) \
  if (!equal) { \
    throw std::runtime_error("Assert failed on line " + __LINE__); \
  }


//TEST_CASE(ok_test) {
void ok_test() {
  ArgParser parser("Test 1");
  parser.description("Test number 1");
  parser.addArg("x").optional("-x").defaultValue(320);
  parser.addArg("y").optional("-y").defaultValue(180);
  parser.addArg("debug").optional("-d").defaultValue(false);

  int argc = 6;
  const char* argv[] = {"bin", "-x", "100", "-y", "200", "-d"};

  int ret = parser.parse(argc, argv);
  ASSERT_EQ(ret == 0);

  int x = parser.get<int>("x");
  int y = parser.get<int>("y");
  bool debug = parser.get<bool>("debug");

  ASSERT_EQ(x == 100);
  ASSERT_EQ(y == 200);
  ASSERT_EQ(debug == false);
}


int main() {
  ok_test();
}