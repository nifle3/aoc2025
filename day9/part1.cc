#include <print>
#include <queue>
#include <ranges>
#include <string_view>

#include "utils.hpp"

struct Rectangle {
  const std::int64_t x1, y1;
  const std::int64_t x2, y2;

  Rectangle(std::int64_t x1, std::int64_t y1, std::int64_t x2,
            std::int64_t y2) noexcept
      : x1(x1), y1(y1), x2(x2), y2(y2) {}

  std::int64_t calculate_area() const noexcept {
    const auto first = static_cast<std::int64_t>(std::abs(y1 - y2 + 1));
    const auto second = static_cast<std::int64_t>(std::abs(x1 - x2 + 1));

    return first * second;
  }
};

int main() {
  auto content = get_lines_from_file("./build/day9/input.txt");
  if (!content.has_value()) {
    std::println("Error");
    return 1;
  }

  const auto parse_pair_to_int = [](std::string_view sv) {
    const auto a = sv | std::views::split(',') |
                   std::views::transform([](auto &&b) {
                     const auto sv = std::string_view(b.begin(), b.end());
                     return sv_to_int64t(sv);
                   }) |
                   std::ranges::to<std::vector>();

    return std::make_pair(a[0], a[1]);
  };

  const auto pairs = content.value() |
                     std::views::transform(parse_pair_to_int) |
                     std::ranges::to<std::vector>();

  std::priority_queue<std::int64_t> q;

  for (const auto [i, first] : pairs | std::views::enumerate) {
    const auto [x1, y1] = first;
    for (const auto second : pairs | std::views::drop(i + 1)) {
      const auto [x2, y2] = second;

      const Rectangle rec(x1, y1, x2, y2);
      const auto area = rec.calculate_area();

      q.push(area);
    }
  }

  const auto result = q.top();
  std::println("Result: {}", result);

  return 0;
}
