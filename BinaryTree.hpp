#pragma once

#include <memory>
#include <utility>
#include <functional>
#include <deque>
#include <iostream>
#include <numeric>


/** Binary tree data stucture. */
template <typename ElementType>
class BinaryTree
{
public:
  /** Binary tree node class. */
  class Node;

  using NodePtr = std::unique_ptr<Node>;

  /** Destructor. */
  virtual ~BinaryTree();

  /** returns the root of the binary tree. */
  Node* getRootPtr() const;

  /** inserts value into the binary tree. */
  void insert(const ElementType& value);

  /**
   * searches the binary tree for a given value.
   *
   * @param[in] to_find value to search for within the tree.
   * @param[in] starting_node node to start searching from.
   */
  static Node* search(const ElementType& to_find, Node* starting_node);

  /**
   * builder function to construct a binary tree from a given stl container.
   *
   * @param[in] elements container of values used to construct the tree.
   * @param[in] num_elements the number of elements to use from the container.
   */
  template<typename ContainerType>
  static std::unique_ptr<BinaryTree> build(const ContainerType& elements, const size_t num_elements);

private:


  /** Constructor. */
  BinaryTree() = delete;

  /** Helper function to find the position of value within the binary tree. */
  NodePtr& find_position(const ElementType& value);

  NodePtr root_ptr; /**< pointer to the root node */
};

template <typename ElementType>
class BinaryTree<ElementType>::Node
{
  using value_type = ElementType;
  friend class BinaryTree;

public:
  /**
   * Node constructor.
   *
   * @param[in] value to initalize the node with.
   */
  Node(ElementType value) : value(value) {}

  /** returns a raw pointer to the left node. */
  Node* getLeft() { return unique_left.get(); }

  /** returns a raw pointer to the right node. */
  Node* getRight() { return unique_right.get(); }

  /** returns the value of the node. */
  ElementType getValue() const { return value; }

  /** sets the value of the node. */
  void* setValue(ElementType& value) { this->value = value; }

private:
  std::unique_ptr<Node> unique_left;  ///< pointer to the left node
  std::unique_ptr<Node> unique_right; ///< pointer to the right node
  ElementType value;                  ///< value of the node
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
    if (node_ptr == nullptr)
    {
      node_ptr = std::make_unique<Node>(value);
    }
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
  // TODO: talk to brandon about this
  // MkUniqueEnablr allows me to call the private binary tree constructor
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



// TODO: Make Non-Recursive InOrder Function
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

// TODO: finish class API
template <typename ElementType>
Node<ElementType>* BSTSearch(
  const Node<ElementType>* root,
  const ElementType& to_find)
{
  return BinaryTree<ElementType>::search(to_find, root);
}

//Node *BSTSearch(const Node *root, const ElementType &to_find);