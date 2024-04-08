#pragma once

#include <memory>
#include <array>

template<size_t TypeSize, size_t Alignment, template<typename Ty> class TAlloc, size_t ElementsPerChunk>
class chunk_allocator_typeless_single_threaded {
	struct pointer_and_size {
		pointer_and_size* pNext;
		size_t Size;
	};

	pointer_and_size* pFirstAllocatedChunk = nullptr;
	pointer_and_size* pLastAllocatedChunk = nullptr;

	struct data {
		data* pNext = nullptr;
	};

	struct T {
		alignas(Alignment) std::array<char, TypeSize> a;
	};

	data* FreeBegin = nullptr;

public:
	union allocated_type {
		data a;
		T b;
	};
private:

	static constexpr size_t ElementSize = sizeof(allocated_type);

	TAlloc<allocated_type> Allocator;

	template<typename T> static constexpr std::enable_if_t<std::is_unsigned<T>::value, T> UintDivideCeil(const T& a, const T& b) {
		return a / b + (((a % b) != 0) ? 1 : 0);
	}

	static constexpr auto Offset = UintDivideCeil(sizeof(pointer_and_size), ElementSize) * ElementSize;
	static constexpr size_t AllocSize = ElementsPerChunk + UintDivideCeil(sizeof(pointer_and_size), ElementSize);

	inline T* allocate_chunk(pointer_and_size*& rPointer) {
		pLastAllocatedChunk = rPointer = (pointer_and_size*)Allocator.allocate(AllocSize);
		if (rPointer == nullptr) [[unlikely]] {
			throw std::bad_alloc();
		}
		rPointer->Size = AllocSize * ElementSize - Offset - ElementSize;
		rPointer->pNext = nullptr;
		return (T*)(((char*)rPointer) + rPointer->Size + Offset);

	}
public:

	chunk_allocator_typeless_single_threaded(const TAlloc<allocated_type>& Alloc = TAlloc<allocated_type>()) : Allocator(Alloc){}
	chunk_allocator_typeless_single_threaded(const chunk_allocator_typeless_single_threaded& other) : Allocator(other.Allocator) {}
	chunk_allocator_typeless_single_threaded(chunk_allocator_typeless_single_threaded&& other) :pFirstAllocatedChunk(other.pFirstAllocatedChunk), pLastAllocatedChunk(other.pLastAllocatedChunk), Allocator(std::move(other.Allocator)), FreeBegin(other.FreeBegin){
		other.pFirstAllocatedChunk = nullptr;
		other.pLastAllocatedChunk = nullptr;
		other.FreeBegin = nullptr;
	}

	chunk_allocator_typeless_single_threaded& operator=(const chunk_allocator_typeless_single_threaded& other) {
		return *this;
	}
	chunk_allocator_typeless_single_threaded& operator=(chunk_allocator_typeless_single_threaded&& other) {
		Allocator = std::move(other.Allocator);

		pFirstAllocatedChunk = other.pFirstAllocatedChunk;
		pLastAllocatedChunk = other.pLastAllocatedChunk;
		FreeBegin = other.FreeBegin;

		other.pFirstAllocatedChunk = nullptr;
		other.pLastAllocatedChunk = nullptr;
		other.FreeBegin = nullptr;
		return *this;
	}


	~chunk_allocator_typeless_single_threaded() {
		auto pCurrent = pFirstAllocatedChunk;
		while (pCurrent != nullptr) {
			const auto pNext = pCurrent->pNext;

			Allocator.deallocate((allocated_type*)pCurrent, AllocSize);

			pCurrent = pNext;
		}

	}
	inline T* allocate() {

		if (FreeBegin != nullptr) {
			const auto Res = (T*)FreeBegin;
			FreeBegin = FreeBegin->pNext;
			return Res;
		}

		if (pLastAllocatedChunk != nullptr) {
			if (pLastAllocatedChunk->Size >= ElementSize) {
				pLastAllocatedChunk->Size -= ElementSize;

				return (T*)(((char*)pLastAllocatedChunk) + pLastAllocatedChunk->Size + Offset);

			}
			if (pLastAllocatedChunk->pNext == nullptr) {
				return allocate_chunk(pLastAllocatedChunk->pNext);
			}
		}

		return allocate_chunk(pFirstAllocatedChunk);

	}
	inline void deallocate(const void* pointer) {
		if (pointer == nullptr) return;
		const auto pData = ((data*)pointer);
		pData->pNext = FreeBegin;
		FreeBegin = pData;
	}

	inline T* allocate(size_t n) {
		if (n == 1) [[likely]] {
			return allocate();
		}
		else {
			return (T*)Allocator.allocate(n);
		}
	}

	inline void deallocate(const void* pointer, size_t n) {
		if (n == 1) [[likely]] {
			deallocate(pointer);
		}
		else {
			Allocator.deallocate((allocated_type*)pointer, n);
		}
	}
};

template<typename T, size_t ElementsPerChunk = 1 << 16, template<typename Ty> class TAlloc = std::allocator>
class chunk_allocator_single_threaded {

	chunk_allocator_typeless_single_threaded<sizeof(T), alignof(T), TAlloc, ElementsPerChunk> allocator;

public:

	using value_type = T;

	chunk_allocator_single_threaded() = default;
	template<typename U>
	chunk_allocator_single_threaded(const TAlloc<U>& Alloc) : allocator(Alloc) {}
	~chunk_allocator_single_threaded() = default;

	inline T* allocate(size_t n) {
		return (T*)allocator.allocate(n);
	}

	inline void deallocate(const T* pointer, size_t n) {
		allocator.deallocate(pointer, n);
	}

	inline T* allocate() {
		return (T*)allocator.allocate();
	}
	inline void deallocate(const void* pointer) {	
		allocator.deallocate(pointer);
	}


	template<typename... TArgs>
	inline T* construct(TArgs&&... Args) {
		const auto pointer = (T*)allocator.allocate(1);
		new (pointer) T(std::forward<TArgs>(Args)...);
		return pointer;
	}

	inline void destroy(const T* pointer) {
		if (pointer == nullptr) return;
		pointer->~T();
		allocator.deallocate(pointer);
	}

};



template<size_t ElementsPerChunk = 1 << 16, template<typename Ty> class TAlloc = std::allocator>
struct chunk_allocator_single_threaded_wrapper {
	template<typename T>
	using allocator = chunk_allocator_single_threaded<T, ElementsPerChunk, TAlloc>;
};




