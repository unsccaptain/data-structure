#pragma once

#include <string>
#include <vector>

/// this is a memory-efficient data structure to index the range of particular
/// prefix.it is meaningless to implement it as a templete class

namespace adt {
using namespace std;

struct Range {
  unsigned Start;
  unsigned End;

  explicit Range() = default;
  explicit Range(unsigned st, unsigned ed) : Start(st), End(ed) {}
};

struct TreeNode {
  TreeNode *Left;  ///< left tree node of current level
  TreeNode *Right; ///< right tree node of current level
  TreeNode *Mid;   ///< next level

  char Token;        ///< prefix unit
  Range PrefixRange; ///<
};

class TernaryTree {
public:
  explicit TernaryTree() = default;

  /// @brief initialize the ternary tree completely
  void Initialize(const vector<string> &symtab) { BuildTree(symtab); }

  /// @brief find the range of the symbol table which start with 'prefix'
  /// @return [start index, end index)
  Range LookupPrefix(const string &prefix) {
    TreeNode *node = Traverse(prefix);
    if (node == nullptr) {
      return Range(0, 0);
    }
    return Range(node->PrefixRange.Start, node->PrefixRange.End + 1);
  }

private:
  /// @brief allocate node from heap and initialize
  TreeNode *AllocNode(char tk, const Range &rg);

  /// @brief insert a single character to the tree
  TreeNode *InsertNode(TreeNode *root, const string &sym, unsigned pos,
                       unsigned idx);

  /// @brief insert a symbol to the tree
  void InsertSymbol(const vector<string> &symtab, unsigned st, unsigned ed);

  /// @brief build the entire tree
  void BuildTree(const vector<string> &symtab);

  /// @brief traverse the ternary tree top-down
  TreeNode *Traverse(const string &prefix);

  /// @brief find the node which has particular character
  TreeNode *FindLevelNode(TreeNode *root, char tk);

private:
  TreeNode *root_;
};

} // namespace adt