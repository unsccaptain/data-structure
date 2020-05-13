#pragma once

#include "Vector.h"
#include <map>
#include <memory>
#include <random>
#include <set>
#include <time.h>
#include <vector>

namespace adt {

template <typename Ty> class SkipListNode {
public:
  using Nodeptr = SkipListNode<Ty> *;
  using Linker = std::pair<Nodeptr, Nodeptr>;
  using LinkageTy = std::vector<Linker>;

  LinkageTy Linkage;
  Ty Value;

public:
  Nodeptr &Prev(size_t H) { return Linkage[H].first; }
  Nodeptr &Succ(size_t H) { return Linkage[H].second; }

  template <typename... ValTy>
  SkipListNode(ValTy &&... Val) : Value(std::forward<ValTy>(Val)...) {}

  ~SkipListNode() {
    Value.~Ty();
    Linkage.~vector();
  }
};

template <class Ty, bool Const> class SkipListIterator {
private:
  using node_ptr = SkipListNode<Ty> *;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Ty;
  using pointer = typename std::conditional<Const, const Ty *, Ty *>::type;
  using reference = typename std::conditional<Const, const Ty &, Ty &>::type;
  using const_reference = const Ty &;
  using difference_type = int;

public:
  SkipListIterator() : cur_(nullptr), head_(nullptr) {}
  SkipListIterator(node_ptr Cur, node_ptr Head) : cur_(Cur), head_(Head) {}

  SkipListIterator(const SkipListIterator &Right)
      : cur_(Right.cur_), head_(Right.head_) {}

  SkipListIterator &operator++() {
    assert(cur_ != head_);
    cur_ = cur_->Succ(0);
    return (*this);
  }

  const SkipListIterator operator++(int) {
    SkipListIterator Tmp = *this;
    ++*this;
    return Tmp;
  }

  SkipListIterator &operator--() {
    assert(cur_ != head_);
    cur_ = cur_->Prev(0);
    return (*this);
  }

  const SkipListIterator operator--(int) {
    SkipListIterator Tmp = *this;
    --*this;
    return Tmp;
  }

  reference operator*() const { return cur_->Value; }
  pointer operator->() const { return &cur_->Value; }

  bool operator!=(const SkipListIterator &Right) const {
    return cur_ != Right.cur_;
  }

  bool operator==(const SkipListIterator &Right) const {
    return cur_ == Right.cur_;
  }

  node_ptr data() const { return cur_; }

private:
  node_ptr cur_, head_;
};

template <typename Ty, typename AllocatorTy = Allocator> class SkipList {
public:
  using node = SkipListNode<Ty>;
  using node_ptr = SkipListNode<Ty> *;
  using Linker = std::pair<node_ptr, node_ptr>;
  using LinkageTy = Vector<Linker>;

  using iterator = SkipListIterator<Ty, false>;
  using const_iterator = SkipListIterator<Ty, true>;
  using al_ = AllocatorTy;

public:
  SkipList() { InitializeRangeTable(); }

  SkipList(const SkipList &Another) {
    for (iterator it = Another.begin(); it != Another.end(); ++it) {
      push(*it);
    }
  }

  size_t size() const { return size_; }
  bool empty() const { return !size(); }

  iterator begin() {
    assert(size() != 0);
    return iterator(Head()->Succ(0), Head());
  }

  const_iterator begin() const {
    assert(size() != 0);
    return const_iterator(Head()->Succ(0), Head());
  }

  iterator end() { return iterator(Head(), Head()); }
  const_iterator end() const { return const_iterator(Head(), Head()); }

  iterator find(const Ty &Val) const {
    return iterator(FindElement(Val), Head());
  }

  void push(const Ty &Value) { InsertValue(Value); }
  void push(Ty &&Value) { InsertValue(Value); }

  template <typename... ValTy> void emplace(ValTy &&... Value) {
    InsertValue(std::forward<ValTy>(Value)...);
  }

  iterator find(const Ty &Value) {
    node_ptr node = FindElement(Value);
    return iterator(node, Head());
  }

  void remove(const Ty &Val) {
    node_ptr Node = FindElement(Val);
    if (Node == Head()) {
      return;
    }
    RemoveNode(Node);
  }

  iterator remove(iterator Where) {
    assert(Where.data() != Head());
    node_ptr next = Where.data()->Succ(0);
    RemoveNode(Where.data());
    return iterator(next, Head());
  }

  /// @brief ���ش��ڵ���Value�ĵ�һ�����ĵ�����
  iterator lower_bound(const Ty &Value) {
    std::vector<node_ptr> TmpLink(height_);
    node_ptr node = LookupPosition(Value, TmpLink);
    if (node == Head())
      return end();
    if (node->Value < Value)
      node = node->Succ(0);
    return iterator(node, Head());
  }

  /// @brief ���ش���Value�ĵ�һ�����ĵ�����
  iterator upper_bound(const Ty &Value) {
    iterator it = lower_bound(Value);
    if (!(Value < *it))
      /// Ҳ����˵�ҵ���Value�Ľ��
      ++it;
    return it;
  }

  void clear() {
    if (size_ == 0)
      return;
    node_ptr cur = Head()->Succ(0);
    while (cur != Head()) {
      node_ptr next = cur->Succ(0);
      cur->~SkipListNode();
      al_::Deallocate(cur);
      cur = next;
    }
    size_ = 0;
    height_ = 0;
    Head()->Linkage.resize(0);
  }

private:
  /// @brief �ҵ�������_Val�ĵ�һ���ڵ�
  node_ptr LookupPosition(const Ty &Value,
                          std::vector<node_ptr> &Linkage) const {
    node_ptr Begin = Head();
    /// less���ڼ�¼nextǰ��һ����㣬��Ϊ�����whileѭ������ʱ����next�Ǵ���Value��
    node_ptr LessPos = nullptr, Next = nullptr;
    /// Ѱ��ÿһ����С��Value�������
    for (int i = height_ - 1; i >= 0; --i) {
      LessPos = Next = Begin;
      while (Next->Succ(i) != Head()) {
        Next = Next->Succ(i);
        /// ���Next->Value<=Value,��ô������ǰ
        if (Next->Value < Value || !(Value < Next->Value)) {
          LessPos = Next;
          continue;
        }
        break;
      }
      Linkage[i] = LessPos;
      Begin = LessPos;
    }
    return LessPos;
  }

  /// @brief ����һ��ֵ
  template <typename... ValTy> node_ptr InsertValue(ValTy &&... Value) {
    std::vector<node_ptr> TmpLink(height_);
    size_t NewHeight = 0;
    const size_t Highest = height_ + 1;
    bool Promote = false;

    node_ptr NewNode = AllocateNode(std::forward<ValTy>(Value)...);

    /// ȡ�ò���λ��
    LookupPosition(NewNode->Value, TmpLink);

    /// ��������߶�
    std::random_device rd;
    unsigned int Index = height_dist_.lower_bound(rd() % INT32_MAX)->second;
    if (Index > Highest)
      Index = Highest;
    NewHeight = Index;

    /// �����Ҫ���Ӹ߶�
    if (NewHeight > height_) {
      height_ = NewHeight;
      Promote = true;
    }

    /// ������߲�ڵ�������ڵ����ӵ�������
    for (size_t i = 0; i < NewHeight - Promote; i++) {
      node_ptr &Next = TmpLink[i]->Succ(i);
      NewNode->Linkage.push_back(Linker(TmpLink[i], Next));
      Next->Prev(i) = NewNode;
      Next = NewNode;
#ifdef _DEBUG
      counter_[i]++;
#endif
    }

    /// ������������ˣ���Ҫ������߲������
    if (Promote) {
      Head()->Linkage.push_back(Linker(NewNode, NewNode));
      NewNode->Linkage.push_back(Linker(Head(), Head()));
#ifdef _DEBUG
      counter_[Head()->Linkage.size()]++;
#endif
    }
    IncrementSize();
    return NewNode;
  }

  /// @brief ��������ɾ��һ�����
  void RemoveNode(node_ptr Node) {
    assert(Node != Head());
    unsigned H = Node->Linkage.size();
    for (unsigned i = 0; i < H; i++) {
      if (Node->Succ(i) != Head()) {
        Node->Succ(i)->Prev(i) = Node->Prev(i);
      }
      Node->Prev(i)->Succ(i) = Node->Succ(i);
    }
    DecrementSize();
  }

  void IncrementSize() { ++size_; }

  void DecrementSize() { --size_; }

  node_ptr Head() const { return (node_ptr)&head_; }

  /// @brief ����һ���½��
  template <typename... ValTy> node_ptr AllocateNode(ValTy &&... Value) {
    node_ptr NewNode = (node_ptr)al_::Allocate(sizeof(node));
    ::new (NewNode) node(std::forward<ValTy>(Value)...);
    return NewNode;
  }

  /// @brief Ѱ��Value��������λ��
  /// @return ͷ�ڵ�ָ�룬���û�ҵ�
  /// @return Value�������е�λ��
  node_ptr FindElement(const Ty &Value) const {
    std::vector<node_ptr> TmpLink(height_);
    node_ptr Node = LookupPosition(Value, TmpLink);
    /// ���ܵ����û�ҵ������м�һ���ڵ��ͣ����
    if (Node == &head_ || Node->Value != Value) {
      return Head();
    }
    return Node;
  }

  /// @brief ��ʼ����ѯ��
  void InitializeRangeTable() {
    unsigned long long max = 1;
    unsigned count = 32;
    while (max <= UINT_MAX) {
      height_dist_[max] = count--;
      max *= 2;
    }
  }

private:
  node head_;
  size_t height_ = 0;
  size_t size_ = 0;
  std::map<unsigned int, unsigned int> height_dist_;
#ifdef _DEBUG
  std::vector<int> counter_ = std::vector<int>(100, 0);
#endif
};

} // namespace adt