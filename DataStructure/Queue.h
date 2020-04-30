#pragma once

#include "List.h"

namespace adt {

template <typename Ty, typename AllocatorTy = Allocator> class Queue {
public:
  using reference = Ty &;
  using const_reference = const Ty &;
  using pointer = Ty *;
  using const_pointer = const Ty *;

  using al_ = AllocatorTy;

public:
  void push(const Ty &Element) { container_.push_back(Element); }
  void push(Ty &&Element) { container_.push_back(Element); }
  void pop() { container_.pop_front(); }

  template <typename... ValTy> void emplace(ValTy... Element) {
    container_.emplace_back(std::forward<ValTy>(Element)...);
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

} // namespace adt