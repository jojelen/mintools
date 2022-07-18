#include "argparse.hpp"

#include <iostream>
#include <string>

int main(int argc, const char *argv[]) {
  ArgParser parser("Test 1");
  parser.description("Test number 1");
  parser.addArg("x").optional("-x").defaultValue(320);
  parser.addArg("y").optional("-y").defaultValue(180);
  parser.addArg("debug").optional("-d").defaultValue(false);

  int ret = parser.parse(argc, argv);
  if (ret) {
    exit(ret);
  }

  int x = parser.get<int>("x");
  int y = parser.get<int>("y");
  bool debug = parser.get<bool>("debug");

  std::cout << "x = " << std::to_string(x) << std::endl;
  std::cout << "y = " << std::to_string(y) << std::endl;
  std::cout << "debug = " << std::to_string(debug) << std::endl;
}