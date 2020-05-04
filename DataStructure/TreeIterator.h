#pragma once

#include "Stack.h"

namespace adt {

template <typename TreeTy> struct TreeTraits {
  /// using node_type = Node;
  /// using node_pointer_type = Node *;
  /// using value_type = int;
  /// using reference = int&;
  /// using pointer = int*;
  /// node_pointer_type GetNextSlibing(node_pointer_type);
  /// node_pointer_type GetPrevSlibing(node_pointer_type);
  /// node_pointer_type GetLeftChild(node_pointer_type);
  /// node_pointer_type GetRightChild(node_pointer_type);
  /// node_pointer_type GetParent(node_pointer_type);
  /// unsigned GetNumOfChildren(node_pointer_type);
  /// node_pointer_type GetChildren(node_pointer_type, unsigned);
  /// reference GetValue(node_pointer_type);
};

/// @brief:�������������
/// @note:traits����ʵ��GetNextSlibing��GetPrevSlibing��GetParent��GetNumOfChildren��GetChildren��GetValue
template <typename TreeTy, bool Const> class TreePreOrderIterator {
public:
  using traits = TreeTraits<TreeTy>;
  using iterator_category = std::bidirectional_iterator_tag;
  using node = typename traits::node_type;
  using node_ptr = typename traits::node_pointer_type;
  using value_type = typename traits::value_type;
  using reference =
      typename std::conditional<Const, typename traits::reference,
                                const typename traits::reference>::type;
  using pointer =
      typename std::conditional<Const, typename traits::pointer,
                                const typename traits::pointer>::type;

public:
  TreePreOrderIterator() : node_(nullptr) {}

  TreePreOrderIterator(node_ptr node) : node_(node) {}

  /// @brief �ҵ�ǰ���������һ�����
  TreePreOrderIterator &operator++() {
    if (!node_) {
      throw(std::exception("TreePreOrderIterator:out of range"));
    }

    node_ptr next_node;
    if (GetNumOfChildren(node_) != 0) {
      node_ = GetChildren(node_, 0);
      return *this;
    }

    while (GetNextSlibing(node_) == nullptr) {
      node_ = GetParent(node_);
    }
    if (node_ != nullptr) {
      node_ = GetNextSlibing(node_);
    }
    return *this;
  }

  const TreePreOrderIterator operator++(int) {
    TreePreOrderIterator iter = *this;
    ++*this;
    return iter;
  }

  /// @brief �ҵ�ǰ���������һ�����
  TreePreOrderIterator &operator--() {
    if (!node_) {
      throw(std::exception("TreePreOrderIterator:out of range"));
    }

    if (GetPrevSlibing(node_) == nullptr) {
      node_ = GetParent(node_);
      return *this;
    }

    node_ = GetPrevSlibing(node_);
    while (size_t child_count = GetNumOfChildren(node_)) {
      node_ = GetChildren(node_, child_count - 1);
    }
    return *this;
  }

  const TreePreOrderIterator operator--(int) {
    TreePreOrderIterator iter = *this;
    --*this;
    return iter;
  }

  reference operator*() const { return GetValue(node_); }

  pointer operator->() const { return &GetValue(node_); }

  bool operator!=(const TreePreOrderIterator &Another) const {
    return node_ != Another.node_;
  }

  bool operator==(const TreePreOrderIterator &Another) const {
    return node_ == Another.node_;
  }

  node_ptr data() const { return node_; }

private:
  node_ptr GetNextSlibing(node_ptr Node) const {
    return traits::GetNextSlibing(Node);
  }

  node_ptr GetPrevSlibing(node_ptr Node) const {
    return traits::GetPrevSlibing(Node);
  }

  node_ptr GetParent(node_ptr Node) const { return traits::GetParent(Node); }

  unsigned GetNumOfChildren(node_ptr Node) const {
    return traits::GetNumOfChildren(Node);
  }

  node_ptr GetChildren(node_ptr Node, unsigned Idx) const {
    return traits::GetChildren(Node, Idx);
  }

  reference GetValue(node_ptr Node) const { return traits::GetValue(Node); }

private:
  node_ptr node_;
};

/// @brief:�������������
/// @note:traits����ʵ��GetParent��GetValue��GetRightChild��GetLeftChild
template <typename TreeTy, bool Const> class TreeInOrderIterator {
public:
  using traits = TreeTraits<TreeTy>;
  using iterator_category = std::bidirectional_iterator_tag;
  using node = typename traits::node_type;
  using node_ptr = typename traits::node_pointer_type;
  using value_type = typename traits::value_type;
  using reference =
      typename std::conditional<Const, const typename traits::reference,
                                typename traits::reference>::type;
  using pointer =
      typename std::conditional<Const, const typename traits::pointer,
                                typename traits::pointer>::type;

public:
  TreeInOrderIterator() : node_(nullptr) {}

  TreeInOrderIterator(node_ptr node) : node_(node) {}

  /// @brief �ҵ������������һ�����
  TreeInOrderIterator &operator++() {
    if (!node_) {
      throw(std::exception("TreeInOrderIterator:out of range"));
    }

    /// ��������ӽڵ㣬���������ӽڵ������ߵ�(Ҷ)�ڵ�
    if (node_ptr child = GetRightChild(node_)) {
      node_ = child;
      while (child = GetLeftChild(node_))
        node_ = child;
      return *this;
    }

    /// �ҵ���һ�����丸�������ӽڵ�Ľڵ�
    node_ptr parent = GetParent(node_);
    while (parent != nullptr && node_ == GetRightChild(parent)) {
      node_ = parent;
      parent = GetParent(node_);
    }
    node_ = parent;
    return *this;
  }

  const TreeInOrderIterator operator++(int) {
    TreeInOrderIterator iter = *this;
    ++*this;
    return iter;
  }

  /// @brief �ҵ������������һ�����
  TreeInOrderIterator &operator--() {
    if (!node_) {
      throw(std::exception("TreeInOrderIterator:out of range"));
    }

    /// ��������ӽڵ㣬���������ӽڵ�����ұߵ�(Ҷ)�ڵ�
    if (node_ptr child = GetLeftChild(node_)) {
      node_ = child;
      while (child = GetRightChild(node_))
        node_ = child;
      return *this;
    }

    /// �ҵ���һ�����丸�������ӽڵ�Ľڵ�
    node_ptr parent = GetParent(node_);
    while (parent != nullptr && node_ == GetLeftChild(parent)) {
      node_ = parent;
      parent = GetParent(node_);
    }
    node_ = parent;
    return *this;
  }

  const TreeInOrderIterator operator--(int) {
    TreeInOrderIterator iter = *this;
    --*this;
    return iter;
  }

  reference operator*() const { return GetValue(node_); }

  pointer operator->() const { return &GetValue(node_); }

  bool operator!=(const TreeInOrderIterator &Another) const {
    return node_ != Another.node_;
  }

  bool operator==(const TreeInOrderIterator &Another) const {
    return node_ == Another.node_;
  }

  node_ptr data() const { return node_; }

  TreeInOrderIterator<TreeTy, false> _Remove_Const() {
    return TreeInOrderIterator<TreeTy, false>(node_);
  }

private:
  node_ptr GetParent(node_ptr Node) const { return traits::GetParent(Node); }

  reference GetValue(node_ptr Node) const { return traits::GetValue(Node); }

  node_ptr GetLeftChild(node_ptr Node) const {
    return traits::GetLeftChild(Node);
  }

  node_ptr GetRightChild(node_ptr Node) const {
    return traits::GetRightChild(Node);
  }

private:
  node_ptr node_;
};
} // namespace adt