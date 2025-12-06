#include <algorithm>
#include <print>
#include <ranges>

#include "utils.hpp"

int algo(const std::string &str) noexcept;

int main() {
  auto content = get_lines_from_file("./build/day3/input.txt");
  if (!content.has_value()) {
    std::println("File not fond");
    return 1;
  }
  auto rows_results = content.value() | std::views::transform(algo);
  auto result =
      std::ranges::fold_left(rows_results, std::size_t{0}, std::plus{});
  std::println("Result: {}", result);

  return 0;
}

int algo(const std::string &str) noexcept {
  int max = 0;

  for (size_t i = 0; i < str.size(); i++) {
    for (size_t j = i + 1; j < str.size(); j++) {
      std::string s = "";
      s += str[i];
      s += str[j];

      auto num = std::stoi(s);
      if (max < num) {
        max = num;
      }
    }
  }

  return max;
}
