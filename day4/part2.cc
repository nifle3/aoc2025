#include <cstddef>
#include <print>
#include <ranges>

#include "utils.hpp"

constexpr std::array<std::pair<std::int8_t, std::int8_t>, 8> dirs = {
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}}};

int main() {
  auto content = get_lines_from_file("./build/day4/input.txt");
  if (!content.has_value()) {
    std::println("File not found");
    return 1;
  }

  std::size_t result(0);
  bool is_any_deleted(false);
  do {
    std::vector<std::pair<std::int64_t, std::int64_t>> deleted_pos;

    for (const auto [y, row] : std::views::enumerate(content.value())) {
      for (const auto [x, cell] : std::views::enumerate(row)) {
        if (cell != '@') {
          continue;
        }

        std::ptrdiff_t count_of_neighbor = 0;

        for (const auto [x_dir, y_dir] : dirs) {
          const auto new_x = x_dir + x;
          const auto new_y = y_dir + y;

          if (new_x < 0 || new_x >= static_cast<int>(row.size())) {
            continue;
          }

          if (new_y < 0 || new_y >= static_cast<int>(content.value().size())) {
            continue;
          }

          if (content.value()[new_y][new_x] == '@') {
            count_of_neighbor++;
          }
        }

        if (count_of_neighbor < 4) {
          result++;
          deleted_pos.push_back({x, y});
        }
      }
    }

    if (deleted_pos.size() != 0) {
      is_any_deleted = true;
    } else {
      is_any_deleted = false;
    }

    for (const auto [x, y] : deleted_pos) {
      content.value()[y][x] = '.';
    }
  } while (is_any_deleted);

  std::println("Result: {}", result);

  return 0;
}
