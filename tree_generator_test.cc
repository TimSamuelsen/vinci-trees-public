// Basic test file for verifying tree generator funcitonality
#include "tree_generator.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

std::vector<std::string> SortedCanonicalForms(int num_nodes, int max_leaves) {
  std::vector<std::string> forms;
  for (const Tree& tree : GenerateTrees(num_nodes, max_leaves)) {
    forms.push_back(tree.canonical_form);
  }
  std::sort(forms.begin(), forms.end());
  return forms;
}

void TestCountsAndForms() {
  const int max_leaves = 4;
  const std::vector<size_t> expected_counts = {0, 1, 1, 2, 4};
  for (int level_nodes = 1; level_nodes <= 4; ++level_nodes) {
    const auto trees = GenerateTrees(level_nodes, max_leaves);
    assert(trees.size() == expected_counts[level_nodes]);
  }

  const std::vector<std::string> expected_forms = {
      "(((())))",
      "((()()))",
      "((())())",
      "(()()())",
  };
  assert(SortedCanonicalForms(/*num_nodes=*/4, max_leaves) == expected_forms);
}

void TestLeafFiltering() {
  constexpr int max_leaves = 2;
  const auto trees = GenerateTrees(/*num_nodes=*/4, max_leaves);
  assert(std::all_of(trees.begin(), trees.end(),
                     [](const Tree& tree) { return tree.leaves <= max_leaves; }));

  // Expect only shapes with at most 2 leaves to survive.
  assert(trees.size() == 3);
  std::vector<std::string> canonical_forms;
  for (const Tree& tree : trees) {
    canonical_forms.push_back(tree.canonical_form);
  }
  std::sort(canonical_forms.begin(), canonical_forms.end());
  const std::vector<std::string> expected = {
      "(((())))",
      "((()()))",
      "((())())",
  };
  assert(canonical_forms == expected);
}

}  // namespace

int main() {
  TestCountsAndForms();
  TestLeafFiltering();
  std::cout << "All tree generator tests passed.\n";
  return 0;
}
