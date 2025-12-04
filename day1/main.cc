#include <print>

#include "utils.hpp"

int main() {
  auto lines = get_lines_from_file("input.txt");
  if (!lines.has_value()) {
    std::println("File not found");
    return 1;
  }

  std::println("Hello world {}", lines.value().size());
  return 0;
}
