// DataStructure.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "List.h"
#include "Queue.h"
#include "SortAlgo.h"
#include "Stack.h"
#include "UnorderedSet.h"
#include "Vector.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
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

int main() {

  std::list<int> k;
  test_vector();
  test_list();

  std::cout << "Hello World!\n";
}