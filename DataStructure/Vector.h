/**
 * Vector: �����ṹ
 * ������
 *	push_back		β������Ԫ��
 *  pop_back		�������һ��Ԫ��
 *  front			����ͷԪ������
 *  end				����βԪ������
 *	append			��������׷��[First,End)��Χ�ڵ�Ԫ��
 *	append			��������׷��Count��Element�ĸ���
 *	assign			��[First,End)��ֵ����ǰ����
 **/
#pragma once

#include "Allocator.h"
#include <assert.h>
#include <initializer_list>
#include <memory>
#include <iostream>

namespace adt {

class Allocator;

template <typename Ty> class VectorBase {
public:
  using iterator = Ty *;
  using const_iterator = const Ty *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reference = Ty &;
  using const_reference = const Ty &;
  using pointer = Ty *;
  using const_pointer = const Ty *;

public:
  size_t size() const { return size_; }
  size_t capacity() const { return capacity_; }
  pointer data() { return data_; }
  const_pointer data() const { return (const_pointer)data_; }

  iterator begin() { return data_; }
  iterator end() { return data_ + size_; }
  const_iterator begin() const { return (const_iterator)data_; }
  const_iterator end() const { return (const_iterator)data_ + size(); }

  reference operator[](size_t Idx) {
    assert(Idx < size_);
    return begin()[Idx];
  }

  const_reference operator[](size_t Idx) const {
    assert(Idx < size_);
    return (const_reference)begin()[Idx];
  }

  reference front() {
    assert(size_ > 0);
    return begin()[0];
  }

  reference back() {
    assert(size_ > 0);
    return end()[-1];
  }

  /// @brief:�������Ԫ��
  void clear() {
    std::destroy(this->begin(), this->end());
    this->size_ = 0;
  }

  /// @brief:�Ƴ�It����Ԫ��
  iterator erase(iterator It) {
    iterator it = It;
    for (iterator iter = It + 1; iter != this->end(); ++iter, ++it) {
      std::destroy_at(it);
      ::new (std::addressof(*it)) Ty(std::move(*iter));
    }
    pop_back();
    return const_cast<iterator>(It);
  }

  /// @brief:��������������
  void push_back(const Ty &Element) {
    if (this->size() >= this->capacity())
      grow();
    ::new (this->end()) Ty(Element);
    ++this->size_;
  }

  /// @brief:������ʱ����
  void push_back(Ty &&Element) {
    if (this->size() >= this->capacity())
      grow();
    ::new (this->end()) Ty(std::move(Element));
    ++this->size_;
  }

  template <typename... ValTy> void emplace_back(ValTy &&... Value) {
    if (this->size() >= this->capacity())
      grow();
    ::new (this->end()) Ty(std::forward<ValTy>(Value)...);
    this->size_++;
  }

  /// @brief:ȡ������
  void pop_back() {
    ++this->size_;
    std::destroy_at(this->end());
  }

  /// @brief:��[First,Last)����Ԫ����ӵ�vector��
  template <typename Iterator> void append(Iterator First, Iterator Last) {
    size_t append_count = std::distance(First, Last);
    if (this->capacity() - this->size() < append_count)
      grow(this->size() + append_count);
    std::uninitialized_copy(First, Last, this->end());
    this->size_ += append_count;
  }

  /// @brief:��Element���Count��
  void append(size_t Count, const Ty &Element) {
    if (this->capacity() - this->size() < Count)
      grow(this->size() + Count);
    std::uninitialized_fill_n(this->end(), Count, Element);
    this->size_ += Count;
  }

  /// @brief:��vector��ֵΪ[First,Last)��Ԫ��
  template <typename Iterator> void assign(Iterator First, Iterator Last) {
    this->clear();
    this->append(First, Last);
  }

  /// @brief:��vector��ֵΪ��ʼ���б�
  void assign(std::initializer_list<Ty> InitList) {
    this->clear();
    this->append(InitList.begin(), InitList.end());
  }

  /// @brief:����������С
  void resize(size_t NewSize, const Ty &Val) {
    int diff = NewSize - size_;
    if (diff == 0)
      return;
    else if (diff > 0) {
      this->grow(NewSize);
      std::uninitialized_fill_n(end(), diff, Val);
      size_ = NewSize;
    } else {
      std::destroy(begin() + NewSize, begin() + size());
      size_ = NewSize;
    }
  }

  /// @brief:����������С
  void resize(size_t NewSize) { resize(NewSize, Ty()); }

  void dump() {
    std::cout << "[";
    for (iterator it = begin(); it != end(); ++it) {
      std::cout << *it << ",";
    }
    std::cout << "]" << std::endl;
  }

protected:
  virtual void grow(size_t Size = 0) = 0;

protected:
  pointer data_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;
};

template <typename Ty, typename AllocatorTy = Allocator>
class Vector : public VectorBase<Ty> {
public:
  using al_ = AllocatorTy;

public:
  explicit Vector() {
    this->data_ = (Ty *)al_::Allocate(16 * sizeof(Ty));
    this->capacity_ = 16;
  }

  explicit Vector(size_t InitSize) {
    this->data_ = (Ty *)al_::Allocate(InitSize * sizeof(Ty));
    this->size_ = this->capacity_ = InitSize;
    /// �����߱��뱣֤BastTyӵ��Ĭ�Ϲ��캯������Ȼ���ܵ��øù��캯��
    Ty temp;
    std::uninitialized_fill(this->begin(), this->end(), temp);
  }

  explicit Vector(size_t InitSize, const Ty &Init) {
    this->data_ = (Ty *)al_::Allocate(InitSize * sizeof(Ty));
    this->size_ = this->capacity_ = InitSize;
    std::uninitialized_fill(this->begin(), this->end(), Init);
  }

  explicit Vector(std::initializer_list<Ty> InitList) {
    this->size_ = this->capacity_ = InitList.size();
    this->data_ = (Ty *)al_::Allocate(this->size_ * sizeof(Ty));
    std::uninitialized_copy(InitList.begin(), InitList.end(), this->begin());
  }

  explicit Vector(const Vector &Another) {
    this->size_ = Another.size();
    this->capacity_ = Another.capacity();
    this->data_ = (Ty *)al_::Allocate(this->capacity_ * sizeof(Ty));
    std::uninitialized_copy(Another.begin(), Another.end(), this->begin());
  }

  explicit Vector(Vector &&Another) {
    this->data_ = Another.data();
    this->size_ = Another.size();
    this->capacity_ = Another.capacity();
  }

  size_t reserve(size_t Size) { grow(Size); }

  /// @brief:��const���ý��и�ֵ
  Vector &operator=(const Vector &Right) {
    this->clear();
    this->assign(Right.begin(), Right.end());
    return *this;
  }

  /// @brief:����ֵ���ý��и�ֵ
  Vector &operator=(Vector &&Right) {
    this->clear();
    this->data_ = Right.data();
    this->size_ = Right.size();
    this->capacity_ = Right.capacity();
    return *this;
  }

private:
  /// @brief:��չ���Կռ�
  virtual void grow(size_t Size) {
    size_t new_cap = Size;
    if (this->capacity_ > Size)
      return;
    if (Size == 0)
      new_cap = this->size_ < 512 ? this->size_ * 2 : this->size_ + 64;
    else
      new_cap = (new_cap + 0x40) & (~(size_t)0x3F);

    Ty *new_data = (Ty *)al_::Allocate(new_cap * sizeof(Ty));
    std::uninitialized_copy(this->begin(), this->end(), new_data);
    std::destroy(this->begin(), this->end());
    this->data_ = new_data;
    this->capacity_ = new_cap;
  }
};

} // namespace adt