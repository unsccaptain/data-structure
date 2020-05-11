// DataStructure.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "BST.h"
#include "List.h"
#include "Queue.h"
#include "Set.h"
#include "SortAlgo.h"
#include "Stack.h"
#include "UnorderedSet.h"
#include "Vector.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>

using namespace std;

class TestElement {
public:
  TestElement() {}

  TestElement(int a, int b) : a_(a), b_(b) {}

  ~TestElement() {}

  int a_;
  int b_;
};

void test_vector() {
  adt::Vector<int> vt;

  cout << endl << "test vector:" << endl;

  cout << "push_back 10|12" << endl;
  vt.push_back(10);
  vt.push_back(12);
  vt.dump();

  cout << "emplace_back 5|8" << endl;
  vt.emplace_back(5);
  vt.emplace_back(8);
  vt.dump();

  cout << "size" << endl;
  cout << vt.size() << endl;

  cout << "front - back" << endl;
  cout << vt.front() << " - " << vt.back() << endl;

  cout << "append 14 dup 5" << endl;
  vt.append((size_t)5, 14);
  vt.dump();

  cout << "resize to 20" << endl;
  vt.resize(20);
  vt.dump();

  cout << "resize to 4" << endl;
  vt.resize(4);
  vt.dump();

  cout << "change vt[0] to 10000" << endl;
  vt[0] = 10000;
  vt.dump();

  cout << "copy cotor" << endl;
  adt::Vector<int> v2 = adt::Vector<int>(vt);
  v2.dump();

  cout << "move cotor" << endl;
  adt::Vector<int> v3 = adt::Vector<int>(std::move(vt));
  v3.dump();

  cout << "erase at begin" << endl;
  v3.erase(v3.begin());
  v3.dump();

  cout << "clear" << endl;
  v3.clear();
  v3.dump();
}

void test_list() {
  adt::List<int> vt;

  cout << endl << "test list:" << endl;

  cout << "push_back 10|12|19|20" << endl;
  vt.push_back(10);
  vt.push_back(12);
  vt.dump();

  cout << "emplace 19|20 after 12" << endl;
  auto it = vt.begin();
  ++it;
  it = vt.emplace(it, 19);
  it = vt.emplace(it, 20);
  vt.dump();

  cout << "emplace_back 5|8" << endl;
  vt.emplace_back(5);
  vt.emplace_back(8);
  vt.dump();

  cout << "size" << endl;
  cout << vt.size() << endl;

  cout << "front - back" << endl;
  cout << vt.front() << " - " << vt.back() << endl;

  cout << "resize to 20" << endl;
  vt.resize(20);
  vt.dump();

  cout << "resize to 4" << endl;
  vt.resize(4);
  vt.dump();

  cout << "change front to 10000" << endl;
  vt.front() = 10000;
  vt.dump();

  cout << "insert 5 after 10000" << endl;
  it = vt.insert(vt.begin(), 5);
  vt.dump();

  cout << "insert 6 dup 3 after 5" << endl;
  vt.insert(it, (size_t)3, 6);
  vt.dump();

  cout << "copy cotor" << endl;
  adt::List<int> v2 = adt::List<int>(vt);
  v2.dump();

  cout << "move cotor" << endl;
  adt::List<int> v3 = adt::List<int>(std::move(v2));
  v3.dump();

  cout << "remove 6" << endl;
  v3.remove(6);
  v3.dump();

  cout << "clear" << endl;
  v3.clear();
  v3.dump();
}

void test_priority_queue() {
  adt::PriorityQueue<int> pq;
  cout << "push 5" << endl;
  pq.push(5);
  cout << "top:" << pq.top() << endl;

  cout << "push 1" << endl;
  pq.push(1);
  cout << "top:" << pq.top() << endl;

  cout << "push 9" << endl;
  pq.push(9);
  cout << "top:" << pq.top() << endl;

  cout << "push 4" << endl;
  pq.push(4);
  cout << "top:" << pq.top() << endl;

  cout << "push 15" << endl;
  pq.push(15);
  cout << "top:" << pq.top() << endl;

  cout << "push 22" << endl;
  pq.push(22);
  cout << "top:" << pq.top() << endl;

  cout << "push 25" << endl;
  pq.push(25);
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;

  cout << "pop" << endl;
  pq.pop();
  cout << "top:" << pq.top() << endl;
}

void dump_set(adt::Set<int> &set) {
  std::cout << "<";
  for (adt::Set<int>::iterator iter = set.begin(); iter != set.end(); ++iter)
    std::cout << *iter << ",";
  std::cout << ">" << std::endl;
}

void test_set() {
  adt::Set<int> avl;

  std::cout << "insert 5" << std::endl;
  avl.insert(5);
  dump_set(avl);

  std::cout << "insert 1" << std::endl;
  avl.insert(1);
  dump_set(avl);

  std::cout << "insert 2" << std::endl;
  avl.insert(2);
  dump_set(avl);

  std::cout << "insert 4" << std::endl;
  avl.insert(4);
  dump_set(avl);

  std::cout << "insert 43" << std::endl;
  avl.insert(43);
  dump_set(avl);

  std::cout << "insert 66" << std::endl;
  avl.insert(66);
  dump_set(avl);

  std::cout << "insert 3" << std::endl;
  avl.insert(3);
  dump_set(avl);

  auto it = avl.lower_bound(9);
  std::cout << "lower bound 9 is " << *it << std::endl;

  it = avl.lower_bound(4);
  std::cout << "lower bound 4 is " << *it << std::endl;

  it = avl.upper_bound(4);
  std::cout << "upper bound 4 is " << *it << std::endl;

  adt::Set<int> avl2 = adt::Set<int>(avl);

  adt::Set<int> avl3 = adt::Set<int>(std::move(avl));
}

int main() {

  std::priority_queue<int> zz;
  std::vector<int> aa;
  std::list<int> k;
  test_vector();
  test_list();
  test_priority_queue();
  test_set();

  std::cout << "Hello World!\n";
}