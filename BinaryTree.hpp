#pragma once

#include <memory>
#include <functional>
#include <deque>
#include <iostream>

template <typename ElementType>
class BinaryTree
{
public:
  class Node;
  using NodePtr = std::unique_ptr<Node>;

  template <typename ContainerType>
  class Builder;

  virtual ~BinaryTree();
  Node* getRootPtr() const;
  void insert(const ElementType& value);
  Node* search(const ElementType& to_find, Node* starting_node = getRootPtr());

private:
  BinaryTree();
  NodePtr root_ptr;
  NodePtr& find_position(const ElementType& value);
};

template <typename ElementType>
class BinaryTree<ElementType>::Node
{
  friend class BinaryTree;

public:
  Node(ElementType value) : value(value) {}
  Node* getLeft() { return unique_left.get(); }
  Node* getRight() { return unique_right.get(); }
  Node* getValue() const { return value; }
  void* setValue(ElementType& value) { this->value = value; }

private:
  std::unique_ptr<Node> unique_left;
  std::unique_ptr<Node> unique_right;
  ElementType value;
};

/* Public Function Definitions */

template <typename ElementType>
using Node = typename BinaryTree<ElementType>::Node;

template <typename ElementType>
BinaryTree<ElementType>::~BinaryTree()
{
  root_ptr = nullptr;
}

template <typename ElementType>
Node<ElementType>* BinaryTree<ElementType>::getRootPtr() const
{
  return root_ptr.get();
}

template <typename ElementType>
void BinaryTree<ElementType>::insert(const ElementType& value)
{
  if (root_ptr == nullptr)
  {
    root_ptr = std::make_unique<Node>(value);
  }
  else
  {
    NodePtr& node_ptr{ find_position(value) };
    node_ptr = std::make_unique<Node>(value);
  }
}

template <typename ElementType>
Node<ElementType>* BinaryTree<ElementType>::search(const ElementType& to_find, Node* starting_node)
{
  auto retVal{ find_position(to_find) };
  return (retVal == nullptr) ? nullptr : retVal.get();
}

/* Private Function Definitions */

template <typename ElementType>
BinaryTree<ElementType>::BinaryTree() : root_ptr{}
{
}

template <typename ElementType>
typename BinaryTree<ElementType>::NodePtr& BinaryTree<ElementType>::find_position(const ElementType& value)
{
  auto cur_node_ptr_ref{ std::ref(root_ptr) };
  NodePtr& cur_node_ptr{ cur_node_ptr_ref };

  while ((cur_node_ptr->left != nullptr) &&
    (cur_node_ptr->right != nullptr))
  {
    if (value == cur_node_ptr->value)
    {
      break;
    }
    else if (value < cur_node_ptr->value)
    {
      cur_node_ptr_ref = std::ref(cur_node_ptr->left);
      if (cur_node_ptr->left == nullptr)
      {
        break;
      }
    }
    else // (value > cur_node_ptr->value)
    {
      cur_node_ptr_ref = std::ref(cur_node_ptr->right);
      if (cur_node_ptr->right == nullptr)
      {
        break;
      }
    }
  }
  return cur_node_ptr_ref;
}

template <typename ContainerType>
class Builder
{
public:
  using ElementType = typename ContainerType::value_type;
  static BinaryTree<ElementType> CreateBST(const ContainerType& elements,
                                           const size_t num_elements);
};

template <typename ElementType>
template <typename ContainerType>
class BinaryTree<ElementType>::Builder
{
  static std::unique_ptr<BinaryTree> build(const ContainerType& elements, const size_t num_elements)
  {
    auto binary_tree_ptr = std::make_unique<BinaryTree>();
    for (int i{ 0 }; i < num_elements; i++)
    {
      binary_tree_ptr->insert(elements[i]);
    }
    return binary_tree_ptr;
  }
};

/* Assignment API Functoins */
template <typename ContainerType>
BinaryTree<typename ContainerType::value_type> CreateBST(const ContainerType& elements,
                                                         const size_t num_elements)
{
  return BinaryTree<typename ContainerType::value_type>::Builder::build(elements, num_elements);
}

template <typename ElementType>
Node<ElementType>* BSTSearch(
  const BinaryTree<ElementType>& tree,
  const ElementType& to_find)
{
  return tree.search(to_find);
}

namespace HelperFunctions
{
  template <typename ElementType, typename VisitFunction>
  void InOrder(const Node<ElementType>* node, VisitFunction func)
  {
    if (node != nullptr)
    {
      InOrder(node->getLeft());
      func();
      InOrder(node->getRight());
    }
  }
} // namespace HelperFunctions

template <typename ElementType, typename VisitFunction = void()>
void VisitInOrder(const Node<ElementType> * root)
{
  HelperFunctions::InOrder(root);
}

template <typename ElementType>
ElementType SumLeafNodes(const BinaryTree<ElementType>& tree)
{
}

// TODO
//Node *BSTSearch(const Node *root, const ElementType &to_find);
//ElementType SumLeafNodes(const BinaryTree *btree);
