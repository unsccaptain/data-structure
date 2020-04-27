#pragma once

#include <memory>
#include <vector>
#include <assert.h>
#include "Allocator.h"

namespace adt {

	template<typename Ty>
	struct ListNode {
		ListNode* Prev;
		ListNode* Next;
		Ty Value;

		explicit ListNode(const Ty& Ele)
			:Value(Ele) {
		}

		explicit ListNode(Ty&& Ele)
			:Value(std::move(Ele)) {
		}

		template<typename... ValTy>
		explicit ListNode(ValTy&&... V)
			:Value(std::forward<ValTy>(V)...) {
		}

		~ListNode() = default;
	};

	template<typename Ty>
	class ListIterator {
	public:
		using node_ptr = ListNode<Ty>*;
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Ty;
		using pointer = Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;
		using difference_type = int;

	public:
		ListIterator(node_ptr Ptr)
			:node_(Ptr) {
		}

		ListIterator& operator++() {
			node_ = node_->Next;
			return *this;
		}

		ListIterator& operator--() {
			node_ = node_->Prev;
			return *this;
		}

		const ListIterator operator++(int) {
			ListIterator it = *this;
			++*this;
			return it;
		}

		const ListIterator operator--(int) {
			ListIterator it = *this;
			--*this;
			return it;
		}

		reference operator->() {
			return node_->Value;
		}

		const_reference operator->() const {
			return (const_reference)node_->Value;
		}

		reference operator*() {
			return node_->Value;
		}

		const_reference operator*() const {
			return node_->Value;
		}

		bool operator!=(const ListIterator& another) {
			return !IsSame(another);
		}

		bool operator==(const ListIterator & another) {
			return IsSame(another);
		}

	private:
		bool IsSame(const ListIterator& another) {
			return node_ == another.node_;
		}

	private:
		node_ptr node_;
	};

	template<typename Ty, typename AllocatorTy = Allocator>
	class List {
	public:
		using node = ListNode<Ty>;
		using node_ptr = ListNode<Ty>*;
		using reference = Ty&;
		using const_reference = const Ty&;
		using value_type = Ty;
		using iterator = ListIterator<Ty>;
		using const_iterator = const ListIterator<Ty>;

		using al_ = AllocatorTy;

	public:
		explicit List()
			:head_(Ty()) {
			head_.Next = head_.Prev = &head_;
		}

		explicit List(const_reference Element)
			:head_(Ty()) {
			head_.Next = head_.Prev = &head_;
		}

		void push_back(const_reference Element) {
			InsertTail(Element);
			++size_;
		}

		void push_back(Ty&& Element) {
			InsertTail(Element);
			++size_;
		}

		template<typename... ValTy>
		void emplace_back(ValTy&&... Element) {
			InsertTail(std::forward<ValTy>(Element)...);
			++size_;
		}

		void push_front(const_reference Element) {
			InsertHead(Element);
			++size_;
		}

		void push_front(Ty&& Element) {
			InsertHead(Element);
			++size_;
		}

		template<typename... ValTy>
		void emplace_front(ValTy&&... Element) {
			InsertHead(std::forward<ValTy>(Element)...);
			++size_;
		}

		void pop_front() { RemoveHead(); --size_; }
		void pop_back() { RemoveTail(); --size_; }
		
		reference front() {
			assert(size_ > 0);
			return head_.Next->Value;
		}

		const_reference front() const {
			assert(size_ > 0);
			return head_.Next->Value;
		}

		reference back() {
			assert(size_ > 0);
			return head_.Prev->Value;
		}

		const_reference back() const {
			assert(size_ > 0);
			return head_.Prev->Value;
		}

		iterator begin() {
			assert(head_.Next != &head_);
			return iterator(head_.Next);
		}

		const_iterator begin() const {
			assert(head_.Next != &head_);
			return (const_iterator)iterator(head_.Next);
		}

		iterator end() {
			assert(head_.Next != &head_);
			return iterator(&head_);
		}

		const_iterator end() const {
			assert(head_.Next != &head_);
			return (const_iterator)iterator(&head_);
		}

		template<typename Iterator>
		void insert(Iterator First, Iterator Last) {
			for (Iterator iter = First;iter != Last;++iter)
				push_back(*iter);
		}

		size_t size() const { return size_; }
		bool empty() const { return size_ == 0; }

		void resize(size_t NewSize) {
			if (NewSize == size_)
				return;
			if (NewSize < size_)
				while (NewSize != size_) {
					RemoveTail();
					--size_;
				}
			else
				while (NewSize != size_) {
					InsertTail(Ty());
					++size_;
				}
		}

		void resize(size_t NewSize, const_reference Element) {
			if (NewSize == size_)
				return;
			if (NewSize < size_)
				while (NewSize != size_) {
					RemoveTail();
					--size_;
				}
			else
				while (NewSize != size_) {
					InsertTail(Element);
					++size_;
				}
		}

	private:
		/// @brief:创建一个新结点，自动完成构造
		template<typename... ValTy>
		node_ptr NewNode(ValTy&&... Element) {
			/// 编译器自己推导左值还是右值
			node_ptr new_node = (node_ptr)al_::Allocate(sizeof(node));
			::new (new_node) node(std::forward<ValTy>(Element)...);
			return new_node;
		}

		/// @brief:在链表尾部插入一个元素
		template<typename... ValTy>
		void InsertTail(ValTy&&... Element) {
			node_ptr new_node = NewNode(std::forward<ValTy>(Element)...);
			new_node->Next = &head_;
			new_node->Prev = head_.Prev;
			head_.Prev->Next = new_node;
			head_.Prev = new_node;
		}

		/// @breif:在链表头部插入一个元素
		template<typename... ValTy>
		void InsertHead(ValTy&&... Element) {
			node_ptr new_node = NewNode(std::forward<ValTy>(Element)...);
			new_node->Next = head_.Next;
			new_node->Prev = &head_;
			head_.Next->Prev = new_node;
			head_.Next = new_node;
		}

		/// @brief:删除链表尾部的结点
		void RemoveTail() {
			node_ptr victim = head_.Prev;
			node_ptr new_tail = victim->Prev;
			new_tail->Next = &head_;
			head_.Prev = new_tail;

			victim->~ListNode();
			al_::Deallocate(victim);
		}

		/// @brief:删除链表头部的结点
		void RemoveHead() {
			node_ptr victim = head_.Next;
			node_ptr new_head = victim->Next;
			new_head->Prev = &head_;
			head_.Next = new_head;
			
			victim->~ListNode();
			al_::Deallocate(victim);
		}

	private:
		node head_;
		size_t size_ = 0;
	};

}