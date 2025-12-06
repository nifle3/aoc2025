#include <algorithm>
#include <cstddef>
#include <generator>
#include <print>
#include <ranges>

#include "utils.hpp"

std::pair<std::string_view, std::string_view> split_with(std::string_view str);

std::pair<std::size_t, std::size_t>
convert_pair_to_size_t(std::pair<std::string_view, std::string_view> &&pair);

bool is_valid_number(std::size_t num);

int main() {
  auto content = read_file_with_delimeter("./build/day2/input.txt", ",");
  if (!content.has_value()) {
    std::println("File not found");
    return 1;
  }

  auto nums = content.value() | std::views::transform(split_with) |
              std::views::transform(convert_pair_to_size_t) |
              std::views::transform([](auto p) {
                return std::views::iota(p.first, p.second + 1);
              }) |
              std::views::join | std::views::filter(is_valid_number);

  auto result = std::ranges::fold_left(nums, std::size_t{0}, std::plus{});

  std::println("Result is {}", result);

  return 0;
}

std::pair<std::string_view, std::string_view> split_with(std::string_view str) {
  auto a =
      str | std::views::split('-') | std::views::transform([](auto &&subrange) {
        return std::string_view(subrange.begin(), subrange.end());
      });

  auto it = a.begin();
  std::string_view key = *it++;
  std::string_view value = *it;

  return std::pair{key, value};
}

std::pair<std::size_t, std::size_t>
convert_pair_to_size_t(std::pair<std::string_view, std::string_view> &&pair) {
  std::println("first: {}, second: {}", pair.first, pair.second);
  return std::pair{str_to_sizet(pair.first), str_to_sizet(pair.second)};
}

bool is_valid_number(std::size_t num) {
  bool is_repeated = false;
  int k = 0;
  int a = 10;
  std::size_t b = 0;
  std::size_t prev = 0;
  while (b != num) {
    prev = b;
    k++;
    b = num % a;
    a *= 10;
    if (b == prev) {
      continue;
    }

    std::size_t r = 1;
    for (std::size_t i = 0; i < k; i++) {
      r *= 10;
    }

    if (num == ((b * r) + b)) {
      is_repeated = true;
      break;
    }
  }

  return is_repeated;
}
