/**
 * Stack:栈结构
 * 方法：
 *	push	元素压栈
 *	pop		元素出栈
 *	top		返回栈顶元素
 *	size	返回栈当前大小
 *	empty	返回栈是否为空
 **/
#pragma once

#include <assert.h>
#include "Vector.h"

namespace adt {

	template<typename Ty, typename AllocatorTy = Allocator>
	class Stack {
	public:
		using reference = Ty&;
		using const_reference = const Ty&;
		using pointer = Ty*;
		using const_pointer = const Ty*;

		using al_ = AllocatorTy;

	public:
		Stack() :container_() {}
		Stack(size_t InitSize) :container_(InitSize) {}

		void push(Ty&& Element) { container_.push_back(Element); }
		void push(const Ty& Element) { container_.push_back(Element); }
		void pop() { container_.pop_back(); }

		size_t size() const { return container_.size(); }
		bool empty() const { return size() == 0; }

		/// @brief:返回栈顶元素
		Ty& top() {
			assert(this->size() != 0);
			return container_.back();
		}

		/// @brief:push an element
		template<typename... ValTy>
		void emplace(ValTy&&... Value) {
			container_.emplace_back(Value);
		}

	private:
		Vector<Ty, al_> container_;
	};

}