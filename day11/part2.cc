#include <deque>
#include <map>
#include <print>
#include <ranges>
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

  std::map<std::pair<std::string_view, std::int64_t>, std::int64_t> cache;
  const auto calc = [&](auto self, bool is_dac, bool is_fft,
                        std::string_view cur) -> std::int64_t {
    is_dac = is_dac || cur == "dac";
    is_fft = is_fft || cur == "fft";
    if (cur == "out") {
      return (is_dac && is_fft) ? 1 : 0;
    }

    std::int64_t state = (is_dac ? 1 : 0) | (is_fft ? 2 : 0);
    auto key = std::make_pair(cur, state);

    if (cache.contains(key)) {
      return cache[key];
    }

    std::int64_t sum = 0;
    for (const auto v : m.at(cur)) {
      sum += self(self, is_dac, is_fft, v);
    }

    return cache[key] = sum;
  };

  const auto result = calc(calc, false, false, "svr");

  std::println("Result: {}", result);

  return 0;
}
