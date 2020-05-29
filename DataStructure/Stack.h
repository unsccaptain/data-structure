/**
 * Stack:ջ�ṹ
 * ������
 *	push	Ԫ��ѹջ
 *	pop		Ԫ�س�ջ
 *	top		����ջ��Ԫ��
 *	size	����ջ��ǰ��С
 *	empty	����ջ�Ƿ�Ϊ��
 **/
#pragma once

#include "Vector.h"
#include <assert.h>

namespace adt {

template <typename Ty, typename AllocatorTy = Allocator> class Stack {
public:
  using reference = Ty &;
  using const_reference = const Ty &;
  using pointer = Ty *;
  using const_pointer = const Ty *;

  using al_ = AllocatorTy;

public:
  Stack() : container_() {}
  Stack(size_t InitSize) : container_(InitSize) {}

  Stack(const Stack &Another) {
    for (auto iter = Another.container_.begin();
         iter != Another.container_.end(); ++iter) {
      push(*iter);
    }
  }

  Stack(Stack &&Another) { container_ = std::move(Another.container_); }

  void push(Ty &&Element) { container_.push_back(Element); }
  void push(const Ty &Element) { container_.push_back(Element); }
  void pop() { container_.pop_back(); }

  size_t size() const { return container_.size(); }
  bool empty() const { return size() == 0; }

  /// @brief:����ջ��Ԫ��
  Ty &top() {
    assert(this->size() != 0);
    return container_.back();
  }

  /// @brief:push an element
  template <typename... ValTy> void emplace(ValTy &&... Value) {
    container_.emplace_back(Value);
  }

private:
  Vector<Ty, al_> container_;
};

} // namespace adt