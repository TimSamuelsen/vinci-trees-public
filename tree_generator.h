// tree_generator.h
// Generates all topologically unique rooted trees with N nodes and ≤ M leaves.
#ifndef TREE_GENERATOR_H_
#define TREE_GENERATOR_H_

#include <string>
#include <vector>

struct Tree {
  int leaves = 0;
  std::string canonical_form;
};

// Generates all possible trees for the given num_nodes and max_leaves
std::vector<Tree> GenerateTrees(int num_nodes, int max_leaves);

#endif  // TREE_GENERATOR_H_
