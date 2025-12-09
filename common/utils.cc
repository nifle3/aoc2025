#include "utils.hpp"

#include <fstream>
#include <iterator>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

std::optional<std::vector<std::string>>
get_lines_from_file(std::string_view file_name) noexcept {
  std::ifstream file(file_name.data());
  if (!file)
    return std::nullopt;

  std::vector<std::string> lines;
  std::string line;

  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  return lines;
}

std::optional<std::string> read_file(std::string_view file_name) noexcept {
  std::ifstream file(file_name.data());
  if (!file) {
    return std::nullopt;
  }
  return std::string(std::istreambuf_iterator<char>(file), {});
}

std::optional<std::vector<std::string>>
read_file_with_delimeter(std::string_view file_name,
                         std::string_view delimeter) noexcept {

  auto content = read_file(file_name);
  if (!content.has_value()) {
    return std::nullopt;
  }

  return content.value() | std::views::split(delimeter) |
         std::views::transform([](auto &&subrange) {
           return std::string(subrange.begin(), subrange.end());
         }) |
         std::ranges::to<std::vector>();
}

std::size_t str_to_sizet(std::string_view str) {
  str.remove_prefix(std::min(str.find_first_not_of(" \t\r\n"), str.size()));
  str.remove_suffix(str.size() - str.find_last_not_of(" \t\r\n") - 1);
  std::size_t result;
  auto [ptr, err] =
      std::from_chars(str.data(), str.data() + str.size(), result);
  if (err == std::errc::result_out_of_range) {
    throw std::out_of_range{"overflow"};
  }

  if (err != std::errc{} || ptr != (str.data() + str.size())) {
    throw std::invalid_argument{"bad number"};
  }

  return result;
}

std::uint64_t sv_to_uint64t(std::string_view sv) {
  sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
  sv.remove_suffix(sv.size() - sv.find_last_not_of(" \t\r\n") - 1);
  std::uint64_t result;
  auto [ptr, err] = std::from_chars(sv.data(), sv.data() + sv.size(), result);

  if (err == std::errc::result_out_of_range) {
    throw std::out_of_range{"overflow"};
  }
  if (err != std::errc{} || ptr != (sv.data() + sv.size())) {
    throw std::invalid_argument{"bad number"};
  }

  return result;
}

std::uint64_t sv_to_int64t(std::string_view sv) {
  sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
  sv.remove_suffix(sv.size() - sv.find_last_not_of(" \t\r\n") - 1);
  std::int64_t result;
  auto [ptr, err] = std::from_chars(sv.data(), sv.data() + sv.size(), result);

  if (err == std::errc::result_out_of_range) {
    throw std::out_of_range{"overflow"};
  }
  if (err != std::errc{} || ptr != (sv.data() + sv.size())) {
    throw std::invalid_argument{"bad number"};
  }

  return result;
}
