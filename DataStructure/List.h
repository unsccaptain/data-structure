#pragma once

#include "Allocator.h"
#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>

namespace adt {

template <typename Ty> struct ListNode {
  ListNode *Prev;
  ListNode *Next;
  Ty Value;

  explicit ListNode(const Ty &Ele) : Value(Ele) {}

  explicit ListNode(Ty &&Ele) : Value(std::move(Ele)) {}

  template <typename... ValTy>
  explicit ListNode(ValTy &&... V) : Value(std::forward<ValTy>(V)...) {}

  ~ListNode() { Value.~Ty(); }
};

template <typename Ty> class ListConstIterator {
public:
  using node_ptr = ListNode<Ty> *;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Ty;
  using pointer = const Ty *;
  using reference = const Ty &;
  using difference_type = int;

public:
  ListConstIterator() : node_(nullptr) {}

  ListConstIterator(node_ptr Ptr) : node_(Ptr) {}

  ListConstIterator &operator++() {
    node_ = node_->Next;
    return *this;
  }

  ListConstIterator &operator--() {
    node_ = node_->Prev;
    return *this;
  }

  const ListConstIterator operator++(int) {
    ListConstIterator it = *this;
    ++*this;
    return it;
  }

  const ListConstIterator operator--(int) {
    ListConstIterator it = *this;
    --*this;
    return it;
  }

  reference operator*() const { return (reference)node_->Value; }

  pointer operator->() const { return (pointer)&node_->Value; }

  bool operator!=(const ListConstIterator &another) { return !IsSame(another); }

  bool operator==(const ListConstIterator &another) { return IsSame(another); }

  node_ptr data() { return node_; }

private:
  bool IsSame(const ListConstIterator &another) {
    return node_ == another.node_;
  }

private:
  node_ptr node_;
};

template <typename Ty> class ListIterator {
public:
  using node_ptr = ListNode<Ty> *;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Ty;
  using pointer = Ty *;
  using reference = Ty &;
  using const_reference = const Ty &;
  using difference_type = int;

public:
  ListIterator(node_ptr Ptr) : node_(Ptr) {}

  ListIterator &operator++() {
    node_ = node_->Next;
    return *this;
  }

  ListIterator &operator--() {
    node_ = node_->Prev;
    return *this;
  }

  const ListIterator operator++(int) {
    ListIterator it = *this;
    ++*this;
    return it;
  }

  const ListIterator operator--(int) {
    ListIterator it = *this;
    --*this;
    return it;
  }

  reference operator*() { return node_->Value; }

  const_reference operator*() const { return (const_reference)node_->Value; }

  pointer operator->() { return &node_->Value; }

  bool operator!=(const ListIterator &another) { return !IsSame(another); }

  bool operator==(const ListIterator &another) { return IsSame(another); }

  node_ptr data() { return node_; }

private:
  bool IsSame(const ListIterator &another) { return node_ == another.node_; }

private:
  node_ptr node_;
};

template <typename Ty, typename AllocatorTy = Allocator> class List {
public:
  using node = ListNode<Ty>;
  using node_ptr = ListNode<Ty> *;
  using reference = Ty &;
  using const_reference = const Ty &;
  using value_type = Ty;
  using iterator = ListIterator<Ty>;
  using const_iterator = ListConstIterator<Ty>;

  using al_ = AllocatorTy;

public:
  explicit List() : head_(Ty()) { head_.Next = head_.Prev = &head_; }

  explicit List(const_reference Element) : head_(Ty()) {
    head_.Next = head_.Prev = &head_;
  }

  explicit List(const List &another) {
    head_.Next = head_.Prev = &head_;
    insert(end(), another.begin(), another.end());
  }

  explicit List(List &&another) {
    if (another.size() == 0) {
      head_.Next = head_.Prev = &head_;
      return;
    }
    node_ptr headA = another.head_.Next;
    node_ptr tailA = another.head_.Prev;
    headA->Prev = &head_;
    tailA->Next = &head_;
    head_.Next = headA;
    head_.Prev = tailA;

    size_ = another.size_;
  }

  void push_back(const_reference Element) {
    InsertTail(Element);
    ++size_;
  }

  void push_back(Ty &&Element) {
    InsertTail(Element);
    ++size_;
  }

  template <typename... ValTy> void emplace_back(ValTy &&... Element) {
    InsertTail(std::forward<ValTy>(Element)...);
    ++size_;
  }

  void push_front(const_reference Element) {
    InsertHead(Element);
    ++size_;
  }

  void push_front(Ty &&Element) {
    InsertHead(Element);
    ++size_;
  }

  template <typename... ValTy> void emplace_front(ValTy &&... Element) {
    InsertHead(std::forward<ValTy>(Element)...);
    ++size_;
  }

  template <typename... ValTy>
  iterator emplace(iterator Where, ValTy &&... Element) {
    iterator pos = InsertAfter(Where.data(), std::forward<ValTy>(Element)...);
    ++size_;
    return pos;
  }

  void pop_front() {
    RemoveHead();
    --size_;
  }
  void pop_back() {
    RemoveTail();
    --size_;
  }

  reference front() {
    assert(size_ > 0);
    return head_.Next->Value;
  }

  const_reference front() const {
    assert(size_ > 0);
    return head_.Next->Value;
  }

  reference back() {
    assert(size_ > 0);
    return head_.Prev->Value;
  }

  const_reference back() const {
    assert(size_ > 0);
    return head_.Prev->Value;
  }

  iterator begin() {
    assert(head_.Next != &head_);
    return iterator(head_.Next);
  }

  const_iterator begin() const {
    assert(head_.Next != &head_);
    return const_iterator(head_.Next);
  }

  iterator end() { return iterator(GetHead()); }

  const_iterator end() const { return const_iterator((const node_ptr)&head_); }

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  template <typename Iterator>
  void insert(iterator Where, Iterator First, Iterator Last) {
    iterator cur = Where;
    for (Iterator iter = First; iter != Last; ++iter) {
      cur = emplace(cur, *iter);
    }
  }

  iterator insert(iterator Where, const Ty &Val) { return emplace(Where, Val); }

  iterator insert(iterator Where, size_t Count, const Ty &Val) {
    iterator cur = Where;
    for (size_t i = 0; i < Count; ++i) {
      cur = emplace(cur, Val);
    }
    return cur;
  }

  iterator insert(iterator Where, size_t Count, Ty &&Val) {
    iterator cur = Where;
    for (size_t i = 0; i < Count; ++i) {
      cur = emplace(cur, Val);
    }
    return cur;
  }

  iterator insert(iterator Where, std::initializer_list<Ty> Init) {
    insert(Where, Init.begin(), Init.end());
  }

  void remove(const Ty &Val) {
    if (size() == 0)
      return;
    iterator it = begin();
    while (it != end()) {
      if (*it == Val) {
        it = UnlinkAndDelete(it.data());
      } else {
        ++it;
      }
    }
  }

  void clear() {
    if (size() == 0)
      return;
    iterator it = begin();
    while (it != end()) {
      it = UnlinkAndDelete(it.data());
    }
    size_ = 0;
  }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  void resize(size_t NewSize) {
    if (NewSize == size_)
      return;
    if (NewSize < size_)
      while (NewSize != size_) {
        RemoveTail();
        --size_;
      }
    else
      while (NewSize != size_) {
        InsertTail(Ty());
        ++size_;
      }
  }

  void resize(size_t NewSize, const_reference Element) {
    if (NewSize == size_)
      return;
    if (NewSize < size_)
      while (NewSize != size_) {
        RemoveTail();
        --size_;
      }
    else
      while (NewSize != size_) {
        InsertTail(Element);
        ++size_;
      }
  }

  void dump() {
    std::cout << "[";
    if (size() != 0) {
      for (iterator it = begin(); it != end(); ++it) {
        std::cout << *it << ",";
      }
    }
    std::cout << "]" << std::endl;
  }

private:
  /// @brief:创建一个新结点，自动完成构造
  template <typename... ValTy> node_ptr NewNode(ValTy &&... Element) {
    /// 编译器自己推导左值还是右值
    node_ptr new_node = (node_ptr)al_::Allocate(sizeof(node));
    ::new (new_node) node(std::forward<ValTy>(Element)...);
    return new_node;
  }

  /// @brief:在Where之后插入一个元素
  template <typename... ValTy>
  iterator InsertAfter(node_ptr Where, ValTy &&... Element) {
    node_ptr new_node = NewNode(std::forward<ValTy>(Element)...);
    new_node->Next = Where->Next;
    new_node->Prev = Where;
    Where->Next->Prev = new_node;
    Where->Next = new_node;
    return iterator(new_node);
  }

  /// @brief:在Where之后插入一个元素
  template <typename... ValTy>
  iterator InsertBefore(node_ptr Where, ValTy &&... Element) {
    node_ptr new_node = NewNode(std::forward<ValTy>(Element)...);
    new_node->Next = Where;
    new_node->Prev = Where->Prev;
    Where->Prev->Next = new_node;
    Where->Prev = new_node;
    return iterator(new_node);
  }

  /// @brief:在链表尾部插入一个元素
  template <typename... ValTy> void InsertTail(ValTy &&... Element) {
    InsertBefore(GetHead(), std::forward<ValTy>(Element)...);
  }

  /// @breif:在链表头部插入一个元素
  template <typename... ValTy> void InsertHead(ValTy &&... Element) {
    InsertAfter(GetHead(), std::forward<ValTy>(Element)...);
  }

  /// @brief:将一个元素移出链表并销毁
  iterator UnlinkAndDelete(node_ptr Where) {
    node_ptr prev = Where->Prev;
    node_ptr next = Where->Next;
    prev->Next = next;
    next->Prev = prev;

    Where->~ListNode();
    al_::Deallocate(Where);

    return iterator(next);
  }

  /// @brief:删除链表尾部的结点
  void RemoveTail() { UnlinkAndDelete(GetHead()->Prev); }

  /// @brief:删除链表头部的结点
  void RemoveHead() { UnlinkAndDelete(GetHead()->Next); }

  node_ptr GetHead() { return &head_; };

private:
  node head_;
  size_t size_ = 0;
}; // namespace adt

} // namespace adt