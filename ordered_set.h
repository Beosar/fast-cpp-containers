#pragma once

#include "binarytree.h"
#include "chunk_allocator.h"

template <typename T_ID>
struct tree_set_helper {

	template<typename... TArgs>
	tree_set_helper(TArgs&&... Args) :Value(std::forward<TArgs>(Args)...) {}


	using key_type = T_ID;
	using value_type = const T_ID;

	value_type Value;

	inline const auto& GetKey() const {
		return Value;
	}
};

template <typename T_ID, typename Compare = std::less<T_ID>, template<typename Ty> class TAlloc = std::allocator>
using ordered_set = tree<tree_set_helper<T_ID>, Compare, TAlloc>;

template <typename T_ID, size_t ChunkAllocatorNumElements = 1 << 16, typename Compare = std::less<T_ID>, template<typename Ty> class TAlloc = std::allocator>
using fast_ordered_set = tree<tree_set_helper<T_ID>, Compare, typename chunk_allocator_single_threaded_wrapper<ChunkAllocatorNumElements, TAlloc>::template allocator>;