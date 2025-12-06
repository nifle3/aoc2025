#include <functional>
#include <print>
#include <ranges>
#include <string_view>

#include "utils.hpp"

int main() {
  auto content = get_lines_from_file("./build/day6/input.txt");
  if (!content.has_value()) {
    std::println("File not found");
    return 1;
  }
  std::vector<std::vector<std::string_view>> splited_rows;

  for (const auto &row : content.value()) {
    const auto splited_row =
        row | std::views::split(' ') |
        std::views::filter([](auto &&b) { return !std::ranges::empty(b); }) |
        std::views::transform(
            [](auto &&b) { return std::string_view(b.begin(), b.end()); }) |
        std::ranges::to<std::vector>();
    splited_rows.push_back(std::move(splited_row));
  }

  std::size_t result(0);

  for (size_t i = 0; i < splited_rows[0].size(); i++) {
    std::function<std::size_t(std::size_t, std::size_t)> op;
    if (splited_rows[splited_rows.size() - 1][i] == "*") {
      op = std::multiplies<std::size_t>{};
    } else {
      op = std::plus<std::size_t>{};
    }

    std::size_t accamulate = str_to_sizet(splited_rows[0][i]);

    for (size_t j = 1; j < splited_rows.size() - 1; j++) {
      accamulate = op(accamulate, str_to_sizet(splited_rows[j][i]));
    }
    result += accamulate;
  }

  std::println("Result: {}", result);

  return 0;
}
