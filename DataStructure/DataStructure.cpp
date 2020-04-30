// DataStructure.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "Vector.h"
#include "List.h"
#include "SortAlgo.h"
#include "Stack.h"
#include "Queue.h"
#include "UnorderedSet.h"
#include <list>
#include <queue>
#include <stack>

using namespace std;

class TestElement {
public:
	TestElement() {
	}

	TestElement(int a, int b)
		:a_(a), b_(b) {
	}

	~TestElement() {
	}

	int a_;
	int b_;
};

void test_vector() {
	adt::Vector<int> vt;

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

int main()
{
	test_vector();

	std::cout << "Hello World!\n";
}