#pragma once

#include "chunk_allocator.h"
#include <iterator>

template<typename T, template<typename Ty> class TAlloc = std::allocator>
class list {
	struct Node {
		template<typename... TArgs>
		Node(TArgs&&... Args) :Data(std::forward<TArgs>(Args)...) {}

		Node* Prev = nullptr;
		Node* Next = nullptr;
		T Data;
	};
private:
	template<typename T>
	class iterator_t : public std::bidirectional_iterator_tag {
		template <typename U, template<typename Ty> class TAlloc>
		friend class list;
	public:

		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		iterator_t(Node* pCurrent) noexcept :pCurrent(pCurrent) {}

		template<typename U>
		iterator_t(const iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) noexcept {
			pCurrent = i.pCurrent;
		}

		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, iterator_t>& operator=(const iterator_t<U>& i) noexcept {
			pCurrent = i.pCurrent;
			return *this;
		}

		template<typename U>
		bool operator==(const iterator_t<U>& i) const noexcept {
			return pCurrent == i.pCurrent;
		}

		template<typename U>
		bool operator!=(const iterator_t<U>& i) const noexcept {
			return pCurrent != i.pCurrent;

		}

		iterator_t& operator++() noexcept {

			pCurrent = pCurrent->Next;

			return *this;
		}
		iterator_t operator++(int) noexcept {

			const auto Result = pCurrent;
			pCurrent = pCurrent->Next;

			return pCurrent;
		}

		iterator_t& operator--() noexcept {

			pCurrent = pCurrent->Prev;

			return *this;
		}

		iterator_t operator--(int) noexcept {

			const auto Result = pCurrent;
			pCurrent = pCurrent->Prev;

			return pCurrent;
		}

		iterator_t operator+(const difference_type Offset) const noexcept {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				++it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				--it;
			}
			return it;
		}

		iterator_t& operator+=(const difference_type Offset) noexcept {
			for (difference_type i = 0; i < Offset; ++i) {
				++(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				--(*this);
			}
			return *this;
		}

		iterator_t operator-(const difference_type Offset) const noexcept {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				--it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				++it;
			}
			return it;
		}

		iterator_t& operator-=(const difference_type Offset) noexcept {
			for (difference_type i = 0; i < Offset; ++i) {
				--(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				++(*this);
			}
			return *this;
		}

		T operator->() const noexcept {
			return &(pCurrent->Data);
		}
		T& operator*() const noexcept {
			return pCurrent->Data;
		}

	private:
		Node* pCurrent;

	};

	template<typename T>
	class reverse_iterator_t : public std::bidirectional_iterator_tag {
		template <typename U, template<typename Ty> class TAlloc>
		friend class list;
	public:
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		reverse_iterator_t(Node* pCurrent) noexcept :pCurrent(pCurrent) {}

		template<typename U>
		reverse_iterator_t(const iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) noexcept {
			pCurrent = i.pCurrent;
		}
		template<typename U>
		reverse_iterator_t(const reverse_iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) noexcept {
			pCurrent = i.pCurrent;
		}

		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, reverse_iterator_t>& operator=(const reverse_iterator_t<U>& i) noexcept {
			pCurrent = i.pCurrent;
			return *this;
		}
		
		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, reverse_iterator_t>& operator=(const iterator_t<U>& i) noexcept {
			pCurrent = i.pCurrent;
			return *this;
		}

		template<typename U>
		bool operator==(const reverse_iterator_t<U>& i) const noexcept {
			return pCurrent == i.pCurrent;
		}

		template<typename U>
		bool operator!=(const reverse_iterator_t<U>& i) const noexcept {
			return pCurrent != i.pCurrent;

		}

		reverse_iterator_t& operator++() noexcept {

			pCurrent = pCurrent->Prev;

			return *this;
		}
		reverse_iterator_t operator++(int) noexcept {

			const auto Result = pCurrent;
			pCurrent = pCurrent->Prev;

			return pCurrent;
		}

		reverse_iterator_t& operator--() noexcept {

			pCurrent = pCurrent->Next;

			return *this;
		}

		reverse_iterator_t operator--(int) noexcept {

			const auto Result = pCurrent;
			pCurrent = pCurrent->Next;

			return pCurrent;
		}

		reverse_iterator_t operator+(const difference_type Offset) const noexcept {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				++it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				--it;
			}
			return it;
		}

		reverse_iterator_t& operator+=(const difference_type Offset) noexcept {
			for (difference_type i = 0; i < Offset; ++i) {
				++(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				--(*this);
			}
			return *this;
		}

		reverse_iterator_t operator-(const difference_type Offset) const noexcept {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				--it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				++it;
			}
			return it;
		}

		reverse_iterator_t& operator-=(const difference_type Offset) noexcept {
			for (difference_type i = 0; i < Offset; ++i) {
				--(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				++(*this);
			}
			return *this;
		}

		T operator->() const noexcept {
			return &(pCurrent->Data);
		}
		T& operator*() const noexcept {
			return pCurrent->Data;
		}

	private:
		Node* pCurrent;

	};


public:

	using iterator = iterator_t<T>;
	using const_iterator = iterator_t<const T>;
	using reverse_iterator = reverse_iterator_t<T>;
	using const_reverse_iterator = reverse_iterator_t<const T>;
	using value_type = T;
	using size_type = std::size_t;
	using reference = T&;
	using const_reference = const T&;
	using allocator_type = TAlloc<Node>;
	
	list() = default;
	
	template<typename U>
	explicit list(const TAlloc<U>& Alloc) :Alloc(Alloc) {}
	
	template<typename U>
	explicit list(TAlloc<U>&& Alloc) : Alloc(std::move(Alloc)) {}

	list(const list& other) : Alloc(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc)){
		for (const auto& i : other) emplace_back(i);
	}

	list& operator=(const list& other) {
		clear();
		Alloc = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc);
		for (const auto& i : other) emplace_back(i);
		return *this;
	}

	list(list&& other) noexcept : Count(other.Count), Head(other.Head), Tail(other.Tail), Alloc(std::move(other.Alloc)) {
		other.Count = 0;
		other.Head = nullptr;
		other.Tail = nullptr;
	}

	list& operator=(list&& other) noexcept {
		Count = other.Count;
		Head = other.Head;
		Tail = other.Tail;
		Alloc = std::move(other.Alloc);

		other.Count = 0;
		other.Head = nullptr;
		other.Tail = nullptr;

		return *this;
	}


	bool operator==(const list& other) const noexcept {
		if (size() != other.size()) return false;
		for (auto it1 = cbegin(), it2 = other.cbegin(); it1 != end(); ++it1, ++it2) {
			if (*it1 != *it2) return false;
		}
		return true;
	}

	bool operator!=(const list& other) const noexcept {
		return !(*this == other);
	}

	~list() noexcept {
		clear();
	}

	iterator begin() noexcept {
		return Head;
	}

	iterator end() noexcept {
		return nullptr;
	}

	const_iterator begin() const noexcept {
		return Head;
	}

	const_iterator end() const noexcept {
		return nullptr;
	}

	const_iterator cbegin() const noexcept {
		return Head;
	}

	const_iterator cend() const noexcept {
		return nullptr;
	}

	reverse_iterator rbegin() noexcept {
		return Tail;
	}

	reverse_iterator rend() noexcept {
		return nullptr;
	}

	const_reverse_iterator rbegin() const noexcept {
		return Tail;
	}

	const_reverse_iterator rend() const noexcept {
		return nullptr;
	}

	const_reverse_iterator crbegin() const noexcept {
		return Tail;
	}

	const_reverse_iterator crend() const noexcept {
		return nullptr;
	}


	reference front() noexcept {
		return Head->Data;
	}
	
	const_reference front() const noexcept {
		return Head->Data;
	}


	reference back() noexcept {
		return Tail->Data;
	}

	const_reference back() const noexcept {
		return Tail->Data;
	}

	bool empty() const noexcept {
		return size() == 0;
	}

	size_type size() const noexcept {
		return Count;
	}

	void push_front(const T& val) {
		emplace_front(val);
	}

	void push_back(const T& val) {
		emplace_back(val);
	}

	template<typename... TArgs>
	void emplace_front(TArgs&&... Args) {
		const auto pNode = Alloc.allocate(1);
		new (pNode) Node(std::forward<TArgs>(Args)...);
		if (Head != nullptr) {
			Head->Prev = pNode;
			pNode->Next = Tail;
			Head = pNode;
		}
		else {
			Head = Tail = pNode;
		}
		++Count;
	}
	
	template<typename... TArgs>
	void emplace_back(TArgs&&... Args) {
		const auto pNode = Alloc.allocate(1);
		new (pNode) Node(std::forward<TArgs>(Args)...);
		if (Tail != nullptr) {
			Tail->Next = pNode;
			pNode->Prev = Tail;
			Tail = pNode;
		}
		else {
			Head = Tail = pNode;
		}
		++Count;
	}

	template<typename... TArgs>
	iterator emplace(const_iterator it, TArgs&&... Args) {
		if (it == end()) {
			emplace_back(std::forward<TArgs>(Args)...);
			return Tail;
		}
		else if (it == begin()) {
			emplace_front(std::forward<TArgs>(Args)...);
			return begin();
		}
		else {

			const auto pNode = Alloc.allocate(1);
			new (pNode) Node(std::forward<TArgs>(Args)...);

			const auto Prev = it.pCurrent->Prev;
			pNode->Prev = Prev;
			Prev->Next = pNode;			

			pNode->Next = it.pCurrent;
			it.pCurrent->Prev = pNode;

			++Count;

			return pNode;
		}
	}
	
	void pop_front() noexcept {
		auto t = Head;
		Head = Head->Next;
		if (Head != nullptr) {
			Head->Prev = nullptr;
		}
		else {
			Tail = nullptr;
		}
		t->~Node();
		Alloc.deallocate(t, 1);

		--Count;
	}

	void pop_back() noexcept {
		auto t = Tail;
		Tail = Tail->Prev;
		if (Tail != nullptr) {
			Tail->Next = nullptr;
		}
		else {
			Head = nullptr;
		}
		t->~Node();
		Alloc.deallocate(t, 1);

		--Count;
	}

	iterator erase(const_iterator it) noexcept {
		if (it.pCurrent == Tail) {
			pop_back();
			return end();
		}
		else if (it == begin()) {
			pop_front();
			return begin();
		}
		else {

			const auto Next = it.pCurrent->Next;
			it.pCurrent->Prev->Next = it.pCurrent->Next;
			it.pCurrent->Next->Prev = it.pCurrent->Prev;

			it.pCurrent->~Node();
			Alloc.deallocate(it.pCurrent, 1);

			--Count;

			return Next;
		}
	}

		
	void clear() noexcept {
		while (!empty()) pop_back();
	}

	

private:
	Node* Head = nullptr;
	Node* Tail = nullptr;
	size_type Count = 0;
	TAlloc<Node> Alloc;

};

template <typename T_ID, size_t ChunkAllocatorNumElements = 1 << 16, template<typename Ty> class TAlloc = std::allocator>
using fast_list = list<T_ID, typename chunk_allocator_single_threaded_wrapper<ChunkAllocatorNumElements, TAlloc>::template allocator>;