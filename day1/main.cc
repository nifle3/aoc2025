#include <print>
#include <string>

#include "utils.hpp"

int main() {
  auto lines = get_lines_from_file("./build/day1/input.txt");
  if (!lines.has_value()) {
    std::println("File not found");
    return 1;
  }

  std::uint64_t res(0);
  std::int64_t dial(50);

  for (const std::string &value : lines.value()) {
    auto letter = value[0];
    auto num = std::stoi(value.substr(1));
    if (letter == 'L') {
      dial -= num;
      while (dial < 0) {
        dial = 100 + dial;
      }
    } else {
      dial = (dial + num) % 100;
    }

    if (dial == 0) {
      res++;
    }
  }
  std::println("Count of line size {}", lines.value().size());
  std::println("Result {}", res);
  return 0;
}
