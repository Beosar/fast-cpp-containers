#pragma once

#include <utility>
#include <iterator>
#include <memory>

template <typename T, typename Compare, template<typename Ty> class TAlloc>
class tree {
public:

	struct AvlNode {
		template<typename... TArgs>
		AvlNode(TArgs&&... Args) :Data(std::forward<TArgs>(Args)...) {}
		AvlNode* left = nullptr;
		AvlNode* right = nullptr;
		AvlNode* parent = nullptr;
		int16_t heightLeft = 0, heightRight = 0; //height of child + 1
		T Data;
	};

private:
	template<typename T>
	class iterator_t : public std::bidirectional_iterator_tag {
		template <typename U, typename Compare, template<typename Ty> class TAlloc>
		friend class tree;
	public:

		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;


		iterator_t(AvlNode* pCurrent) noexcept :pCurrent(pCurrent) {}

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

		iterator_t& operator++() {

			if (pCurrent->right != nullptr) {
				pCurrent = pCurrent->right;
				while (pCurrent->left != nullptr) {
					pCurrent = pCurrent->left;
				}
			}
			else {
				while (true) {
					auto Prev = pCurrent;
					pCurrent = pCurrent->parent;
					if (pCurrent != nullptr) {
						if (pCurrent->left == Prev) {
							//We came from the left, so parent node is the next node.
							break;
						}

						//We came from the right, need to go up higher.

					}
					else {
						break;
					}
				}
			}

			return *this;
		}


		iterator_t& operator--() {

			if (pCurrent->left != nullptr) {
				pCurrent = pCurrent->left;
				while (pCurrent->right != nullptr) {
					pCurrent = pCurrent->right;
				}
			}
			else {
				while (true) {
					auto Prev = pCurrent;
					pCurrent = pCurrent->parent;
					if (pCurrent != nullptr) {
						if (pCurrent->right == Prev) {
							//We came from the right, so parent node is the next node.
							break;
						}

						//We came from the left, need to go up higher.

					}
					else {
						break;
					}
				}
			}

			return *this;
		}

		iterator_t operator++(int) {

			const auto Result = pCurrent;
			++(*this);

			return pCurrent;
		}

		iterator_t operator--(int) {

			const auto Result = pCurrent;
			--(*this);

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

		T* operator->() const noexcept {
			return &(pCurrent->Data.Value);
		}
		T& operator*() const noexcept {
			return pCurrent->Data.Value;
		}

	private:
		AvlNode* pCurrent;

	};


	template<typename T>
	class reverse_iterator_t : public std::bidirectional_iterator_tag {
		template <typename U, typename Compare, template<typename Ty> class TAlloc>
		friend class tree;
	public:

		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;


		reverse_iterator_t(AvlNode* pCurrent) noexcept :pCurrent(pCurrent) {}

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

			if (pCurrent->left != nullptr) {
				pCurrent = pCurrent->left;
				while (pCurrent->right != nullptr) {
					pCurrent = pCurrent->right;
				}
			}
			else {
				while (true) {
					auto Prev = pCurrent;
					pCurrent = pCurrent->parent;
					if (pCurrent != nullptr) {
						if (pCurrent->right == Prev) {
							//We came from the right, so parent node is the next node.
							break;
						}

						//We came from the left, need to go up higher.

					}
					else {
						break;
					}
				}
			}

			return *this;
		}

		reverse_iterator_t& operator--() noexcept {

			if (pCurrent->right != nullptr) {
				pCurrent = pCurrent->right;
				while (pCurrent->left != nullptr) {
					pCurrent = pCurrent->left;
				}
			}
			else {
				while (true) {
					auto Prev = pCurrent;
					pCurrent = pCurrent->parent;
					if (pCurrent != nullptr) {
						if (pCurrent->left == Prev) {
							//We came from the left, so parent node is the next node.
							break;
						}

						//We came from the right, need to go up higher.

					}
					else {
						break;
					}
				}
			}

			return *this;
		}

		reverse_iterator_t operator++(int) noexcept {

			const auto Result = pCurrent;
			++(*this);

			return pCurrent;
		}

		reverse_iterator_t operator--(int) noexcept {

			const auto Result = pCurrent;
			--(*this);

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


		T* operator->() const noexcept {
			return &(pCurrent->Data.Value);
		}
		T& operator*() const noexcept {
			return pCurrent->Data.Value;
		}

	private:
		AvlNode* pCurrent;

	};

public:
	using value_type = typename T::value_type;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using key_compare = Compare;
	using allocator_type = TAlloc<AvlNode>;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using node_type = AvlNode;
	using iterator = iterator_t<value_type>;
	using const_iterator = iterator_t<const value_type>;
	using reverse_iterator = reverse_iterator_t<value_type>;
	using const_reverse_iterator = reverse_iterator_t<const value_type>;

	tree(){}

	explicit tree(const Compare& comp) :comp(comp) {}
	
	template<typename U>
	explicit tree(const Compare& comp, const TAlloc<U>& alloc)
	: comp(comp), Alloc(alloc){}
	
	template<typename U>
	explicit tree(const TAlloc<U>& alloc)
		: Alloc(alloc) {}

	template<typename U>
	explicit tree(TAlloc<U>&& Alloc) noexcept : Alloc(std::move(Alloc)) {}

	tree(const tree& other) : comp(other.comp), Alloc(std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc)) {
		for (const auto& i : other) emplace(i);
	}

	tree& operator=(const tree& other) {
		clear();
		Alloc = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.Alloc);
		comp = other.comp;
		for (const auto& i : other) emplace(i);
		return *this;
	}

	tree(tree&& other) noexcept : comp(std::move(other.comp)), Count(other.Count), root(other.root), Alloc(std::move(other.Alloc)) {
		other.Count = 0;
		other.root = nullptr;
	}

	tree& operator=(tree&& other) noexcept {
		Count = other.Count;
		root = other.root;
		Alloc = std::move(other.Alloc);
		comp = std::move(other.comp);

		other.Count = 0;
		other.root = nullptr;

		return *this;
	}

	~tree() noexcept {
		clear();
	}

	bool operator==(const tree& other) const noexcept {
		if (size() != other.size()) return false;
		for (auto it1 = cbegin(), it2 = other.cbegin(); it1 != end(); ++it1, ++it2) {
			if (*it1 != *it2) return false;
		}
		return true;
	}

	bool operator!=(const tree& other) const noexcept {
		return !(*this == other);
	}

	bool empty() const noexcept {
		return root == nullptr;
	}


	iterator begin() noexcept {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->left != nullptr) {
				Current = Current->left;
			}
		}
		return Current;
	}

	iterator end() noexcept {
		return nullptr;
	}

	const_iterator begin() const noexcept {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->left != nullptr) {
				Current = Current->left;
			}
		}
		return Current;
	}

	const_iterator end() const noexcept {
		return nullptr;
	}

	const_iterator cbegin() const noexcept {
		return begin();
	}

	const_iterator cend() const noexcept {
		return end();
	}
	
	reverse_iterator rbegin() noexcept {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->right != nullptr) {
				Current = Current->right;
			}
		}
		return Current;
	}

	reverse_iterator rend() noexcept {
		return nullptr;
	}

	const_reverse_iterator rbegin() const noexcept {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->right != nullptr) {
				Current = Current->right;
			}
		}
		return Current;
	}

	const_reverse_iterator rend() const noexcept {
		return nullptr;
	}

	const_reverse_iterator crbegin() const noexcept {
		return begin();
	}

	const_reverse_iterator crend() const noexcept {
		return end();
	}


	template<typename... TArgs>
	std::pair<iterator,bool> emplace(TArgs&&... Args){
		
		const auto pNode = Alloc.allocate(1);
		new (pNode) AvlNode(std::forward<TArgs>(Args)...);

		auto result = Insert(pNode);
		
		if (!result.second) {
			pNode->~AvlNode();
			Alloc.deallocate(pNode, 1);
		}

		return result;
	}

	iterator erase(const iterator Where) noexcept {
		auto Result = Where;
		++Result;
		if (Where.pCurrent->parent != nullptr) {
			auto pCurrent = Where.pCurrent->parent;

			(pCurrent->left == Where.pCurrent ? pCurrent->left : pCurrent->right) = DeleteNodeUnchecked(Where.pCurrent);

			while (pCurrent != nullptr) {
				auto& rpNode = pCurrent->parent != nullptr ? (pCurrent->parent->left == pCurrent ? pCurrent->parent->left : pCurrent->parent->right) : root;
				rpNode = UpdateOnDelete(pCurrent);
				pCurrent = pCurrent->parent;
			}
		}
		else {
			root = DeleteNodeUnchecked(Where.pCurrent);
		}
		return Result;
	}

	size_t erase(const typename T::key_type& x) noexcept {
		const auto SizeBefore = size();
		root = DeleteNode(root, x);
		
		return SizeBefore - size();
	}

	void clear() noexcept {
		MakeEmpty(root);
		root = nullptr;
		Count = 0;

	}

	size_t size() const noexcept {
		return Count;
	}

private:

	std::pair<iterator, bool> Insert(AvlNode* const pNode) {

		
		auto t = root;
		std::array<AvlNode**, 64> Stack; //parent, pointer to child pointer
		auto pStack = Stack.data();
		AvlNode* parent = nullptr;

		while (t != nullptr) {
			if (comp(pNode->Data.GetKey(), t->Data.GetKey())) {
				parent = t;
				*pStack = &t->left;
				++pStack;
				t = t->left;

			}
			else if (comp(t->Data.GetKey(), pNode->Data.GetKey())) {
				parent = t;
				*pStack = &t->right;
				++pStack;
				t = t->right;
			}
			else {
				return { t, false };
			}
		}

		pNode->parent = parent;
		Count++;

		t = pNode;
		--pStack;
		for (; pStack >= Stack.data(); --pStack) {

			**pStack = t;
			t = t->parent;

			const auto isLeft = *pStack == &t->left;
			auto& rHeight = isLeft ? t->heightLeft : t->heightRight;

			const auto height = rHeight;
			rHeight = Height(**pStack) + 1;
			const bool HeightChanged = rHeight != height;
			
			const int balance = GetBalance(t);


			if (balance > 1) {

				if (comp(t->left->Data.GetKey(), pNode->Data.GetKey())){
					t->left = LeftRotate(t->left);
				}
				t = RightRotate(t);
				
			}
			else if (balance < -1){
				if (comp(pNode->Data.GetKey(), t->right->Data.GetKey())) {
					t->right = RightRotate(t->right);
				}
				t = LeftRotate(t);
			}
			else if (!HeightChanged) {
				return { pNode, true };
			}



		}

		root = t;

		return { pNode, true };

	}

	static int16_t Height(AvlNode* t) noexcept {
		return t == 0 ? -1 : std::max(t->heightLeft, t->heightRight);
	}

	static AvlNode* LeftRotate(AvlNode* t) noexcept {

		const auto y = t->right;
		const auto T2 = y->left;
		const auto parent = t->parent;

		y->left = t;
		t->parent = y;

		t->right = T2;
		if (T2) {
			T2->parent = t;
		}

		y->parent = parent;

		UpdateHeightRight(t);
		UpdateHeightLeft(y);

		return y;
	}

	static AvlNode* RightRotate(AvlNode* t) noexcept {

		const auto x = t->left;
		const auto T2 = x->right;
		const auto parent = t->parent;

		x->right = t;
		t->parent = x;

		t->left = T2;
		if (T2) {
			T2->parent = t;
		}

		x->parent = parent;

		UpdateHeightLeft(t);
		UpdateHeightRight(x);

		return x;
	}
	

	static AvlNode* FindMin(AvlNode* t) noexcept {
		if (t == 0)
			return t;

		while (t->left != nullptr){
			t = t->left;
		}
		return t;
	}

	static AvlNode* FindMax(AvlNode* t) noexcept {
		if (t == 0)
			return t;

		while (t->right != nullptr){
			t = t->right;
		}
		return t;
	}

	iterator Find(const typename T::key_type& x, AvlNode* t) const noexcept {
		while (t != 0){
			if (comp(x, t->Data.GetKey()))
				t = t->left;
			else if (comp(t->Data.GetKey(), x))
				t = t->right;
			else
				return t;
		}
		return nullptr;
	}

	iterator LowerBound(const typename T::key_type& x, AvlNode* t) const noexcept {
		while (t != 0) {
			if (comp(x, t->Data.GetKey())) {
				if (t->left != nullptr) {
					t = t->left;
				}
				else {
					return t;
				}
			}
			else if (comp(t->Data.GetKey(), x)) {
				if (t->right != nullptr) {
					t = t->right;
				}
				else {
					iterator res = t;
					++res;
					return res;
				}
			}
			else {
				return t;
			}
		}
		return nullptr;
	}
	
	iterator UpperBound(const typename T::key_type& x, AvlNode* t) const noexcept {
		auto it = LowerBound(x, t);
		if (it != end() && !comp(x, it.pCurrent->Data.GetKey())) ++it;
		return it;
	}


	static AvlNode* UpdateOnDelete(AvlNode* t) noexcept {
		UpdateHeight(t);

		const int balance = GetBalance(t);

		if (balance > 1 && GetBalance(t->left) >= 0)
			return RightRotate(t);

		if (balance > 1 && GetBalance(t->left) < 0) {
			t->left = LeftRotate(t->left);
			return RightRotate(t);
		}

		if (balance < -1 && GetBalance(t->right) <= 0)
			return LeftRotate(t);

		if (balance < -1 && GetBalance(t->right) > 0) {
			t->right = RightRotate(t->right);
			return LeftRotate(t);
		}

		return t;
	}


	AvlNode* DeleteNodeUnchecked(AvlNode* t) noexcept {
		if (t->left == nullptr || t->right == nullptr) {
			AvlNode* temp = t;

			t = t->left ? t->left : t->right;
			if (t != nullptr) {
				t->parent = temp->parent;
			}

			Count--;
			temp->~AvlNode();
			Alloc.deallocate(temp, 1);

			if (t == nullptr) return nullptr;
		}
		else {

			AvlNode* temp = t;

			t = FindMin(t->right);

			const bool UpdatedPTRight = temp->right != t;

			if (UpdatedPTRight) {		

				t->parent->left = t->right;
				if (t->parent->left != nullptr) {
					t->parent->left->parent = t->parent;
				}

				t->right = temp->right;
				t->right->parent = t;
			}

			t->left = temp->left;
			if (t->left != nullptr) {
				t->left->parent = t;
			}

			
			t->parent = temp->parent;

			

			if (UpdatedPTRight && temp->right != nullptr) {
				auto pCurrent = FindMin(temp->right);
				while (pCurrent != t) {
					auto& rpNode = pCurrent->parent->left == pCurrent ? pCurrent->parent->left : pCurrent->parent->right;
					rpNode = UpdateOnDelete(pCurrent);
					pCurrent = pCurrent->parent;
				}
			}

			Count--;
			temp->~AvlNode();
			Alloc.deallocate(temp, 1);

		}

		return UpdateOnDelete(t);

	}

	AvlNode* DeleteNode(AvlNode* t, const typename T::key_type& ID)noexcept {
		if (t == nullptr)
			return t;

		if (comp(ID, t->Data.GetKey()))
			t->left = DeleteNode(t->left, ID);

		else if (comp(t->Data.GetKey(), ID))
			t->right = DeleteNode(t->right, ID);

		else{
			return DeleteNodeUnchecked(t);
			
		}		

		return UpdateOnDelete(t);
	}
	static int16_t GetBalance(AvlNode* t) noexcept {
		if (t == nullptr)
			return 0;
		return t->heightLeft - t->heightRight;
	}
	void MakeEmpty(AvlNode* t) noexcept {
		if (t == nullptr) return;
		MakeEmpty(t->left);
		MakeEmpty(t->right);
		t->~AvlNode();
		Alloc.deallocate(t, 1);
	}

	static inline void UpdateHeightLeft(AvlNode* t) {
		t->heightLeft = Height(t->left) + 1;
	}
	static inline void UpdateHeightRight(AvlNode* t) {
		t->heightRight= Height(t->right) + 1;
	}

	static inline void UpdateHeight(AvlNode* t) {
		t->heightLeft = Height(t->left) + 1;
		t->heightRight = Height(t->right) + 1;
	}

public:

	iterator find(const typename T::key_type& x) noexcept {
		return Find(x, root);
	}
	const_iterator find(const typename T::key_type& x) const noexcept {
		return Find(x, root);
	}

	iterator lower_bound(const typename T::key_type& x) noexcept {
		return LowerBound(x, root);
	}
		
	const_iterator lower_bound(const typename T::key_type& x) const noexcept {
		return LowerBound(x, root);
	}

	iterator upper_bound(const typename T::key_type& x) noexcept {
		return UpperBound(x, root);
	}

	const_iterator upper_bound(const typename T::key_type& x) const noexcept {
		return UpperBound(x, root);
	}

private:

	Compare comp;

	AvlNode* root = nullptr;
	size_t Count = 0;

	TAlloc<AvlNode> Alloc;
	
};




