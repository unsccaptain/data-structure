/**
 * Stack:栈结构
 * 方法：
 *	push	元素压栈
 *  pop		元素出栈
 *  top		返回栈顶元素
 *  size	返回栈当前大小
 *	empty	返回栈是否为空
 **/
#pragma once

#include <assert.h>

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
		Stack() {
			this->data_ = (Ty*)al_::Allocate(16 * sizeof(Ty));
			this->size_ = 0;
			this->capacity_ = 16;
		}

		Stack(size_t InitSize) {
			this->data_ = (Ty*)al_::Allocate(InitSize * sizeof(Ty));
			this->size_ = this->capacity_ = InitSize;
			/// 调用者必须保证BastTy拥有默认构造函数，不然不能调用该构造函数
			Ty temp;
			std::uninitialized_fill(this->Begin(), this->End(), temp);
		}

		/// @brief:入栈
		void push(Ty&& ele) {
			if (this->Size() >= this->Capacity())
				grow();
			::new (this->End()) Ty(std::move(ele));
			this->size_++;
		}

		/// @brief:入栈
		void push(const Ty& ele) {
			if (this->Size() >= this->Capacity())
				grow();
			::new (this->End()) Ty(ele);
			this->size_++;
		}

		/// @brief:出栈
		void pop() {
			assert(this->size_ != 0);
			this->size_--;
			std::destroy_at(this->End());
		}

		/// @brief:栈当前大小
		size_t size() const { return size_; }

		/// @brief:返回栈是否为空
		bool empty() const { return size_ == 0; }

		/// @brief:返回栈顶元素
		Ty& top() {
			assert(this->size_ != 0);
			return *(data_ + size_ - 1);
		}

	private:
		/// @brief:扩展线性空间
		void grow(size_t Size = 0) {
			size_t new_cap = Size;
			if (Size == 0)
				new_cap = this->size_ < 512 ? this->size_ * 2 : this->size_ + 64;
			else
				new_cap = (new_cap + 0x40) & (~(size_t)0x3F);

			Ty* new_data = (Ty*)al_::Allocate(new_cap * sizeof(Ty));
			std::uninitialized_copy(this->Begin(), this->End(), new_data);
			std::destroy(this->Begin(), this->End());
			this->data_ = new_data;
			this->capacity_ = new_cap;
		}

	private:
		size_t Size() const { return size_; }
		size_t Capacity() const { return capacity_; }
		pointer Begin() { return data_; }
		pointer End() { return data_ + size_; }

	private:
		pointer data_ = nullptr;
		size_t size_ = 0;
		size_t capacity_ = 0;
	};

}