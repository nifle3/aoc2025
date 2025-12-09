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
      const auto t = std::make_tuple(first.distance_sqr(second), i, j);
      edges.push_back(std::move(t));
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

    parent[a_lead] = b_lead;
  };

  for (const auto [_, i, j] : edges | std::views::take(1000)) {
    unite(i, j);
  }

  std::map<int64_t, uint64_t> sizes;

  for (const auto [i, _] : points | std::views::enumerate) {
    const auto root = leader(leader, i);
    sizes[root] += 1;
  }

  std::vector<std::uint64_t> sizesv;

  for (const auto [_, size] : sizes) {
    sizesv.push_back(size);
  }

  std::sort(sizesv.begin(), sizesv.end(), std::greater<std::uint64_t>{});

  uint64_t result(1);
  for (const auto size : sizesv | std::views::take(3)) {
    result *= size;
  }

  std::println("Result: {}", result);

  return 0;
}
