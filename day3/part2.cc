#include <algorithm>
#include <print>
#include <ranges>

#include "utils.hpp"

std::uint64_t algo(const std::string &str) noexcept;

int main() {
  auto content = get_lines_from_file("./build/day3/input.txt");
  if (!content.has_value()) {
    std::println("File not fond");
    return 1;
  }
  auto rows_results = content.value() | std::views::transform(algo);
  auto result =
      std::ranges::fold_left(rows_results, std::uint64_t{0}, std::plus{});
  std::println("Result: {}", result);

  return 0;
}

uint64_t algo(const std::string &str) noexcept {
  std::vector<char> result;
  result.reserve(12);

  auto to_remove = str.size() - 12;

  for (const auto s : str) {

    while (result.size() != 0 && result[result.size() - 1] < s &&
           to_remove > 0) {
      result.pop_back();
      to_remove--;
    }

    result.push_back(s);
  }

  std::uint64_t res(0);
  const auto size = result.size() < 12 ? result.size() : 12;
  std::from_chars(result.data(), result.data() + size, res);

  return res;
}
