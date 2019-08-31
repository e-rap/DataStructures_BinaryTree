#include <iostream>
#include <vector>
#include "AssignmentFunctions.h"
#include "BinaryTree.hpp"


int main()
{
  using ElementType = int32_t;
  using ContainerType = std::vector<ElementType>;

  ContainerType test_input{ 4, 2, 6, 3 ,1, 5, 7 };
  auto unique_root_ptr = CreateBST(test_input, test_input.size());
  auto root_ptr = unique_root_ptr.get();

  std::cout << "Binary tree in order traversal: ";
  VisitInOrder(root_ptr);

  std::cout << "Sum of leaf nodes: " << SumLeafNodes(root_ptr) << "\n";

  return 0;
}