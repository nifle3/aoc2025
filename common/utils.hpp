#pragma once

#include <optional>
#include <string>
#include <vector>

std::optional<std::vector<std::string>>
get_lines_from_file(std::string_view file_name) noexcept;
