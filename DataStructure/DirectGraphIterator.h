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
  /// ���map����node��slot��Ϊtrue˵���Ѿ�������ɣ�false˵�����ڱ�����
  /// ���û��˵����Ӧ��slot˵����û��������
  map<pointer, bool> visited_;

  /// @brief ��ȡ��һ�������������һ�����
  inline void getNext() {
    if (node_stack_.size() == 0)
      throw(exception("out of range"));

  ITERATOR_SUCC:
    stack_ele &ele = node_stack_.top();

    if (visited_[ele.first] == false) {
      node_iterator &child_it = ele.second;
      pointer node = ele.first;

      /// ���ջ���Ľڵ��̻�δ������ϣ����Ǿ�ֱ�ӱ���ջ���ڵ�ĺ�̽ڵ�
      while (child_it != node_trait::succ_end(node)) {
        pointer child_node = *child_it++;
        if (visited_.count(child_node) == 0) {
          /// �ҵ������û�б������Ľڵ㣬����ջ�еĵ���������������ڵ�
          node_stack_.push(
              stack_ele(child_node, node_trait::succ_begin(child_node)));
          visited_[child_node] = false;
          return;
        }
      }

      /// ��ǰ����Ѿ����������
      visited_[ele.first] = true;
    }

    /// ���������Ѿ�dfs��ɵĽڵ�
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

    /// ����ͼ�Ѿ����������
    if (node_stack_.size() == 0)
      return;

    goto ITERATOR_SUCC;
  }

public:
  explicit DirectGraphPreOrderIterator(pointer node) {
    node_stack_.push(stack_ele(node, node_trait::succ_begin(node)));
    visited_[node] = false;
  }

  /// @brief �յ�����������end()
  explicit DirectGraphPreOrderIterator() = default;

  /// @brief ǰ����
  DirectGraphPreOrderIterator &operator++() {
    getNext();
    return *this;
  }

  /// @brief ������
  const DirectGraphPreOrderIterator operator++(int) {
    DirectGraphPreOrderIterator tmp = *this;
    ++*this;
    return tmp;
  }

  /// @brief ���
  bool operator==(const DirectGraphPreOrderIterator &another) const {
    return another.node_stack_ == node_stack_;
  }

  /// @brief ������
  bool operator!=(const DirectGraphPreOrderIterator &another) const {
    return !(*this == another);
  }

  /// @brief ȡ����ǰ��������
  reference operator*() const { return *node_stack_.top().first; }

  /// @brief ȡ����ǰ����ָ��
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
  /// ���map����node��slot��Ϊtrue˵���Ѿ�������ɣ�false˵�����ڱ�����
  /// ���û��˵����Ӧ��slot˵����û��������
  map<pointer, bool> visited_;

  /// @brief ��ȡ��һ�������������һ�����
  inline void getNext() {
    if (node_stack_.size() == 0)
      throw(exception("out of range"));

  ITERATOR_SUCC:
    stack_ele ele = node_stack_.top();

    /// ��ǰ��㻹û������
    if (visited_[ele.first] == false) {
      node_iterator iter = ++ele.second;
      pointer node = ele.first;

      /// ���ѭ������Ҫ�ҵ���һ��û�пɷ����ӽڵ��node
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
        /// ���һֱ�����һ����̽�㶼���Ѿ�������״̬���ǵ�ǰ���ͱ��������
        /// ֱ�ӷ��ص�ǰ���
        if (it == node_trait::succ_end(node)) {
          node_stack_.emplace(node, node_trait::succ_end(node));
          visited_[node] = true;
          return;
        }
        node = new_node;
      }
    }

    /// ջ������Ѿ���ȫ���������Է�����һ����
    node_stack_.pop();

    if (node_stack_.size() == 0)
      return;

    /// Ѱ����һ������һ��������
    goto ITERATOR_SUCC;
  }

public:
  explicit DirectGraphPostOrderIterator(pointer node) {
    /// �ҵ�����ĵ�һ�����
    do {
      if (visited_.count(node) != 0)
        break;
      node_stack_.emplace(node, node_trait::succ_begin(node));
      visited_[node] = false;
      /// ���һ��ʼ�ͳ��ֻ�·����ʱ����begin�Ѿ���������
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

  /// @brief �յ�����������end()
  explicit DirectGraphPostOrderIterator() = default;

  /// @brief ǰ����
  DirectGraphPostOrderIterator &operator++() {
    getNext();
    return *this;
  }

  /// @brief ������
  const DirectGraphPostOrderIterator operator++(int) {
    DirectGraphPreOrderIterator tmp = *this;
    ++*this;
    return tmp;
  }

  /// @brief ���
  bool operator==(const DirectGraphPostOrderIterator &another) {
    return another.node_stack_ == node_stack_;
  }

  /// @brief ������
  bool operator!=(const DirectGraphPostOrderIterator &another) {
    return !(*this == another);
  }

  /// @brief ȡ����ǰ��������
  reference operator*() const { return *node_stack_.top().first; }

  /// @brief ȡ����ǰ����ָ��
  pointer operator->() const { return node_stack_.top().first; }
};

/// reverse post order
} // namespace adt