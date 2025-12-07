#include <memory>
#include <print>
#include <ranges>

#include "utils.hpp"

struct TreeNode {
  std::uint64_t end;
  std::uint64_t start;
  std::uint64_t max_end;

  std::shared_ptr<TreeNode> left;
  std::shared_ptr<TreeNode> right;

  TreeNode(std::uint64_t start, std::uint64_t end) noexcept
      : end(end), start(start), max_end(end), left(nullptr), right(nullptr) {}

  void add(std::shared_ptr<TreeNode> new_node) noexcept {
    this->max_end = std::max(this->max_end, new_node->end);

    if (this->start <= new_node->start) {
      if (!this->right) {
        this->right = new_node;
        return;
      }

      this->right->add(new_node);
    } else {
      if (!this->left) {
        this->left = new_node;
        return;
      }

      this->left->add(new_node);
    }
  }
  bool find(std::uint64_t value) noexcept {
    if (this->max_end < value) {
      return false;
    }

    if (this->end >= value && this->start <= value) {
      return true;
    }

    if (this->left && this->left->max_end >= value) {
      return this->left->find(value);
    }

    if (this->right && this->start < value) {
      return this->right->find(value);
    }

    return false;
  }
};

struct Tree {
  std::shared_ptr<TreeNode> root;

  Tree() noexcept : root(nullptr) {}

  void add(std::shared_ptr<TreeNode> new_node) noexcept {
    if (!this->root) {
      this->root = new_node;
    } else {
      this->root->add(new_node);
    }
  }

  bool find(std::uint64_t value) {
    if (!this->root) {
      return false;
    }

    return this->root->find(value);
  }
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

  Tree tree;

  for (auto &&range : ids_range) {
    const auto [start, end] = split_with(range);
    const auto starti = std::stoull(std::move(start));
    const auto endi = std::stoull(std::move(end));
    const auto new_node = std::make_shared<TreeNode>(starti, endi);
    tree.add(new_node);
  }

  std::size_t result(0);

  for (auto &&id : ids) {
    const auto idi = std::stoull(std::move(id));
    const auto is_find = tree.find(idi);
    if (is_find) {
      result++;
    }
  }

  std::println("Result: {}", result);

  return 0;
}
