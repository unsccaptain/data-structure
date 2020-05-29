#include "TernaryTree.h"

using namespace adt;

/// @brief allocate node from heap and initialize
TreeNode *TernaryTree::AllocNode(char tk, const Range &rg) {
  TreeNode *new_node = new TreeNode();
  new_node->Token = tk;
  new_node->PrefixRange = rg;
  new_node->Left = nullptr;
  new_node->Right = nullptr;
  new_node->Mid = nullptr;
  return new_node;
}

/// @brief insert a single character to the tree
TreeNode *TernaryTree::InsertNode(TreeNode *root, const string &sym,
                                  unsigned pos, unsigned idx) {
  if (pos == sym.size())
    return root;
  char tk = sym[pos];
  if (!root) {
    root = AllocNode(tk, Range(idx, idx));
    root->Mid = InsertNode(root->Mid, sym, ++pos, idx);
    return root;
  }
  if (tk < root->Token) {
    root->Left = InsertNode(root->Left, sym, pos, idx);
  } else if (tk > root->Token) {
    root->Right = InsertNode(root->Right, sym, pos, idx);
  } else {
    if (root->PrefixRange.Start > idx)
      root->PrefixRange.Start = idx;
    else if (root->PrefixRange.End < idx)
      root->PrefixRange.End = idx;
    root->Mid = InsertNode(root->Mid, sym, ++pos, idx);
  }
  return root;
}

/// @brief insert a symbol to the tree
void TernaryTree::InsertSymbol(const vector<string> &symtab, unsigned st,
                               unsigned ed) {
  if (ed - st == 1) {
    InsertNode(root_, symtab[st], 0, st);
    return;
  } else if (ed - st == 0) {
    return;
  }

  unsigned mid_index = floor(1.0 * (st + ed) / 2);
  const string &root_sym = symtab[mid_index];
  InsertNode(root_, root_sym, 0, mid_index);
  InsertSymbol(symtab, st, mid_index);
  InsertSymbol(symtab, mid_index + 1, ed);
}

/// @brief build the entire tree
void TernaryTree::BuildTree(const vector<string> &symtab) {
  int mid_index = (int)floor(1.0 * symtab.size() / 2);
  const string &root_sym = symtab[mid_index];
  this->root_ = AllocNode(root_sym[0], Range(0, symtab.size()));
  InsertSymbol(symtab, 0, symtab.size());
}

/// @brief traverse the ternary tree top-down
TreeNode *TernaryTree::Traverse(const string &prefix) {
  TreeNode *node = root_;
  TreeNode *last = node;
  unsigned pos = 0;
  while (pos != prefix.size()) {
    node = FindLevelNode(node, prefix[pos]);
    if (!node)
      return nullptr;
    last = node;
    node = node->Mid;
    ++pos;
  }
  return last;
}

/// @brief find the node which has particular character
TreeNode *TernaryTree::FindLevelNode(TreeNode *root, char tk) {
  if (!root)
    return nullptr;
  if (tk < root->Token)
    return FindLevelNode(root->Left, tk);
  if (tk > root->Token)
    return FindLevelNode(root->Right, tk);
  return root;
}
