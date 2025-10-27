Tree Generation
--------------------------------------------------
In this question, we will be writing an exhaustive generator to
generate trees. Trees are allowed to have any number of children
nodes. (I.e. we are not restricting ourselves to binary trees.)
Please write a program to generate all possible trees with N nodes
that have less than or equal to M leaf nodes.
Do not redundantly generate topologically equivalent trees. A tree is
topologically equivalent to another tree if we can reorder its
children (or the children of its children, etc ...) to arrive at the
second tree. I.e. trees are not distinguished by the order of its
children.
Efficiency Goal: Please print out the trees as they are generated, and
try to minimize the time required to generate each successive tree.
Show us the generated trees, and count the total number of trees for:
 1) N = 8, M = 5. Num of Trees at N=8 and M= 5 is 108
 2) N = 30, M = 3. Num of Trees at N=30 and M= 3 is 13661

-  Please list print the Trees at N=8 and M=5
-  Please list print the TreesNum of Trees at N=30 and M=3

Key Concepts
--------------------------------------------------
### Iterative solution
The core idea is iterative: solving for `N` reuses the trees we already built for `1 … N-1`. We generate each size in order, memoising every catalogue so the next size can stitch together its children without recomputing earlier work.

### Canonical tree representation
This solution uses a canonical bracket tree representation to make deduplication, 
comparisons, and printing easier (https://webhome.cs.uvic.ca/~wendym/courses/582/16/notes/582_12_tree_can_form.pdf).
```
  o
 / \   -> (()())
o   o
```

### Partitions
We use partitions to describe how the root distributes its node among it's children.

With 3 nodes below the root (n=4), the partitions are:
[3] (one child with 3 nodes)
[2,1] (two children sized 2 and 1)
[1,1,1] (three single-node children)

Solution Overview
--------------------------------------------------
We use a basic CLI to run the tree generator. Example run:
```
# For N=30 M=3
./tree_gen 30 3
```

The CLI calls the GenerateTrees function which does the following:
1. Generate trees bottom-up: iterate `n = 1…N`, reusing previously computed catalogues.
2. For each `n`, enumerate partitions of `n-1` to describe the root’s child sizes.
3. For a given partition like [2,1], try every combination of the stored size-2 and size-1 trees and skip pairings with too many leaves. This step is done for every possible partition. Valid combinations are sorted and then stored in a set of unique trees in canonical form.
4. Returns an output vector of the unique trees generated.

The CLI will then print a summary of the generated trees, followed by all the trees in canonical form.

Example outputs can be found in the out/ directory.

Build steps
--------------------------------------------------
Requires g++
```
g++ -std=c++20 -O2 tree_generator_main.cc tree_generator.cc -o tree_gen
```

Running
--------------------------------------------------
```
# For N=30 M=3
./tree_gen 30 3
```

Performance
--------------------------------------------------
Execution time for N=30,M=3 (filtered stdout)
```
tim@LAPTOP-4QRN01E9:~/dev/vinci/vinci-cpp-trees$ time ./tree_gen 30 3 | grep -v '('
Generating trees for N=30 with leaf bound M=3
Final count for N=30, leaves <= 3: 13661
Listing canonical forms:

real    0m0.155s
user    0m0.147s
sys     0m0.012s
```

execution time with printing all trees:
```
real    0m0.385s
user    0m0.150s
sys     0m0.072s
```


I ran these profiling steps on WSL2 running Ubuntu22, the system has the following specs:
```
=== CPU ===
CPU(s):                               12
Model name:                           Intel(R) Core(TM) i7-10850H CPU @ 2.70GHz
Thread(s) per core:                   2
Core(s) per socket:                   6
NUMA node0 CPU(s):                    0-11
=== Memory ===
Total: 7.6Gi | Used: 1.8Gi | Free: 309Mi
```

For this take home assignmnet I think a runtime of ~150ms is okay. If we wanted to further optimize things I would do more advanced profiling with perf and valgrind to see where we can improve. 

Future improvements
--------------------------------------------------
- Better CLI. I kept this very simple, but it's not the most user friendly.
- Improved error handling in `tree_generator.cc`. Currently I'm relying on the CLI to make sure that we have valid inputs, but if we wanted to expose the GenerateTrees function more widely we would want to handle the possibility of bad inputs.
- Better testing. The current tests are barebones and do not use a testing framework for simplicity. For a real application we would want to integrate this with existing testing frameworks.