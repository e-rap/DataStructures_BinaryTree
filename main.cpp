#include <iostream>
#include <vector>
#include "BinaryTree.hpp"

template<typename DataType>
void VisitFunction(Node<DataType>* node)
{
  std::cout << node->getValue();
}

int main()
{
  using ElementType = int32_t;
  using ContainerType = std::vector<ElementType>;
  std::vector<ElementType> test_input{ 4, 2, 6, 3 ,1, 5, 7 };
  auto binary_tree = CreateBST(test_input, test_input.size());

  std::vector<ElementType> values{};
  auto node_list = [](Node<ElementType>* node, std::vector<ElementType>& values) {values.push_back(node->getValue()); };

  VisitInOrder(binary_tree->getRootPtr(), node_list, values);

  for (auto val : values)
  {
    std::cout << val << " ";
  }


  auto leaf_node_sum{ SumLeafNodes<ElementType>(binary_tree->getRootPtr()) };

  return 0;
}