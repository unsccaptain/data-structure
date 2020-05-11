#pragma once

#include "BST.h"

namespace adt {

template <typename Ty, typename AllocatorTy = Allocator>
class Set : public BSTAvlImpl<Ty, AllocatorTy> {
public:
  using node_type = AvlTreeNode<Ty>;
  using iterator = TreeInOrderIterator<node_type, false>;
  using const_iterator = TreeInOrderIterator<node_type, true>;

public:
  Set() {}

  template <typename Iterator> Set(Iterator First, Iterator Last) {
    for (Iterator it = First; it != Last; ++it) {
      this->insert(*it);
    }
  }

  Set(const Set &Another) {
    for (const_iterator it = Another.begin(); it != Another.end(); ++it) {
      this->insert(*it);
    }
  }

  Set(Set &&Another) noexcept
      : BSTAvlImpl<Ty, AllocatorTy>(std::move(Another)) {}
};

} // namespace adt