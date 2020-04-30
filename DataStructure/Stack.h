/**
 * Stack:ջ�ṹ
 * ������
 *	push	Ԫ��ѹջ
 *	pop		Ԫ�س�ջ
 *	top		����ջ��Ԫ��
 *	size	����ջ��ǰ��С
 *	empty	����ջ�Ƿ�Ϊ��
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

		/// @brief:����ջ��Ԫ��
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