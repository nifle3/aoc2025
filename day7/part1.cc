#include <algorithm>
#include <cstdint>
#include <print>
#include <ranges>
#include <set>

#include "utils.hpp"

struct Solver {
  std::set<std::int64_t> beans_path;
  std::uint64_t res;

  Solver() : beans_path({}), res(0) {}

  void first_line(const std::string_view sv) {
    for (const auto [i, c] : std::views::enumerate(sv)) {
      if (c == 'S') {
        this->beans_path.insert(i);
        break;
      }
    }
  }

  void operator()(const std::string_view sv) {
    for (const auto [i, c] : std::views::enumerate(sv)) {
      if (c != '^') {
        continue;
      }

      if (!this->beans_path.contains(i)) {
        continue;
      }

      this->beans_path.erase(i);
      this->beans_path.insert(i - 1);
      this->beans_path.insert(i + 1);
      this->res++;
    }
  }
};

int main() {
  const auto result =
      get_lines_from_file("./build/day7/input.txt").transform([](auto &&vec) {
        Solver sol{};
        sol.first_line(vec[0]);
        std::ranges::for_each(vec | std::views::drop(1), std::ref(sol));
        return sol.res;
      });

  if (!result.has_value()) {
    std::println("Error");
    return 1;
  }

  std::println("Result: {}", result.value());
  return 0;
}
