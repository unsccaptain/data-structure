#pragma once

#include <memory>

namespace adt {

/// @brief ����
template <class Ty> void Swap(Ty &a, Ty &b) {
  Ty temp = std::move(a);
  a = std::move(b);
  b = std::move(temp);
}

/// @brief �Ƚ�comperator
template <class _Ty> struct Less {
  constexpr bool operator()(const _Ty &_Left, const _Ty &_Right) const {
    return _Left < _Right;
  }
};

/// @brief ��ȡID
template <class _Ty> struct Identify {
  constexpr size_t operator()(const _Ty &_A) const { return static_cast<size_t>(_A); }
};

} // namespace adt