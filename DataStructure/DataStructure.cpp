// DataStructure.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <algorithm>
#include "Vector.h"
#include "List.h"
#include "SortAlgo.h"
#include "Stack.h"
#include "Queue.h"
#include <list>
#include <queue>
#include <stack>

class TestElement {
public:
	TestElement() {
	}

	TestElement(int a, int b)
		:a_(a), b_(b) {
	}

	int a_;
	int b_;
};

int main()
{
	int k = 5;

	std::list<int> std_list;
	std::stack<int> std_stack;
	std::vector<int> std_vector;
	std::queue<int> std_queue;

	adt::Queue<TestElement> queue;
	queue.push(TestElement(1, 2));
	queue.emplace(3, 4);
	std::cout << queue.size();
	queue.pop();
	TestElement& ele = queue.front();

	adt::List<int> ss;
	ss.push_back(4);
	ss.push_back(6);
	ss.push_back(100);
	ss.push_back(23);

	adt::Stack<int> kk;
	kk.push(3);
	kk.push(5);
	kk.pop();
	int top = kk.top();

	adt::Vector<TestElement> aa;
	aa.emplace_back(1, 2);

	adt::Vector<int> cc = adt::Vector<int>(10);
	cc[0] = 1;
	cc.assign({ 1,3,4,3,4,6,7,3,2,5,6,7,3,4,6,5,4 });

	adt::SelectorSortor(ss.begin(), ss.end());

	std::cout << "Hello World!\n";
}