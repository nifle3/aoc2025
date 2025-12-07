#include <print>
#include <ranges>
#include <set>

#include "utils.hpp"

struct Interval {
  std::uint64_t l;
  std::uint64_t r;

  Interval(std::uint64_t l, std::uint64_t r) : l(l), r(r) {}

  bool operator<(const Interval &other) const { return this->l < other.l; }
};

std::pair<std::string, std::string> split_with(std::string_view str) {
  auto a =
      str | std::views::split('-') | std::views::transform([](auto &&subrange) {
        return std::string(subrange.begin(), subrange.end());
      });

  auto it = a.begin();
  std::string start = *it++;
  std::string end = *it;

  return std::pair{start, end};
}

void add_interval(std::set<Interval> &intervals, std::uint64_t l,
                  std::uint64_t r) {
  Interval new_interval(l, r);
  auto it = intervals.lower_bound(new_interval);
  if (it != intervals.begin()) {
    it--;
    if (it->r < new_interval.l) {
      it++;
    }
  }

  while (it != intervals.end()) {
    if (it->l > new_interval.r) {
      break;
    }
    new_interval.l = std::min(new_interval.l, it->l);
    new_interval.r = std::max(new_interval.r, it->r);

    it = intervals.erase(it);
  }

  intervals.insert(new_interval);
}

int main() {
  auto content = get_lines_from_file("./build/day5/input.txt");
  if (!content.has_value()) {
    std::println("File not found");
    return 1;
  }

  auto data = content.value();
  auto it = std::find(data.begin(), data.end(), "");
  std::vector<std::string> ids_range(data.begin(), it);
  std::vector<std::string> ids(it != data.end() ? it + 1 : it, data.end());

  std::set<Interval> intervals;

  for (auto &&range : ids_range) {
    const auto [start, end] = split_with(range);
    const auto starti = std::stoull(std::move(start));
    const auto endi = std::stoull(std::move(end));
    add_interval(intervals, starti, endi);
  }

  std::size_t result(0);

  for (auto &&interval : intervals) {
    result += interval.r - interval.l + 1;
  }

  std::println("Result: {}", result);

  return 0;
}
