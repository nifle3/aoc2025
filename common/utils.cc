#include "utils.hpp"

#include <fstream>
#include <optional>
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
