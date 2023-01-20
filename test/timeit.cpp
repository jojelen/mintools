#include <chrono>
#include <string_view>
#include <vector>

#include "tinytest.hpp"

#define THOUSAND 1e3
#define MILLION 1e6
#define BILLION 1e9

void printDuration(std::string_view msg, const std::chrono::time_point<std::chrono::steady_clock> &start, const std::chrono::time_point<std::chrono::steady_clock> &end) {
  std::chrono::duration<double> duration = end - start;
  auto ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
  if (ns < THOUSAND) {
    std::cout << msg << ": " << ns << " ns\n";
  } else if (ns >= THOUSAND && ns < MILLION) {
    auto us =
        std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::cout << msg << ": " << us << " us\n";
  } else if (ns >= MILLION && ns <= BILLION) {
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << msg << ": " << ms << " ms\n";
  } else {
    size_t sec = static_cast<size_t>(
        std::chrono::duration_cast<std::chrono::seconds>(duration).count());

    size_t min = sec / 60;
    size_t hours = min / 60;
    min %= 60;
    sec = sec - 60 * 60 * hours - 60 * min;
    std::cout << msg << ": " << hours << " h " << min << " m " << sec << " s\n";
  }
}

template<typename T>
std::vector<T> vec_add(const std::vector<T> a, const std::vector<T> b) {
	std::vector<T> c(a.size());
	for (size_t i = 0; i < a.size(); ++i) {
		c[i] = a[i] + b[i];
	}

	return c;
}

TEST_CASE(vec_addition_1e4) {
	size_t n = 1e4;
	std::vector<int> vec1(n, 42);
	std::vector<int> vec2(n, 42);

    auto start = std::chrono::steady_clock::now();
	auto res = vec_add(vec1, vec2);
    auto end = std::chrono::steady_clock::now();
	printDuration("Vector addition: ", start, end);
}

TEST_CASE(vec_addition_1e8) {
	size_t n = 1e8;
	std::vector<int> vec1(n, 42);
	std::vector<int> vec2(n, 42);

    auto start = std::chrono::steady_clock::now();
	auto res = vec_add(vec1, vec2);
    auto end = std::chrono::steady_clock::now();
	printDuration("Vector addition: ", start, end);
}
