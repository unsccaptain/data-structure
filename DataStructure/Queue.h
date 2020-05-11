#pragma once

#include "Basis.h"
#include "List.h"
#include "Vector.h"

namespace adt {

template <typename Ty, typename AllocatorTy = Allocator> class Queue {
public:
  using reference = Ty &;
  using const_reference = const Ty &;
  using pointer = Ty *;
  using const_pointer = const Ty *;

  using al_ = AllocatorTy;

public:
  Queue() {}

  Queue(const Queue &Another) {
    using Iterator = List<Ty, al_>::iterator;
    for (Iterator it = Another.container_.begin();
         it != Another.container_.end(); ++it) {
      push(*it);
    }
  }

  Queue(Queue &&Another) { container_ = std::move(Another.container_); }

  template <typename Iterator> Queue(Iterator First, Iterator Last) {
    for (Iterator it = First; it != Last; ++it) {
      push(*it);
    }
  }

  void push(const Ty &Element) { container_.emplace_back(Element); }
  void push(Ty &&Element) { container_.emplace_back(Element); }
  void pop() { container_.pop_front(); }

  template <typename... ValTy> void emplace(ValTy... Value) {
    container_.emplace_back(std::forward<ValTy>(Value)...);
  }

  Ty &front() { return container_.front(); }
  Ty &back() { return container_.back(); }
  const Ty &front() const { return container_.front(); }
  const Ty &back() const { return container_.back(); }

  size_t size() const { return container_.size(); }
  bool empty() const { return container_.empty(); }

private:
  List<Ty, al_> container_;
};

template <typename Ty, typename AllocatorTy = Allocator> class PriorityQueue {
public:
  using reference = Ty &;
  using const_reference = const Ty &;
  using pointer = Ty *;
  using const_pointer = const Ty *;
  using value_compare = Less<Ty>;

  using al_ = AllocatorTy;

  PriorityQueue() : less_(value_compare()) { container_.resize(1); }

  PriorityQueue(const value_compare &Pred) : less_(Pred) {
    container_.resize(1);
  }

  template <typename Iterator>
  PriorityQueue(Iterator First, Iterator Last) : less_(value_compare()) {
    container_.resize(1);
    for (Iterator it = First; it < Last; ++it)
      push(*it);
  }

  template <typename Iterator>
  PriorityQueue(Iterator First, Iterator Last, const value_compare &Pred)
      : less_(Pred) {
    container_.resize(1);
    for (Iterator it = First; it < Last; ++it)
      push(*it);
  }

  PriorityQueue(const PriorityQueue &Another) {
    using Iterator = typename Vector<Ty, al_>::iterator;
    container_.resize(1);
    for (Iterator it = ++Another.container_.begin();
         it < Another.container_.end(); ++it)
      push(*it);
  }

  PriorityQueue(PriorityQueue &&Another) {
    container_ = std::move(Another.container_);
    less_ = Another.less_;
  }

  void push(const Ty &Element) {
    container_.emplace_back(Element);
    AdjustLast();
  }

  void push(Ty &&Element) {
    container_.emplace_back(Element);
    AdjustLast();
  }

  template <typename... ValTy> void emplace(ValTy... Element) {
    container_.emplace_back(std::forward<ValTy>(Element)...);
    AdjustLast();
  }

  void pop() {
    assert(!empty());
    size_t LastIdx = container_.size() - 1;
    container_[1] = std::move(container_[LastIdx]);
    container_.pop_back();
    AdjustFirst();
  }

  const Ty &top() const {
    assert(!empty());
    return container_[1];
  }

  size_t size() const { return container_.size(); }

  bool empty() const { return !size(); }

private:
  /// @brief 最后一个元素上浮
  void AdjustLast() {
    size_t LastIdx = container_.size() - 1, Parent = LastIdx / 2;
    while (Parent >= 1) {
      Ty &Last = container_[LastIdx];
      if (less_.operator()(container_[Parent], Last)) {
        Swap(Last, container_[Parent]);
        LastIdx = Parent;
        Parent /= 2;
      } else
        break;
    }
  }

  /// @brief 第一个元素下沉
  void AdjustFirst() {
    size_t LeftChild = 2;
    size_t MaxIdx = LeftChild;
    size_t RightChild = LeftChild + 1;
    while (LeftChild < container_.size()) {
      if (RightChild < container_.size() &&
          less_.operator()(container_[LeftChild], container_[RightChild])) {
        MaxIdx = RightChild;
      }
      if (less_.operator()(container_[LeftChild / 2], container_[MaxIdx])) {
        Swap(container_[MaxIdx], container_[LeftChild / 2]);
        LeftChild = MaxIdx;
        continue;
      }
      break;
    }
  }

private:
  Vector<Ty, al_> container_;
  value_compare less_;
};

} // namespace adt