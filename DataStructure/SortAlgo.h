#pragma once

#include <memory>

namespace adt {

/// @brief 交换
template <class Ty> static void Swap(Ty &a, Ty &b) {
  Ty temp = std::move(a);
  a = std::move(b);
  b = std::move(temp);
}

/// @brief 冒泡排序
template <class Iterator>
static void BubbleSortor(Iterator First, Iterator Last) {
  bool changed;
  Iterator end = Last;
  for (; First != end; --end) {
    /// 选择气泡（select bubble)
    changed = true;
    Iterator b = First;
    for (Iterator a = b++; b != end; ++b, ++a) {
      /// 上浮气泡
      if (*b < *a) {
        Swap(*a, *b);
        changed = false;
      }
    }
    if (changed)
      break;
  }
}

// @brief 选择排序
template <class Iterator>
static void SelectorSortor(Iterator First, Iterator Last) {
  for (Iterator iter = First; iter != Last; ++iter) {
    Iterator exchange = iter, iter_1 = iter;
    /// 找到后面的子序列中最小元素
    for (++iter_1; iter_1 != Last; ++iter_1) {
      if (*iter_1 < *exchange)
        exchange = iter_1;
    }
    if (exchange == iter)
      continue;
    /// 将最小元素交换到前面去
    Swap(iter, exchange);
  }
}

} // namespace adt