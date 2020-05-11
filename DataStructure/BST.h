#pragma once

#include "TreeIterator.h"
#include "Vector.h"
#include "Basis.h"
#include <algorithm>

namespace adt {

template <typename Ty> struct AvlTreeNode {
  AvlTreeNode *Children[2] = {nullptr};
  AvlTreeNode *Parent = nullptr;
  unsigned Height = 0;
  Ty Value;

  AvlTreeNode *&Left() { return Children[0]; }
  AvlTreeNode *&Right() { return Children[1]; }

  void Left(AvlTreeNode *Node) {
    Children[0] = Node;
    if (Node)
      Node->Parent = this;
  }
  void Right(AvlTreeNode *Node) {
    Children[1] = Node;
    if (Node)
      Node->Parent = this;
  }

  template <typename... ValTy>
  explicit AvlTreeNode(ValTy &&... V) : Value(std::forward<ValTy>(V)...) {}

  ~AvlTreeNode() { Value.~Ty(); }
};

template <typename Ty> struct TreeTraits<AvlTreeNode<Ty>> {
  using node_type = AvlTreeNode<Ty>;
  using node_pointer_type = AvlTreeNode<Ty> *;
  using value_type = Ty;
  using reference = Ty &;
  using pointer = Ty *;

  static node_pointer_type GetNextSlibing(node_pointer_type Node) {
    if (Node == Node->Parent->Left())
      return Node->Parent->Right();
    return nullptr;
  }

  static node_pointer_type GetPrevSlibing(node_pointer_type Node) {
    if (Node == Node->Parent->Right())
      return Node->Parent->Left();
    return nullptr;
  }

  static node_pointer_type GetLeftChild(node_pointer_type Node) {
    return Node->Left();
  }

  static node_pointer_type GetRightChild(node_pointer_type Node) {
    return Node->Right();
  }

  static node_pointer_type GetParent(node_pointer_type Node) {
    return Node->Parent;
  }

  static unsigned GetNumOfChildren(node_pointer_type Node) {
    if (Node->Left() && Node->Right())
      return 2;
    if (Node->Left() || Node->Right())
      return 1;
    return 0;
  }

  static node_pointer_type GetChildren(node_pointer_type Node, unsigned Idx) {
    assert(Idx < 2);
    if (Idx == 0 && Node->Left()) {
      return Node->Left();
    }
    return Node->Right();
  }

  static reference GetValue(node_pointer_type Node) { return Node->Value; }
};

template <typename Ty, typename AllocatorTy = Allocator> class BSTAvlImpl {
public:
  using node_type = AvlTreeNode<Ty>;
  using node_ptr = AvlTreeNode<Ty> *;
  using iterator = TreeInOrderIterator<node_type, false>;
  using const_iterator = TreeInOrderIterator<node_type, true>;
  using value_compare = Less<Ty>;

  using al_ = AllocatorTy;

public:
  BSTAvlImpl() : root_(nullptr), size_(0) {}

  BSTAvlImpl(BSTAvlImpl &&Another)
      : root_(Another.root_), size_(Another.size_) {
    Another.root_ = nullptr;
    Another.size_ = 0;
  }

  iterator insert(const Ty &Value) {
    node_ptr new_node = NewNode(Value);
    root_ = InsertImpl(root_, new_node);
    root_->Parent = nullptr;
    ++size_;
    return iterator(new_node);
  }

  iterator insert(Ty &&Value) {
    node_ptr new_node = NewNode(std::move(Value));
    root_ = InsertImpl(root_, new_node);
    root_->Parent = nullptr;
    ++size_;
    return iterator(new_node);
  }

  iterator find(const Ty &Value) {
    node_ptr node = root_;
    while (node) {
      if (node->Value < Value)
        node = node->Right();
      else if (Value < node->Value)
        node = node->Left();
      else
        return iterator(node);
    }
    return end();
  }

  size_t erase(const Ty &Value) {
    iterator node = find(Value);
    if (node == end())
      return 0;
    RemoveImpl(node.data());
    --size_;
    return 1;
  }

  iterator erase(iterator Where) {
    iterator it = Where++;
    RemoveImpl(it.data());
    --size_;
    return Where;
  }

  iterator erase(const_iterator Where) {
    iterator it = Where._Remove_Const()++;
    RemoveImpl(it.data());
    --size_;
    return Where._Remove_Const();
  }

  size_t count(const Ty &Value) const {
    if (find(Value) != end())
      return 1;
    return 0;
  }

  bool empty() const { return size_ == 0; }

  iterator begin() {
    assert(size_ != 0);
    node_ptr node = root_;
    while (node->Left())
      node = node->Left();
    return iterator(node);
  }

  const_iterator begin() const {
    assert(size_ != 0);
    node_ptr node = root_;
    while (node->Left())
      node = node->Left();
    return const_iterator(node);
  }

  iterator end() { return iterator(); }

  const_iterator end() const { return const_iterator(); }

  iterator lower_bound(const Ty &Value) { return LookupLowerBound(Value); }

  iterator upper_bound(const Ty &Value) {
    iterator it = LookupLowerBound(Value);
    if (!(Value < *it))
      ++it;
    return it;
  }

  void clear() {
    Stack<node_ptr> bfs;
    bfs.push(root_);
    while (bfs.size()) {
      node_ptr top = bfs.top();
      if (top->Left())
        bfs.push(top->Left());
      if (top->Right())
        bfs.push(top->Right());
      bfs.pop();
      Replace(top, nullptr, top->Parent);
      top->~AvlTreeNode();
      al_::Deallocate(top);
    }
    size_ = 0;
  }

private:
  /// @brief ƽ��һ�����
  node_ptr MakeBalance(node_ptr SubTree) {
    int factor = GetBalanceFactor(SubTree);
    if (factor > 1 && GetBalanceFactor(SubTree->Left()) > 0) {
      /// LL
      SubTree = RightRotate(SubTree);
    } else if (factor > 1 && GetBalanceFactor(SubTree->Left()) < 0) {
      /// LR
      SubTree->Left() = LeftRotate(SubTree->Left());
      SubTree = RightRotate(SubTree);
    } else if (factor < -1 && GetBalanceFactor(SubTree->Right()) < 0) {
      /// RR
      SubTree = LeftRotate(SubTree);
    } else if (factor < -1 && GetBalanceFactor(SubTree->Right()) > 0) {
      /// RL
      SubTree->Right() = RightRotate(SubTree->Right());
      SubTree = LeftRotate(SubTree);
    }

    /// ���µ�ǰ���߶�
    UpdateHeight(SubTree);

    return SubTree;
  }

  /// @brief ��������ľ���ʵ��
  node_ptr InsertImpl(node_ptr SubTree, node_ptr Node) {
    if (!SubTree) {
      if (!root_)
        root_ = Node;
      return Node;
    }

    /// �ҵ�����λ��
    if (Node->Value < SubTree->Value) {
      SubTree->Left(InsertImpl(SubTree->Left(), Node));
    } else if (SubTree->Value < Node->Value) {
      SubTree->Right(InsertImpl(SubTree->Right(), Node));
    }

    /// ƽ�⻯
    return MakeBalance(SubTree);
  }

  /// @brief ��ȡ���ڵ���Value����С���
  iterator LookupLowerBound(const Ty &Value) {
    node_ptr node = root_, last_node = node;
    while (node) {
      if (node->Value < Value) {
        last_node = node;
        node = node->Right();
      } else if (Value < node->Value) {
        last_node = node;
        node = node->Left();
      } else
        return iterator(node);
    }
    iterator it = iterator(last_node);
    if (last_node->Value < Value) {
      ++it;
    }
    return it;
  }

  /// @brief ��Parent�µ�Old�����New������
  void Replace(node_ptr Old, node_ptr New, node_ptr Parent) {
    if (Parent == nullptr) {
      root_ = New;
      root_->Parent = nullptr;
      return;
    }
    if (Old == Parent->Left()) {
      Parent->Left(New);
    } else {
      Parent->Right(New);
    }
  }

  /// @brief ɾ��һ�����
  void RemoveImpl(node_ptr Element) {
    node_ptr parent = Element->Parent;
    if (Element->Left() == nullptr) {
      /// �ô�ɾ���������ӽڵ��滻��ɾ�����
      Replace(Element, Element->Right(), Element->Parent);
      /// �ݹ��ƽ���������Ƚ��
      while (parent != nullptr) {
        node_ptr old_parent = parent->Parent;
        node_ptr new_head = MakeBalance(parent);
        Replace(parent, new_head, old_parent);
        parent = old_parent;
      }
    } else if (Element->Right() == nullptr) {
      /// �ô�ɾ���������ӽڵ��滻��ɾ�����
      Replace(Element, Element->Left(), Element->Parent);
      /// �ݹ��ƽ���������Ƚ��
      while (parent != nullptr) {
        node_ptr old_parent = parent->Parent;
        node_ptr new_head = MakeBalance(parent);
        Replace(parent, new_head, old_parent);
        parent = old_parent;
      }
    } else {
      /// �ҵ���ɾ�����ĺ�̽ڵ�
      node_ptr succ = Element->Right();
      while (succ->Left()) {
        succ = succ->Left();
      }
      Element->Value = std::move(succ->Value);
      RemoveImpl(succ);
    }
  }

  /// @brief ɾ��һ��Ԫ��
  node_ptr RemoveImpl(node_ptr SubTree, const Ty &Element) {
    if (SubTree->Value < Element) {
      SubTree->Right() = RemoveImpl(SubTree->Right());
    } else if (Element < SubTree->Value) {
      SubTree->Left() = RemoveImpl(SubTree->Left());
    } else {
      if (SubTree->Left() == nullptr) {
        /// ��ʱ���������һ����㣬����ֱ�ӷ���
        return SubTree->Right();
      } else if (SubTree->Right() == nullptr) {
        /// ��ʱ���������һ����㣬����ֱ�ӷ���
        return SubTree->Left();
      } else {
        /// �ҵ���ɾ�����ĺ�̽ڵ�
        node_ptr succ = SubTree->Right();
        while (succ->Left()) {
          succ = succ->Left();
        }
        /// ����̽���Ƴ�
        succ->Right = RemoveImpl(SubTree->Right, succ->Value);
        succ->Left = SubTree->Left;
        return succ;
      }
    }

    return MakeBalance(SubTree);
  }

  /// @brief ����
  node_ptr RightRotate(node_ptr Node) {
    node_ptr new_head = Node->Left();
    node_ptr old_right = new_head->Right();
    Node->Left(old_right);
    new_head->Right(Node);
    /// ���¸߶�
    UpdateHeight(Node);
    UpdateHeight(new_head);
    return new_head;
  }

  /// @brief ����
  node_ptr LeftRotate(node_ptr Node) {
    node_ptr new_head = Node->Right();
    node_ptr old_left = new_head->Left();
    Node->Right(old_left);
    new_head->Left(Node);
    /// ���¸߶�
    UpdateHeight(Node);
    UpdateHeight(new_head);
    return new_head;
  }

  /// @brief ���µ�ǰ���߶�
  void UpdateHeight(node_ptr Node) {
    Node->Height = 1 + std::max(Node->Left() ? Node->Left()->Height : 0,
                                Node->Right() ? Node->Right()->Height : 0);
  }

  /// @brief ��ȡƽ������
  /// @return <0 ����������������
  /// @return =0 ��������������һ����
  /// @return >0 ����������������
  int GetBalanceFactor(node_ptr Node) {
    int left = Node->Left() ? Node->Left()->Height : 0;
    int right = Node->Right() ? Node->Right()->Height : 0;
    return left - right;
  }

  /// @brief �����µĽ��
  template <typename... ValTy> node_ptr NewNode(ValTy &&... Value) {
    node_ptr new_node = (node_ptr)al_::Allocate(sizeof(node_type));
    ::new (new_node) node_type(std::forward<ValTy>(Value)...);
    new_node->Height = 1;
    return new_node;
  }

private:
  node_ptr root_;
  size_t size_;
  value_compare less_;
};

} // namespace adt