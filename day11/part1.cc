#include <deque>
#include <print>
#include <ranges>
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "utils.hpp"

int main() {
  auto content = get_lines_from_file("./build/day11/input.txt");
  if (!content) {
    std::println("Error");
    return 1;
  }

  const auto row_to_in_out = [](std::string_view sv) {
    auto in_out = sv | std::views::split(':') | std::ranges::to<std::vector>();
    const auto in = in_out[0];
    const auto out =
        in_out[1] | std::views::split(' ') |
        std::views::transform(
            [](auto &&b) { return std::string_view(b.begin(), b.end()); }) |
        std::views::filter([](std::string_view sv) { return !sv.empty(); }) |
        std::ranges::to<std::vector>();

    return std::make_pair(in, out);
  };

  const auto in_out = content.value() | std::views::transform(row_to_in_out);

  std::unordered_map<std::string_view, std::vector<std::string_view>> m;
  for (auto &&edge : in_out) {
    m.insert(std::move(edge));
  }

  std::deque<std::string_view> d;
  std::int64_t result(0);
  d.push_back("you");

  while (d.size() != 0) {
    const auto e = d.front();
    d.pop_front();
    if (e == "out") {
      result++;
      continue;
    }

    for (const auto value : m[e]) {
      d.push_back(value);
    }
  }

  std::println("Result: {}", result);

  return 0;
}
