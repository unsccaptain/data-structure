#pragma once

#include <memory>

namespace adt {

/// @brief ½»»»
template <class Ty> void Swap(Ty &a, Ty &b) {
  Ty temp = std::move(a);
  a = std::move(b);
  b = std::move(temp);
}

/// @brief ±È½Ïcomperator
template <class _Ty> struct Less {
  constexpr bool operator()(const _Ty &_Left, const _Ty &_Right) const {
    return _Left < _Right;
  }
};

} // namespace adt