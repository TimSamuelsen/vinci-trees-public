#include "tree_generator.h"

#include <algorithm>
#include <set>
#include <string_view>
#include <vector>

namespace {

struct TreeComparator {
  // Order trees by canonical string so std::set deduplicates shapes.
  bool operator()(const Tree& lhs, const Tree& rhs) const {
    return lhs.canonical_form < rhs.canonical_form;
  }
};

// Copies each child's canonical string, sorts them to avoid 
// duplicates and concatenates them to create parent's canonical form.
// e.g. children ["(())", "(()())"] -> canonical "((())(()()))".
std::string CanonicalFromChildren(
    const std::vector<const Tree*>& child_pointers) {
  // handle case of no children
  if (child_pointers.empty()) {
    return "()";
  }

  // Copy child canonical strings into sortable views so permutations collapse.
  std::vector<std::string_view> child_fragments(child_pointers.size());
  for (size_t child_index = 0; child_index < child_pointers.size();
       ++child_index) {
    child_fragments[child_index] =
        child_pointers[child_index]->canonical_form;
  }
  // Sort child fragments to ensure that they're always in the same order 
  std::sort(child_fragments.begin(), child_fragments.end());

  // Generate full canonical form of tree
  std::string canonical_form;
  canonical_form.push_back('(');
  for (std::string_view fragment : child_fragments) {
    canonical_form.append(fragment);
  }
  canonical_form.push_back(')');
  return canonical_form;
}

// Tries every tree of required size for each child slot,
// skips tree with too many leaves and inserts final parent 
// tree into the unique_trees upon completion. 
// e.g. partition [2,1], selected ["(()())","()"] -> parent "((()())())".
void DfsCombinations(size_t child_index,
                     int accumulated_leaves,
                     int max_leaves,
                     const std::vector<int>& partition_parts,
                     const std::vector<std::vector<Tree>>& memoized_levels,
                     std::vector<const Tree*>& selected_children,
                     std::set<Tree, TreeComparator>& unique_trees) {
  // If we're reached the desired size, add it to the unique_trees and exit                    
  if (child_index == partition_parts.size()) {
    unique_trees.insert(
        Tree{accumulated_leaves, CanonicalFromChildren(selected_children)});
    return;
  }

  const std::vector<Tree>& candidate_trees =
      memoized_levels[partition_parts[child_index]];
  if (candidate_trees.empty()) return;

  for (const Tree& candidate_tree : candidate_trees) {
    // Add leaves to candidate
    const int next_leaf_sum = accumulated_leaves + candidate_tree.leaves;
    // If we exceed the number of leaves, this tree is invalid so skip it
    if (next_leaf_sum > max_leaves) {
      continue;
    }

    selected_children[child_index] = &candidate_tree;
    DfsCombinations(child_index + 1, next_leaf_sum, max_leaves, partition_parts,
                    memoized_levels, selected_children, unique_trees);
  }
}

// Walks through every possible way the root can distribute it's children
// e.g. remaining=3 -> partitions [3], [2,1], [1,1,1] before DFS expansion.
void ExplorePartitions(int remaining_nodes,
                       int max_part_value,
                       int max_leaves,
                       const std::vector<std::vector<Tree>>& memoized_levels,
                       std::vector<int>& partition_parts,
                       std::set<Tree, TreeComparator>& unique_trees) {
  // If we're done building up the partitions, 
  if (remaining_nodes == 0) {
    if (partition_parts.empty()) {
      return;
    }
    std::vector<const Tree*> selected_children(partition_parts.size(), nullptr);
    DfsCombinations(/*child_index=*/0, /*accumulated_leaves=*/0, max_leaves,
                    partition_parts, memoized_levels, selected_children,
                    unique_trees);
    return;
  }

  for (int part_value = std::min(remaining_nodes, max_part_value);
       part_value >= 1; --part_value) {
    partition_parts.push_back(part_value);
    ExplorePartitions(remaining_nodes - part_value, part_value, max_leaves,
                      memoized_levels, partition_parts, unique_trees);
    partition_parts.pop_back();
  }
}

}  // namespace

std::vector<Tree> GenerateTrees(int num_nodes, int max_leaves) {
  // memoized_levels[i] stores all valid trees for i nodes
  std::vector<std::vector<Tree>> memoized_levels(num_nodes + 1);
  for (int level_nodes = 1; level_nodes <= num_nodes; ++level_nodes) {
    // seed the initial tree
    if (level_nodes == 1) {
      memoized_levels[1] = {Tree{1, "()"}};
      continue;
    }

    std::set<Tree, TreeComparator> unique_trees;
    std::vector<int> partition_parts;
    ExplorePartitions(level_nodes - 1, level_nodes - 1, max_leaves,
                      memoized_levels, partition_parts, unique_trees);
    memoized_levels[level_nodes] =
        std::vector<Tree>(unique_trees.begin(), unique_trees.end());
  }
  return memoized_levels[num_nodes];
}
