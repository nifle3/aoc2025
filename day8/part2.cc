#include <cmath>
#include <cstdint>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <string_view>

#include "utils.hpp"

struct Point {
  std::int64_t x, y, z;

  Point(std::uint64_t x, std::uint64_t y, std::uint64_t z) noexcept
      : x(x), y(y), z(z) {}

  std::uint64_t distance_sqr(const Point &other) const noexcept {
    const auto d_x = this->x - other.x;
    const auto d_y = this->y - other.y;
    const auto d_z = this->z - other.z;
    return (d_x * d_x) + (d_y * d_y) + (d_z * d_z);
  }
};

int main() {
  auto content = get_lines_from_file("./build/day8/input.txt");
  if (!content.has_value()) {
    std::println("Error while reading file");
    return 1;
  }

  const auto split_row = [](std::string_view sv) {
    return sv | std::views::split(',') | std::views::transform([](auto &&b) {
             const auto sv = std::string_view(b.begin(), b.end());
             return sv_to_uint64t(sv);
           }) |
           std::ranges::to<std::vector>();
  };

  const auto parse_point = [](const std::vector<std::uint64_t> &v) {
    return Point(v[0], v[1], v[2]);
  };

  const auto points = content.value() | std::views::transform(split_row) |
                      std::views::transform(parse_point) |
                      std::ranges::to<std::vector>();

  using edge = std::tuple<std::uint64_t, std::int64_t, std::int64_t>;

  std::vector<edge> edges;

  for (const auto [i, first] : std::views::enumerate(points)) {
    for (const auto [j, second] :
         points | std::views::enumerate | std::views::drop(i + 1)) {
      edges.emplace_back(first.distance_sqr(second), i, j);
    }
  }

  std::sort(edges.begin(), edges.end(),
            [](const edge &first, const edge &second) {
              return std::get<0>(first) < std::get<0>(second);
            });

  std::vector<std::int64_t> parent(points.size());
  std::iota(parent.begin(), parent.end(), 0);

  auto leader = [&parent](auto self, std::int64_t v) {
    if (parent[v] == v) {
      return v;
    }

    return parent[v] = self(self, parent[v]);
  };

  auto unite = [&parent, &leader](std::int64_t a, std::int64_t b) {
    const auto a_lead = leader(leader, a);
    const auto b_lead = leader(leader, b);

    if (a_lead == b_lead) {
      return false;
    }
    parent[a_lead] = b_lead;
    return true;
  };

  std::uint64_t result(0);
  std::int64_t component_count(points.size());
  for (const auto [_, i, j] : edges) {
    auto b = unite(i, j);
    if (b) {
      component_count--;
    }

    if (component_count == 1) {
      result = static_cast<std::uint64_t>(points[i].x) *
               static_cast<std::uint64_t>(points[j].x);
      break;
    }
  }

  std::println("Result: {}", result);

  return 0;
}
