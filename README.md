# fast-cpp-containers
C++ AVL-tree map/set, list, and queue with a very fast memory pool allocator.

Usage and iterator validity are identical to the respective C++ standard library containers (`map`, `set`, `list`, and `queue`).

Use `fast_ordered_map`, `fast_ordered_set`, `fast_list`, and `fast_queue` to use those containers with a fast allocator that allocates 2<sup>16</sup> elements at once by default. Whenever the allocated memory is insufficient, another chunk is allocated.

**IMPORTANT:** The memory allocator in "chunk_allocator.h" is **NOT** a general purpose allocator. Its interface is similar to C++ allocators only for compatibility with the containers in this library.

The allocator is **NOT** thread-safe. Concurrent access is undefined behavior.

An instance of the allocator owns the allocated memory and can be used as a stand-alone memory pool. Only allocations of single elements use the memory pool, an allocation of multiple elements uses the underlying allocator, in which case the memory is not owned by the instance of the allocator.

## Container usage example

```
fast_ordered_map<uint64_t, int> m;
m[0] = 5;
m.erase(m.begin());
```

## Memory pool usage example

```
chunk_allocator_single_threaded<int> MemoryPool;
int* pData = MemoryPool.construct(42);
std::cout << *pData << std::endl;
MemoryPool.destroy(pData);
```
