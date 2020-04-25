// DataStructure.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <algorithm>
#include "Vector.h"
#include "List.h"
#include "SortAlgo.h"
#include "Stack.h"
#include <list>
#include <stack>

class TestElement {
public:
	TestElement() {
	}
};

int main()
{
	int k = 5;

	std::stack<int> std_stack;

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
	

	adt::Vector<int> cc = adt::Vector<int>(10);
	cc[0] = 1;
	cc.assign({ 1,3,4,3,4,6,7,3,2,5,6,7,3,4,6,5,4 });

	adt::SelectorSortor(ss.begin(), ss.end());

	std::cout << "Hello World!\n";
}