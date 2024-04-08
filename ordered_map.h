#pragma once

#include "binarytree.h"
#include "chunk_allocator.h"

template <typename T_ID, typename T_Data>
struct tree_map_helper {

	template<typename... TArgs>
	tree_map_helper(TArgs&&... Args) :Value(std::forward<TArgs>(Args)...) {}


	using key_type = T_ID;
	using value_type = std::pair<const T_ID, T_Data>;

	value_type Value;

	inline const auto& GetKey() const {
		return Value.first;
	}
};

template <typename T_ID, typename T_Data, typename Compare = std::less<T_ID>, template<typename Ty> class TAlloc = std::allocator>
class tree_map : public tree<tree_map_helper< T_ID, T_Data>, Compare, TAlloc> {
public:
	T_Data& operator[](const T_ID& ID) {
		const auto it = tree<tree_map_helper< T_ID, T_Data>, Compare, TAlloc>::find(ID);
		if (it != tree<tree_map_helper< T_ID, T_Data>, Compare, TAlloc>::end()) return it->second;
		return tree<tree_map_helper< T_ID, T_Data>, Compare, TAlloc>::emplace(ID, T_Data()).first->second;
	}
};

template <typename T_ID, typename T_Data, typename Compare = std::less<T_ID>, template<typename Ty> class TAlloc = std::allocator>
using ordered_map = tree_map<T_ID, T_Data, Compare, TAlloc>;

template <typename T_ID, typename T_Data, size_t ChunkAllocatorNumElements = 1 << 16, typename Compare = std::less<T_ID>, template<typename Ty> class TAlloc = std::allocator>
using fast_ordered_map = tree_map<T_ID, T_Data, Compare, typename chunk_allocator_single_threaded_wrapper<ChunkAllocatorNumElements, TAlloc>::template allocator>;