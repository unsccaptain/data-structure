#pragma once

#include <memory>

namespace adt {

/// @brief ����
template <class Ty> static void Swap(Ty &a, Ty &b) {
  Ty temp = std::move(a);
  a = std::move(b);
  b = std::move(temp);
}

/// @brief ð������
template <class Iterator>
static void BubbleSortor(Iterator First, Iterator Last) {
  bool changed;
  Iterator end = Last;
  for (; First != end; --end) {
    /// ѡ�����ݣ�select bubble)
    changed = true;
    Iterator b = First;
    for (Iterator a = b++; b != end; ++b, ++a) {
      /// �ϸ�����
      if (*b < *a) {
        Swap(*a, *b);
        changed = false;
      }
    }
    if (changed)
      break;
  }
}

// @brief ѡ������
template <class Iterator>
static void SelectorSortor(Iterator First, Iterator Last) {
  for (Iterator iter = First; iter != Last; ++iter) {
    Iterator exchange = iter, iter_1 = iter;
    /// �ҵ����������������СԪ��
    for (++iter_1; iter_1 != Last; ++iter_1) {
      if (*iter_1 < *exchange)
        exchange = iter_1;
    }
    if (exchange == iter)
      continue;
    /// ����СԪ�ؽ�����ǰ��ȥ
    Swap(iter, exchange);
  }
}

} // namespace adt