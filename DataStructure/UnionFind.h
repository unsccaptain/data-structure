#pragma once

#include "Basis.h"
#include <assert.h>
#include <map>
#include <vector>

namespace adt {
using namespace std;

template <typename Ty, typename Id = Identify<Ty>, size_t MaxId = 1000>
class UnionFind {
  using find_result = pair<bool, size_t>;

public:
  UnionFind() {
    rank_.resize(MaxId, 0);
    root_.resize(MaxId, (unsigned)-1);
  }

  /// @brief 将两颗树合并到同一颗树中
  void union2(const Ty &A, const Ty &B) {
    size_t rootA = Root(A);
    size_t rootB = Root(B);

    if (rank_[rootA] > rank_[rootB]) {
      root_[rootB] = rootA;
    } else {
      root_[rootA] = rootB;
      if (rank_[rootA] == rank_[rootB])
        ++rank_[rootB];
    }
  }

  /// @brief 查询两个ID是否在同一颗树中
  find_result find(const Ty &A, const Ty &B) {
    size_t rootA = Root(A);
    size_t rootB = Root(B);
    if (rootA != rootB) {
      return find_result(false, 0);
    }

    return find_result(true, rootA);
  }

  /// @brief 查询一个ID的根节点
  size_t find(const Ty &A) { return Root(A); }

  /// @brief 重置并查集
  void reset() {
    for (size_t i = 0; i < MaxId; ++i) {
      rank_[i] = 0;
      root_[i] = (unsigned)-1;
    }
  }

private:
  /// @brief 获取某个结点的根结点序号
  size_t Root(const Ty &A) {
    size_t id = id_(A);
    assert(id < MaxId);
    if (root_[id] == (unsigned)-1) {
      root_[id] = id;
      rank_[id] = 1;
      return id;
    }

    while (root_[id] != id) {
      id = root_[id];
    }

    return id;
  }

private:
  vector<unsigned> rank_;
  vector<size_t> root_;
  Id id_;
};

} // namespace adt