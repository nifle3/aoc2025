#include <print>

#include "utils.hpp"

int main() {
  auto content = get_lines_from_file("./build/day1/input.txt");
  if (!content.has_value()) {
    std::println("File not found");
    return 1;
  }

  std::uint64_t res(0);
  std::int64_t dial(50);

  for (const std::string &value : content.value()) {
    auto letter = value[0];
    auto num = std::stoll(value.substr(1));

    if (letter == 'L') {
      if (dial == 0) {
        res += num / 100;
      } else {
        if (num >= dial) {
          res += 1 + (num - dial) / 100;
        }
      }

      dial = (dial - (num % 100) + 100) % 100;

    } else { // letter == 'R'
      res += (dial + num) / 100;

      dial = (dial + num) % 100;
    }
  }
  std::println("Result {}", res);
  return 0;
}
