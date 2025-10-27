// Basic CLI for tree generator, only allows positive inputs
#include "tree_generator.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
  const int num_nodes = (argc > 1) ? std::max(1, std::atoi(argv[1])) : 30;
  const int max_leaves = (argc > 2) ? std::max(1, std::atoi(argv[2])) : 3;

  const auto trees = GenerateTrees(num_nodes, max_leaves);

  std::cout << "Generating trees for N=" << num_nodes
            << " with leaf bound M=" << max_leaves << "\n"
            << "Final count for N=" << num_nodes
            << ", leaves <= " << max_leaves
            << ": " << trees.size() << "\n"
            << "Listing canonical forms:\n";

  for (const Tree& tree : trees) {
    std::cout << tree.canonical_form << "\n";
  }

  return 0;
}
