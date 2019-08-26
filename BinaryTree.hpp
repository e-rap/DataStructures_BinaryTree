#pragma once

#include <memory>
#include <utility>
#include <functional>
#include <deque>
#include <iostream>
#include <numeric>

template <typename ElementType>
class BinaryTree
{
public:
  class Node;
  using NodePtr = std::unique_ptr<Node>;

  virtual ~BinaryTree();
  Node* getRootPtr() const;
  void insert(const ElementType& value);

  static Node* search(const ElementType& to_find, Node* starting_node);
  template<typename ContainerType>
  static std::unique_ptr<BinaryTree> build(const ContainerType& elements, const size_t num_elements);

private:
  BinaryTree();
  NodePtr root_ptr;
  NodePtr& find_position(const ElementType& value);
};

template <typename ElementType>
class BinaryTree<ElementType>::Node
{
  using value_type = ElementType;
  friend class BinaryTree;

public:
  Node(ElementType value) : value(value) {}
  Node* getLeft() { return unique_left.get(); }
  Node* getRight() { return unique_right.get(); }
  ElementType getValue() const { return value; }
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
static Node<ElementType>* BinaryTree<ElementType>::search(const ElementType& to_find, Node* starting_node)
{
  auto retVal{ find_position(to_find) };
  return (retVal == nullptr) ? nullptr : retVal.get();
}

/* Private Function Definitions */

template <typename ElementType>
BinaryTree<ElementType>::BinaryTree() : root_ptr{ nullptr }
{
}

template <typename ElementType>
typename BinaryTree<ElementType>::NodePtr& BinaryTree<ElementType>::find_position(const ElementType& value)
{
  auto cur_node_ptr_ref{ std::ref(root_ptr) };
  while (cur_node_ptr_ref.get() != nullptr)
  {
    NodePtr& cur_node_ptr{ cur_node_ptr_ref.get() };
    if (value == cur_node_ptr->value)
    {
      break;
    }
    else if (value < cur_node_ptr->value)
    {
      cur_node_ptr_ref = std::ref(cur_node_ptr->unique_left);
      if (cur_node_ptr->getLeft() == nullptr)
      {
        break;
      }
    }
    else // (value > cur_node_ptr->value)
    {
      cur_node_ptr_ref = std::ref(cur_node_ptr->unique_right);
      if (cur_node_ptr->getRight() == nullptr)
      {
        break;
      }
    }
  }
  return cur_node_ptr_ref;
}

template <typename ElementType>
template <typename ContainerType>
static std::unique_ptr<BinaryTree<ElementType>> BinaryTree<ElementType>::build(const ContainerType& elements, const size_t num_elements)
{
  struct MkUniqueEnablr : public BinaryTree<ElementType> {};
  auto binary_tree_ptr{ std::make_unique<MkUniqueEnablr>() };
  for (size_t i{ 0 }; i < num_elements; i++)
  {
    binary_tree_ptr->insert(elements[i]);
  }
  return binary_tree_ptr;
}

/* Assignment API Functions */

template<typename ContainerType>
std::unique_ptr<BinaryTree<typename ContainerType::value_type>> CreateBST(const ContainerType& elements,
                                                                          const size_t num_elements)
{
  using ElementType = ContainerType::value_type;
  return std::move(BinaryTree<ElementType>::build(elements, num_elements));
}




namespace HelperFunctions
{
  template <typename Node, typename VisitFunction, typename... Args>
  void InOrder(Node node, VisitFunction visit_func, Args&& ... args)
  {
    if (node != nullptr)
    {
      InOrder(node->getLeft(), visit_func, std::forward<Args>(args)...);
      visit_func(node, std::forward<Args>(args)...);
      InOrder(node->getRight(), visit_func, std::forward<Args>(args)...);
    }
  }
} // namespace HelperFunctions

template <typename Node, typename VisitFunction, typename... Args>
void VisitInOrder(Node root, VisitFunction visit_func, Args&& ... args)
{
  HelperFunctions::InOrder(root, visit_func, std::forward<Args>(args)...);
}

template <typename ElementType>
ElementType SumLeafNodes(const BinaryTree<ElementType>& tree)
{
}

template <typename ElementType, typename Node>
ElementType SumLeafNodes(Node root)
{
  std::vector<ElementType> node_list{};
  auto leaf_node_list = [](Node node, std::vector<ElementType>& values)
  {
    if ((node->getLeft() == nullptr) && (node->getRight() == nullptr))
    {
      values.push_back(node->getValue());
    }
  };
  VisitInOrder(root, leaf_node_list, node_list);
  ElementType retVal{ std::accumulate(node_list.cbegin(), node_list.cend(), 0) };
  return retVal;
}

// TODO
template <typename ElementType>
Node<ElementType>* BSTSearch(
  const Node<ElementType>* root,
  const ElementType& to_find)
{
  return BinaryTree<ElementType>::search(to_find, root);
}

//Node *BSTSearch(const Node *root, const ElementType &to_find);