#pragma once

#include <memory>
#include <utility>
#include <functional>
#include <numeric>
#include <exception>


/** Binary tree data stucture. */
class BinaryTree
{
public:
  BinaryTree() = delete;
  ~BinaryTree() = delete;

  /** Binary tree node class. */
  template <typename ElementType>
  class Node;

  template <typename ElementType>
  using NodePtr = std::unique_ptr<Node<ElementType>>;

  /** inserts value into the binary tree. */
  template<typename ElementType>
  static void insert(Node<ElementType>* root, const ElementType& value);

  /**
   * searches the binary tree for a given value.
   *
   * @param[in] root node to start searching from.
   * @param[in] to_find value to search for within the tree.
   * @returns if found returns pointer to node containing value to_find, else nullptr.
   */
  template<typename ElementType>
  static Node<ElementType>* search(Node<ElementType>* root, const typename ElementType& to_find);

  /**
   * builds a binary tree structure of nodes from a given STL container of elements
   *
   * @param[in] container of values used to construct the tree.
   * @param[in] num_elements the number of elements to use from the container.
   * @returns a std::unique_ptr to the root node of the tree
   */
  template<typename InputIt>
  static std::unique_ptr<Node<typename InputIt::value_type>> build(const InputIt first, const InputIt last);

  /**
   * In order traversal of the binary tree data structure.
   *
   * @param[in] root root of the binary tree.
   * @param[in] visit_func the function executed when visiting a node.
   * @prama[in] args arguments for the function call.
   */
  template <typename Node_Ptr, typename VisitFunction, typename... Args>
  static void InOrderTraversal(Node_Ptr root, VisitFunction visit_func, Args&& ... args);

private:

  /**
   * Helper function to find the node positoin for the the value within the binary tree.
   *
   * @returns a reference to the node.
   */
  template<typename ElementType>
  static Node<ElementType>* find_position(Node<ElementType>* root, const ElementType& value);

  /**
   * Inner helper function for recursive InOrder Traversal
   */
  template <typename Node_Ptr, typename VisitFunction, typename... Args>
  static void InOrder(Node_Ptr node, VisitFunction visit_func, Args&& ... args);
};

template <typename ElementType>
class BinaryTree::Node
{
  using value_type = ElementType;
  friend BinaryTree;

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


template <typename ElementType>
void BinaryTree::insert(Node<ElementType>* root, const ElementType& value)
{
  if (root != nullptr)
  {
    Node<ElementType>* node_ptr{ find_position(root, value) };
    if (node_ptr != nullptr)
    {
      if (value < node_ptr->getValue())
      {
        node_ptr->unique_left = std::make_unique<Node<ElementType>>(value);
      }
      else
      {
        node_ptr->unique_right = std::make_unique<Node<ElementType>>(value);
      }
    }
    else
    {
      // Node with value exists do nothing
    }
  }
  else
  {
    throw std::exception("Error: input root is not initialized");
  }
}

template <typename ElementType>
BinaryTree::Node<ElementType>* BinaryTree::search(Node<ElementType>* root, const typename ElementType& to_find)
{
  auto retVal{ find_position(root, to_find) };
  return (retVal == nullptr) ? nullptr : retVal;
}

template <typename InputIt>
static std::unique_ptr<BinaryTree::Node<typename InputIt::value_type>> BinaryTree::build(const InputIt first, const InputIt last)
{
  using ElementType = typename InputIt::value_type;
  auto iterator{ first };

  // insert first value into the root node
  auto root_ptr{ std::make_unique<BinaryTree::Node<ElementType>>(*iterator) };

  // fill in the rest of the values
  while (iterator != last)
  {
    BinaryTree::insert<ElementType>(root_ptr.get(), *iterator);
    ++iterator;
  }
  return root_ptr;
}

template<typename Node_Ptr, typename VisitFunction, typename ...Args>
void BinaryTree::InOrderTraversal(Node_Ptr root, VisitFunction visit_func, Args&& ...args)
{
  InOrder(root, visit_func, std::forward<Args>(args)...);
}

/* Private Function Definitions */

template <typename ElementType>
typename BinaryTree::Node<ElementType>* BinaryTree::find_position(Node<ElementType>* root, const ElementType& value)
{
  auto cur_node_ptr{ root };
  auto return_ptr{ root };
  while (cur_node_ptr != nullptr)
  {
    return_ptr = cur_node_ptr;
    if (value == cur_node_ptr->value)
    {
      break;
    }
    else if (value < cur_node_ptr->value)
    {
      cur_node_ptr = cur_node_ptr->getLeft();
    }
    else // (value > cur_node_ptr->value)
    {
      cur_node_ptr = cur_node_ptr->getRight();
    }
  }
  return return_ptr;
}

template <typename Node_Ptr, typename VisitFunction, typename... Args>
void BinaryTree::InOrder(Node_Ptr node, VisitFunction visit_func, Args&& ... args)
{
  if (node != nullptr)
  {
    InOrder(node->getLeft(), visit_func, std::forward<Args>(args)...);
    visit_func(node, std::forward<Args>(args)...);
    InOrder(node->getRight(), visit_func, std::forward<Args>(args)...);
  }
}