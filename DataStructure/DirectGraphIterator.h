#pragma once

#include "Stack.h"
#include <map>

namespace adt {
using namespace std;

template <typename NodeTy> class DirectGraphNodeTrait {
public:
  /// using node_ref = Node*;
  /// using node_iterator = Iterator(Node);
  /// node_iterator succ_begin(node_ref);
  /// node_iterator succ_end(node_ref);
  /// size_t succ_size(node_ref);
  /// node_iterator prev_begin(node_ref);
  /// node_iterator prev_end(node_ref);
};

template <typename NodeTy> class ReverseDirectGraphNodeTrait {
public:
  /// using node_ref = Node*;
  /// using node_iterator = Iterator(Node);
  /// node_iterator succ_begin(node_ref);
  /// node_iterator succ_end(node_ref);
  /// size_t succ_size(node_ref);
  /// node_iterator prev_begin(node_ref);
  /// node_iterator prev_end(node_ref);
};

template <typename NodeTy, typename node_trait = DirectGraphNodeTrait<NodeTy>>
class DirectGraphPreOrderIterator {
private:
  using iterator_category = std::forward_iterator_tag;
  using value_type = NodeTy;
  using reference = NodeTy &;
  using pointer = NodeTy *;

  using node_iterator = typename node_trait::node_iterator;
  using stack_ele = pair<pointer, node_iterator>;

  Stack<stack_ele> node_stack_;
  /// 如果map中有node的slot，为true说明已经遍历完成，false说明正在被遍历
  /// 如果没有说明对应的slot说明还没被遍历到
  map<pointer, bool> visited_;

  /// @brief 获取下一个先序遍历的下一个结点
  inline void getNext() {
    if (node_stack_.size() == 0)
      throw(exception("out of range"));

  ITERATOR_SUCC:
    stack_ele &ele = node_stack_.top();

    if (visited_[ele.first] == false) {
      node_iterator &child_it = ele.second;
      pointer node = ele.first;

      /// 如果栈顶的节点后继还未遍历完毕，我们就直接遍历栈顶节点的后继节点
      while (child_it != node_trait::succ_end(node)) {
        pointer child_node = *child_it++;
        if (visited_.count(child_node) == 0) {
          /// 找到后继中没有被遍历的节点，更新栈中的迭代器并返回这个节点
          node_stack_.push(
              stack_ele(child_node, node_trait::succ_begin(child_node)));
          visited_[child_node] = false;
          return;
        }
      }

      /// 当前结点已经遍历完成了
      visited_[ele.first] = true;
    }

    /// 弹出所有已经dfs完成的节点
    while (node_stack_.size()) {
      pointer node = node_stack_.top().first;
      node_iterator child_it = node_stack_.top().second;
      if (child_it == node_trait::succ_end(node)) {
        visited_[node] = true;
        node_stack_.pop();
        continue;
      }
      break;
    }

    /// 整个图已经被遍历完成
    if (node_stack_.size() == 0)
      return;

    goto ITERATOR_SUCC;
  }

public:
  explicit DirectGraphPreOrderIterator(pointer node) {
    node_stack_.push(stack_ele(node, node_trait::succ_begin(node)));
    visited_[node] = false;
  }

  /// @brief 空迭代器，用作end()
  explicit DirectGraphPreOrderIterator() = default;

  /// @brief 前自增
  DirectGraphPreOrderIterator &operator++() {
    getNext();
    return *this;
  }

  /// @brief 后自增
  const DirectGraphPreOrderIterator operator++(int) {
    DirectGraphPreOrderIterator tmp = *this;
    ++*this;
    return tmp;
  }

  /// @brief 相等
  bool operator==(const DirectGraphPreOrderIterator &another) const {
    return another.node_stack_ == node_stack_;
  }

  /// @brief 不等于
  bool operator!=(const DirectGraphPreOrderIterator &another) const {
    return !(*this == another);
  }

  /// @brief 取出当前结点的引用
  reference operator*() const { return *node_stack_.top().first; }

  /// @brief 取出当前结点的指针
  pointer operator->() const { return node_stack_.top().first; }
};

template <typename NodeTy, typename node_trait = DirectGraphNodeTrait<NodeTy>>
class DirectGraphPostOrderIterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = NodeTy;
  using reference = NodeTy &;
  using pointer = NodeTy *;

  using node_iterator = typename node_trait::node_iterator;
  using stack_ele = pair<pointer, node_iterator>;

  Stack<stack_ele> node_stack_;
  /// 如果map中有node的slot，为true说明已经遍历完成，false说明正在被遍历
  /// 如果没有说明对应的slot说明还没被遍历到
  map<pointer, bool> visited_;

  /// @brief 获取下一个后序遍历的下一个结点
  inline void getNext() {
    if (node_stack_.size() == 0)
      throw(exception("out of range"));

  ITERATOR_SUCC:
    stack_ele ele = node_stack_.top();

    /// 当前结点还没处理完
    if (visited_[ele.first] == false) {
      node_iterator iter = ++ele.second;
      pointer node = ele.first;

      /// 这个循环中需要找到第一个没有可访问子节点的node
      while (1) {
        node_iterator it = iter;
        pointer new_node = nullptr;

        for (; it != node_trait::succ_end(node); ++it) {
          if (visited_.count(*it) != 0)
            continue;
          if (node == ele.first)
            node_stack_.pop();
          node_stack_.emplace(node, it);
          visited_[node] = false;
          new_node = *it;
          iter = node_trait::succ_begin(new_node);
          break;
        }
        /// 如果一直到最后一个后继结点都是已经遍历的状态，那当前结点就遍历完成了
        /// 直接返回当前结点
        if (it == node_trait::succ_end(node)) {
          node_stack_.emplace(node, node_trait::succ_end(node));
          visited_[node] = true;
          return;
        }
        node = new_node;
      }
    }

    /// 栈顶结点已经完全遍历，可以返回上一级了
    node_stack_.pop();

    if (node_stack_.size() == 0)
      return;

    /// 寻找上一级的下一个后序结点
    goto ITERATOR_SUCC;
  }

public:
  explicit DirectGraphPostOrderIterator(pointer node) {
    /// 找到后序的第一个结点
    do {
      if (visited_.count(node) != 0)
        break;
      node_stack_.emplace(node, node_trait::succ_begin(node));
      visited_[node] = false;
      /// 如果一开始就出现环路，这时可能begin已经被访问了
      node_iterator it = node_trait::succ_begin(node);
      for (; it != node_trait::succ_end(node); ++it) {
        if (visited_.count(*it))
          continue;
        break;
      }
      if (it == node_trait::succ_end(node)) {
        visited_[node] = true;
        break;
      }
      node = *it;
    } while (1);
  }

  /// @brief 空迭代器，用作end()
  explicit DirectGraphPostOrderIterator() = default;

  /// @brief 前自增
  DirectGraphPostOrderIterator &operator++() {
    getNext();
    return *this;
  }

  /// @brief 后自增
  const DirectGraphPostOrderIterator operator++(int) {
    DirectGraphPreOrderIterator tmp = *this;
    ++*this;
    return tmp;
  }

  /// @brief 相等
  bool operator==(const DirectGraphPostOrderIterator &another) {
    return another.node_stack_ == node_stack_;
  }

  /// @brief 不等于
  bool operator!=(const DirectGraphPostOrderIterator &another) {
    return !(*this == another);
  }

  /// @brief 取出当前结点的引用
  reference operator*() const { return *node_stack_.top().first; }

  /// @brief 取出当前结点的指针
  pointer operator->() const { return node_stack_.top().first; }
};

/// reverse post order
} // namespace adt