#include "argparse.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <string_view>

class AssertException : public std::exception {
  std::string msg;

public:
  AssertException(const std::string &msg) : msg(msg) {}

  const char *what() const noexcept override { return msg.c_str(); }
};

#define ASSERT(is_true)                                                        \
  if (!(is_true)) {                                                            \
    size_t line = __LINE__;                                                    \
    throw AssertException("Assert failed on line " + std::to_string(line));    \
  }

struct TestCase {
  std::function<void(void)> func;
  std::string_view name;
  size_t line_number;
  std::string_view file;
};

class Register {
public:
  static std::vector<TestCase> tests;

  static void run();

  template <typename T> static void add();
};

std::vector<TestCase> Register::tests = {};

template <typename T> void Register::add() {
  TestCase test_case;
  test_case.name = std::string_view(T::name);
  test_case.func = std::function<void(void)>(T::run);
  test_case.line_number = T::line_number;
  test_case.file = T::file;
  tests.push_back(test_case);
}

void Register::run() {
  std::vector<std::string_view> failed_tests;
  for (auto &t : Register::tests) {
    // std::cout << "Running " << t.name << " in \"" << t.file << "\" on line nr
    // "
    //          << t.line_number << std::endl;
    std::cout << "[" << t.name << "]: Running..." << std::endl;
    try {
      t.func();
      std::cout << "[" << t.name << "]: OK!" << std::endl;
    } catch (const AssertException &e) {
      std::cerr << "[" << t.name << "]: " << e.what() << std::endl;
      failed_tests.emplace_back(t.name);
    }
  }

  size_t num_ok = Register::tests.size() - failed_tests.size();
  std::cout << "\nTest suite finished!\n";
  std::cout << "Out of " << std::to_string(Register::tests.size()) << " tests, "
            << std::to_string(num_ok) << " passed and "
            << std::to_string(failed_tests.size()) << " failed." << std::endl;
}

#define TEST_CASE(func_name)                                                   \
  struct func_name {                                                           \
    static constexpr std::string_view name = #func_name;                       \
    static constexpr size_t line_number = __LINE__;                            \
    static constexpr std::string_view file = __FILE__;                         \
    static void run();                                                         \
                                                                               \
    func_name() { Register::add<func_name>(); }                                \
  };                                                                           \
  func_name test_##func_name;                                                  \
  void func_name::run()

int main() { Register::run(); }

// TODO: All of the above should be in a separate header...

TEST_CASE(first_test) {
  std::cout << "First test!" << std::endl;
  ASSERT(true);
}

TEST_CASE(second_test) {
  std::cout << "Second test!" << std::endl;
  ASSERT(false);
}

TEST_CASE(basic_ok_test) {
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
  ASSERT(43 == 200);
  ASSERT(debug);
}

