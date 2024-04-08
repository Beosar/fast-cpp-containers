#pragma once

#include "chunk_allocator.h"

template<typename T, template<typename Ty> class TAlloc = std::allocator>
class queue {
	struct Node {
		template<typename... TArgs>
		Node(TArgs&&... Args) :Data(std::forward<TArgs>(Args)...) {}

		Node* Next = nullptr;
		T Data;
	};
public:
	queue() = default;
	
	template<typename U>
	explicit queue(const TAlloc<U>& Alloc):Alloc(Alloc){}
	
	template<typename U>
	explicit queue(TAlloc<U>&& Alloc) : Alloc(std::move(Alloc)) {}
	
	queue(const queue& other) : Alloc(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc)) {
		auto Current = other.Head;
		while (Current != nullptr) {
			emplace(Current->Data);
			Current = Current->Next;
		}
	}

	queue& operator=(const queue& other) {
		clear();
		Alloc = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc);
		auto Current = other.Head;
		while (Current != nullptr) {
			emplace(Current->Data);
			Current = Current->Next;
		}
		return *this;
	}

	queue(queue&& other) : Count(other.Count), Head(other.Head), Tail(other.Tail), Alloc(std::move(other.Alloc)) {
		other.Count = 0;
		other.Head = nullptr;
		other.Tail = nullptr;
	}

	queue& operator=(queue&& other) {
		Count = other.Count;
		Head = other.Head;
		Tail = other.Tail;
		Alloc = std::move(other.Alloc);

		other.Count = 0;
		other.Head = nullptr;
		other.Tail = nullptr;

		return *this;
	}

	~queue() {
		clear();
	}

	using value_type = T;
	using size_type = std::size_t;
	using reference = T&;
	using const_reference = const T&;
	using node_type = Node;
	using allocator_type = TAlloc<Node>;

	reference front() {
		return Head->Data;
	}
	
	const_reference front() const{
		return Head->Data;
	}


	reference back() {
		return Tail->Data;
	}

	const_reference back() const {
		return Tail->Data;
	}

	bool empty() const {
		return size() == 0;
	}

	size_type size() const {
		return Count;
	}

	void push(const T& val) {
		emplace(val);
	}

	template<typename... TArgs>
	void emplace(TArgs&&... Args) {
		const auto pNode = Alloc.allocate(1);
		new (pNode) Node(std::forward<TArgs>(Args)...);
		if (Head != nullptr) {
			Tail->Next = pNode;
			Tail = pNode;
		}
		else {
			Head = Tail = pNode;
		}
		++Count;
	}
	
	void pop() {
		auto t = Head;
		Head = Head->Next;
		t->~Node();
		Alloc.deallocate(t, 1);

		--Count;
	}
		
	void clear() {
		while (!empty()) pop();
	}


private:
	Node* Head = nullptr;
	Node* Tail = nullptr;
	size_type Count = 0;
	TAlloc<Node> Alloc;

};

template <typename T_ID, size_t ChunkAllocatorNumElements = 1 << 16, template<typename Ty> class TAlloc = std::allocator>
using fast_queue = queue<T_ID, typename chunk_allocator_single_threaded_wrapper<ChunkAllocatorNumElements, TAlloc>::template allocator>;