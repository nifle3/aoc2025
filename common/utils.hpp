#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

std::optional<std::vector<std::string>>
get_lines_from_file(std::string_view file_name) noexcept;

std::optional<std::string> read_file(std::string_view file_name) noexcept;

std::optional<std::vector<std::string>>
read_file_with_delimeter(std::string_view file_name,
                         std::string_view delimeter) noexcept;

std::size_t str_to_sizet(const std::string_view str);

std::uint64_t sv_to_uint64t(std::string_view sv);
std::uint64_t sv_to_int64t(std::string_view sv);
