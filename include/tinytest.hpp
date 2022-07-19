// Small single-header test framework.
//
// std: C++17
// Author: Joel Oredsson
//
// Example of usage (Note that tinytest creates a main function):
//
//  #include "tinytest.hpp"
//
//  TEST_CASE(first_test) {
//      int x = 1 + 1;
//      ASSERT(x == 2);
//  }
#pragma once

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

#define ASSERT(is_true)                                  \
    if (!(is_true)) {                                    \
        size_t line = __LINE__;                          \
        throw AssertException("Assert failed on line " + \
                              std::to_string(line));     \
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

    static int run();

    template <typename T>
    static void add();
};

std::vector<TestCase> Register::tests = {};

template <typename T>
void Register::add() {
    TestCase test_case;
    test_case.name = std::string_view(T::name);
    test_case.func = std::function<void(void)>(T::run);
    test_case.line_number = T::line_number;
    test_case.file = T::file;
    tests.push_back(test_case);
}

int Register::run() {
    std::cout << "\nStarting test suite!\n" << std::endl;
    std::vector<std::string_view> failed_tests;
    for (auto &t : Register::tests) {
        // DEBUG
        // std::cout << "Running " << t.name << " in \"" << t.file << "\" on
        // line nr
        // "
        //          << t.line_number << std::endl;
        std::cout << "[" << t.name << "]: Running..." << std::endl;
        try {
            t.func();
            std::cout << "[" << t.name << "]: OK!" << std::endl;
        } catch (const AssertException &e) {
            std::cerr << "[" << t.name << "]: " << e.what() << std::endl;
            failed_tests.emplace_back(t.name);
        } catch (const std::exception &e) {
            std::cerr << "[" << t.name << "]: " << e.what() << std::endl;
            failed_tests.emplace_back(t.name);
        }
    }

    size_t num_failed = failed_tests.size();
    size_t num_ok = Register::tests.size() - num_failed;
    std::cout << "\nTest suite finished!\n";
    std::cout << "Out of " << std::to_string(Register::tests.size())
              << " tests, " << std::to_string(num_ok) << " passed and "
              << std::to_string(num_failed) << " failed." << std::endl;

    return num_failed == 0 ? 0 : -1;
}

#define TEST_CASE(func_name)                                 \
    struct func_name {                                       \
        static constexpr std::string_view name = #func_name; \
        static constexpr size_t line_number = __LINE__;      \
        static constexpr std::string_view file = __FILE__;   \
        static void run();                                   \
                                                             \
        func_name() { Register::add<func_name>(); }          \
    };                                                       \
    func_name test_##func_name;                              \
    void func_name::run()

int main() { return Register::run(); }
