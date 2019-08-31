#pragma once

#include<iostream>
#include<vector>
#include"BinaryTree.hpp"


/**
 * Creates a binary search tree.
 *
 * @param[in] elements an stl container with the values for the binary tree.
 * @param[in] num_elements the number of elements to insert from the conatiner.
 */
template<typename ContainerType>
std::unique_ptr<BinaryTree::Node<typename ContainerType::value_type>> CreateBST(const ContainerType& elements,
                                                                                const size_t num_elements)
{
  using ElementType = ContainerType::value_type;
  auto first{ elements.cbegin() };
  auto last{ elements.cbegin() + num_elements };
  return BinaryTree::build(first, last);
}

/** Visits the binary tree in order and prints out the node values. */
template <typename ElementType>
void VisitInOrder(BinaryTree::Node<ElementType>* root)
{
  std::vector<ElementType> values{};
  auto node_list{ [](BinaryTree::Node<ElementType>* node, std::vector<ElementType>& values)
  {
    values.push_back(node->getValue());
  } };

  BinaryTree::InOrderTraversal(root, node_list, values);
  std::cout << "(";
  auto it{ values.cbegin() };
  while (it != values.cbegin() + values.size() - 1)
  {
    std::cout << *it << ", ";
    ++it;
  }
  std::cout << *it << ")\n";
}

/** Returns the sum of all leaf nodes in the binary tree. */
template <typename ElementType>
ElementType SumLeafNodes(BinaryTree::Node<ElementType>* root)
{
  std::vector<ElementType> values{};
  auto leaf_node_list{ [](BinaryTree::Node<ElementType>* node, std::vector<ElementType>& values)
  {
    if ((node->getLeft() == nullptr) && (node->getRight() == nullptr))
    {
      values.push_back(node->getValue());
    }
  } };
  BinaryTree::InOrderTraversal(root, leaf_node_list, values);
  return std::accumulate(values.cbegin(), values.cend(), ElementType{ 0 });
}

/**
 * Searches the binary tree for the value, to_find
 *
 * @param[in] root a raw pointer to the root of the binary tree
 * @param[in] to_find the value to find within the tree
 */
template <typename ElementType>
BinaryTree::Node<ElementType>* BSTSearch(
  const BinaryTree::Node<ElementType>* root,
  const ElementType& to_find)
{
  return BinaryTree::search(root, to_find);
}