#pragma once

#include <utility>

template <typename T, typename Compare, template<typename Ty> class TAlloc>
class tree {
public:

	struct AvlNode {
		template<typename... TArgs>
		AvlNode(TArgs&&... Args) :Data(std::forward<TArgs>(Args)...) {}
		AvlNode* left = nullptr;
		AvlNode* right = nullptr;
		AvlNode* parent = nullptr;
		int height = 0;
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


		iterator_t(AvlNode* pCurrent) :pCurrent(pCurrent) {}

		template<typename U>
		iterator_t(const iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) {
			pCurrent = i.pCurrent;
		}

		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, iterator_t>& operator=(const iterator_t<U>& i) {
			pCurrent = i.pCurrent;
			return *this;
		}

		template<typename U>
		bool operator==(const iterator_t<U>& i) const {
			return pCurrent == i.pCurrent;
		}

		template<typename U>
		bool operator!=(const iterator_t<U>& i) const {
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

		iterator_t operator+(const difference_type Offset) const {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				++it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				--it;
			}
			return it;
		}

		iterator_t& operator+=(const difference_type Offset) {
			for (difference_type i = 0; i < Offset; ++i) {
				++(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				--(*this);
			}
			return *this;
		}

		iterator_t operator-(const difference_type Offset) const {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				--it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				++it;
			}
			return it;
		}

		iterator_t& operator-=(const difference_type Offset) {
			for (difference_type i = 0; i < Offset; ++i) {
				--(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				++(*this);
			}
			return *this;
		}

		T* operator->() const {
			return &(pCurrent->Data.Value);
		}
		T& operator*() const {
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


		reverse_iterator_t(AvlNode* pCurrent) :pCurrent(pCurrent) {}

		template<typename U>
		reverse_iterator_t(const iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) {
			pCurrent = i.pCurrent;
		}
		template<typename U>
		reverse_iterator_t(const reverse_iterator_t<U>& i, std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>>* = 0) {
			pCurrent = i.pCurrent;
		}

		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, reverse_iterator_t>& operator=(const reverse_iterator_t<U>& i) {
			pCurrent = i.pCurrent;
			return *this;
		}

		template<typename U>
		std::enable_if_t<std::is_const_v<T> || !std::is_const_v<U>, reverse_iterator_t>& operator=(const iterator_t<U>& i) {
			pCurrent = i.pCurrent;
			return *this;
		}

		template<typename U>
		bool operator==(const reverse_iterator_t<U>& i) const {
			return pCurrent == i.pCurrent;
		}

		template<typename U>
		bool operator!=(const reverse_iterator_t<U>& i) const {
			return pCurrent != i.pCurrent;

		}

		reverse_iterator_t& operator++() {

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

		reverse_iterator_t& operator--() {

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

		reverse_iterator_t operator++(int) {

			const auto Result = pCurrent;
			++(*this);

			return pCurrent;
		}

		reverse_iterator_t operator--(int) {

			const auto Result = pCurrent;
			--(*this);

			return pCurrent;
		}

		reverse_iterator_t operator+(const difference_type Offset) const {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				++it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				--it;
			}
			return it;
		}

		reverse_iterator_t& operator+=(const difference_type Offset) {
			for (difference_type i = 0; i < Offset; ++i) {
				++(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				--(*this);
			}
			return *this;
		}

		reverse_iterator_t operator-(const difference_type Offset) const {
			auto it = *this;
			for (difference_type i = 0; i < Offset; ++i) {
				--it;
			}
			for (difference_type i = 0; i > Offset; --i) {
				++it;
			}
			return it;
		}

		reverse_iterator_t& operator-=(const difference_type Offset) {
			for (difference_type i = 0; i < Offset; ++i) {
				--(*this);
			}
			for (difference_type i = 0; i > Offset; --i) {
				++(*this);
			}
			return *this;
		}


		T* operator->() const {
			return &(pCurrent->Data.Value);
		}
		T& operator*() const {
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
	explicit tree(TAlloc<U>&& Alloc) : Alloc(std::move(Alloc)) {}

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

	tree(tree&& other) : comp(std::move(other.comp)), Count(other.Count), root(other.root), Alloc(std::move(other.Alloc)) {
		other.Count = 0;
		other.root = nullptr;
	}

	tree& operator=(tree&& other) {
		Count = other.Count;
		root = other.root;
		Alloc = std::move(other.Alloc);
		comp = std::move(other.comp);

		other.Count = 0;
		other.root = nullptr;

		return *this;
	}

	~tree(){
		clear();
	}

	bool operator==(const tree& other) const {
		if (size() != other.size()) return false;
		for (auto it1 = cbegin(), it2 = other.cbegin(); it1 != end(); ++it1, ++it2) {
			if (*it1 != *it2) return false;
		}
		return true;
	}

	bool operator!=(const tree& other) const {
		return !(*this == other);
	}

	bool empty() const {
		return root == nullptr;
	}


	iterator begin(){
		auto Current = root;
		if (Current != nullptr) {
			while (Current->left != nullptr) {
				Current = Current->left;
			}
		}
		return Current;
	}

	iterator end() {
		return nullptr;
	}

	const_iterator begin() const {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->left != nullptr) {
				Current = Current->left;
			}
		}
		return Current;
	}

	const_iterator end() const {
		return nullptr;
	}

	const_iterator cbegin() const {
		return begin();
	}

	const_iterator cend() const {
		return end();
	}
	
	reverse_iterator rbegin() {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->right != nullptr) {
				Current = Current->right;
			}
		}
		return Current;
	}

	reverse_iterator rend() {
		return nullptr;
	}

	const_reverse_iterator rbegin() const {
		auto Current = root;
		if (Current != nullptr) {
			while (Current->right != nullptr) {
				Current = Current->right;
			}
		}
		return Current;
	}

	const_reverse_iterator rend() const {
		return nullptr;
	}

	const_reverse_iterator crbegin() const {
		return begin();
	}

	const_reverse_iterator crend() const {
		return end();
	}


	template<typename... TArgs>
	std::pair<iterator,bool> emplace(TArgs&&... Args){
		std::pair<iterator, bool> result(nullptr, false);
		
		const auto pNode = Alloc.allocate(1);
		new (pNode) AvlNode(std::forward<TArgs>(Args)...);

		root = Insert(pNode, nullptr, root, result);
		
		if (!result.second) {
			pNode->~AvlNode();
			Alloc.deallocate(pNode, 1);
		}

		return result;
	}

	iterator erase(const iterator Where) {
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

	size_t erase(const typename T::key_type& x) {
		const auto SizeBefore = size();
		root = DeleteNode(root, x);
		
		return SizeBefore - size();
	}

	void clear(){
		MakeEmpty(root);
		root = nullptr;
		Count = 0;

	}

	size_t size() const {
		return Count;
	}

#ifdef _DEBUG

	void check_tree() const {
		const auto CheckTreeRes = check_tree(root, nullptr);
		assert(CheckTreeRes == size());
	}

#endif

private:

#ifdef _DEBUG

	size_t check_tree(AvlNode* t, AvlNode* parent) const {
		if (t == nullptr) return 0;
		assert(t->parent == parent);

		if (t->left != nullptr) {
			assert(comp(t->left->Data.GetKey(), t->Data.GetKey()));
		}

		if (t->right != nullptr) {
			assert(comp(t->Data.GetKey(), t->right->Data.GetKey()));
		}

		assert(t->height == std::max(Height(t->left), Height(t->right)) + 1);
		const auto balance = GetBalance(t);
		assert(balance >= -1 && balance <= 1);

		return check_tree(t->left, t) + check_tree(t->right, t) + 1;
	}

#endif


	AvlNode* Insert(AvlNode* const pNode, AvlNode* const parent, AvlNode* const t, std::pair<iterator, bool>& result){
		if (t == nullptr){		
			pNode->parent = parent;
			Count++;
			result.first = pNode;
			result.second = true;
			return pNode;
		}
		if (comp(pNode->Data.GetKey(), t->Data.GetKey())){
			t->left = Insert(pNode, t, t->left, result);
		}
		else if (comp(t->Data.GetKey(), pNode->Data.GetKey())){
			t->right = Insert(pNode, t, t->right, result);
		}
		else{
			result.first = t;
			result.second = false;
			return t;
		}

		if (!result.second) return t;

		
		t->height = std::max(Height(t->left), Height(t->right)) + 1;

		const int balance = GetBalance(t);

		
		if (balance > 1 && comp(pNode->Data.GetKey(), t->left->Data.GetKey()))
			return RightRotate(t);

		if (balance < -1 && comp(t->right->Data.GetKey(), pNode->Data.GetKey()))
			return LeftRotate(t);

		if (balance > 1 && comp(t->left->Data.GetKey(), pNode->Data.GetKey()))
		{
			t->left = LeftRotate(t->left);
			return RightRotate(t);
		}

		if (balance < -1 && comp(pNode->Data.GetKey(), t->right->Data.GetKey()))
		{
			t->right = RightRotate(t->right);
			return LeftRotate(t);
		}

		return t;
	}

	static int Height(AvlNode* t){
		return t == 0 ? -1 : t->height;
	}

	AvlNode* LeftRotate(AvlNode* t){

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

		t->height = std::max(Height(t->left), Height(t->right)) + 1;
		y->height = std::max(Height(y->left), Height(y->right)) + 1;

		return y;
	}

	AvlNode* RightRotate(AvlNode* t){

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

		t->height = std::max(Height(t->left), Height(t->right)) + 1;
		x->height = std::max(Height(x->left), Height(x->right)) + 1;

		return x;
	}
	

	AvlNode* FindMin(AvlNode* t){
		if (t == 0)
			return t;

		while (t->left != nullptr){
			t = t->left;
		}
		return t;
	}

	AvlNode* FindMax(AvlNode* t){
		if (t == 0)
			return t;

		while (t->right != nullptr){
			t = t->right;
		}
		return t;
	}

	iterator Find(const typename T::key_type& x, AvlNode* t) const{
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

	iterator LowerBound(const typename T::key_type& x, AvlNode* t) const {
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
	
	iterator UpperBound(const typename T::key_type& x, AvlNode* t) const {
		auto it = LowerBound(x, t);
		if (it != end() && !comp(x, it.pCurrent->Data.GetKey())) ++it;
		return it;
	}


	AvlNode* UpdateOnDelete(AvlNode* t) {
		t->height = std::max(Height(t->left), Height(t->right)) + 1;

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


	AvlNode* DeleteNodeUnchecked(AvlNode* t) {
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

			assert(t->left == nullptr);

			const bool UpdatedPTRight = temp->right != t;

			if (UpdatedPTRight) {		
				
				assert(t->parent->left == t);

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

	AvlNode* DeleteNode(AvlNode* t, const typename T::key_type& ID){
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
	static int GetBalance(AvlNode* t){
		if (t == nullptr)
			return 0;
		return Height(t->left) - Height(t->right);
	}
	void MakeEmpty(AvlNode* t){
		if (t == nullptr) return;
		MakeEmpty(t->left);
		MakeEmpty(t->right);
		t->~AvlNode();
		Alloc.deallocate(t, 1);
	}

public:

	iterator find(const typename T::key_type& x) {
		return Find(x, root);
	}
	const_iterator find(const typename T::key_type& x) const {
		return Find(x, root);
	}

	iterator lower_bound(const typename T::key_type& x) {
		return LowerBound(x, root);
	}
		
	const_iterator lower_bound(const typename T::key_type& x) const {
		return LowerBound(x, root);
	}

	iterator upper_bound(const typename T::key_type& x) {
		return UpperBound(x, root);
	}

	const_iterator upper_bound(const typename T::key_type& x) const {
		return UpperBound(x, root);
	}

private:

	Compare comp;

	AvlNode* root = nullptr;
	size_t Count = 0;

	TAlloc<AvlNode> Alloc;
	
};




