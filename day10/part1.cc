#include <algorithm>
#include <climits>
#include <print>
#include <ranges>

#include "utils.hpp"

struct Manual {
  std::vector<std::int64_t> light;
  std::vector<std::vector<std::int64_t>> wires;
  std::vector<std::int64_t> joltage;

  Manual(const Manual &other) = delete;

  Manual(std::vector<std::int64_t> light,
         std::vector<std::vector<std::int64_t>> &&wires,
         std::vector<std::int64_t> &&joltage)
      : light(light), wires(std::move(wires)), joltage(std::move(joltage)) {}

  [[nodiscard]] std::int64_t calculate() const noexcept {
    auto wires_bit = this->wires_to_bit_mask();
    auto light_bit = this->light_to_bit_mask();
    const auto m = wires_bit.size();
    auto min = INT_MAX;

    for (std::int64_t mask = 0; mask < (1 << m); mask++) {
      std::int64_t current = 0;
      for (size_t i = 0; i < m; ++i) {
        if (mask & (1 << i)) {
          current ^= wires_bit[i];
        }
      }
      if (current == light_bit) {
        int presses = __builtin_popcount(mask);
        min = std::min(presses, min);
      }
    }

    return min;
  }

private:
  [[nodiscard]] std::int64_t light_to_bit_mask() const noexcept {
    std::int64_t res(0);

    for (const auto [i, value] : light | std::views::enumerate) {
      if (value == 1) {
        res |= (1LL << i);
      }
    }

    return res;
  }

  [[nodiscard]] std::vector<std::int64_t> wires_to_bit_mask() const noexcept {
    std::vector<std::int64_t> wires_bits;
    wires_bits.reserve(this->wires.size());
    for (const auto &wire : this->wires) {
      std::int64_t res(0);

      for (const auto value : wire) {
        res |= (1LL << value);
      }

      wires_bits.push_back(res);
    }

    return wires_bits;
  }
};

int main() {
  const auto content = get_lines_from_file("./build/day10/input.txt");
  if (!content) {
    std::println("Error");
    return 1;
  }

  const auto to_manuals = [](std::string_view sv) {
    std::vector<std::int64_t> light;
    std::vector<std::vector<std::int64_t>> wires;
    std::vector<std::int64_t> joltage;

    auto svs =
        sv | std::views::split(' ') | std::views::transform([](auto &&b) {
          return std::string_view(b.begin(), b.end());
        }) |
        std::views::filter([](std::string_view sv) { return !sv.empty(); });

    std::ranges::for_each(svs, [&](std::string_view sv) {
      const char first = sv[0];
      sv.remove_prefix(1);
      sv.remove_suffix(1);

      if (first == '[') [[unlikely]] {
        light = sv | std::views::transform([](char c) {
                  std::int64_t res = c == '.' ? 0 : 1;
                  return res;
                }) |
                std::ranges::to<std::vector>();
        return;
      }

      const auto ints = sv | std::views::split(',') |
                        std::views::transform([](auto &&b) {
                          const auto sv = std::string_view(b.begin(), b.end());
                          return sv_to_int64t(sv);
                        }) |
                        std::ranges::to<std::vector>();

      if (first == '(') [[likely]] {
        wires.push_back(std::move(ints));
        return;
      }

      if (first == '{') [[unlikely]] {
        joltage = std::move(ints);
        return;
      }
    });

    return Manual(light, std::move(wires), std::move(joltage));
  };

  const auto results =
      content.value() | std::views::transform(to_manuals) |
      std::views::transform([](auto &&row) { return row.calculate(); });

  const auto result =
      std::ranges::fold_left(results, 0, std::plus<std::int64_t>{});

  std::println("Result: {}", result);

  return 0;
}
