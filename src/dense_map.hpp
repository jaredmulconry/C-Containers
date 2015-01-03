#if !defined(DENSE_MAP_HPP)
#define DENSE_MAP_HPP
#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace custom_containers
{
	template<class Cont>
	struct DenseMapConstIterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using MyType = DenseMapConstIterator<Cont>;
		using key_type = typename Cont::key_type;
		using mapped_type = typename Cont::mapped_type;
		using difference_type = typename Cont::difference_type;
		using key_reference = typename Cont::key_const_reference;
		using value_reference = typename Cont::value_const_reference;
		using key_pointer = typename Cont::key_const_pointer;
		using value_pointer = typename Cont::value_const_pointer;
		using reference = typename Cont::const_reference;
		using pointer = typename Cont::const_pointer;
		using value_type = typename Cont::value_type;
		using Kpointer = typename Cont::KIterator;
		using Vpointer = typename Cont::VIterator;
	protected:
		Kpointer key;
		Vpointer value;
	public:
		DenseMapConstIterator() noexcept = default;
		DenseMapConstIterator(const MyType&) noexcept = default;
		~DenseMapConstIterator() = default;
		DenseMapConstIterator(Kpointer k, Vpointer v)
			:key(k)
			, value(v)
		{}
		template<typename I, class =
			std::enable_if_t < std::is_base_of<MyType, I>::value >>
		explicit DenseMapConstIterator(const I& i) noexcept
			: MyType(static_cast<const MyType&>(i))
		{}
		DenseMapConstIterator& operator=(const MyType&) noexcept = default;

		reference operator*() const
		{
			return reference(key_reference(*key), value_reference(*value));
		}

		MyType& operator++()
		{
			++this->key;
			++this->value;
			return *this;
		}
		MyType operator++(int)
		{
			MyType tmp = *this;
			++*this;
			return tmp;
		}
		MyType& operator--()
		{
			--this->key;
			--this->value;
			return *this;
		}
		MyType operator--(int)
		{
			MyType tmp = *this;
			--*this;
			return tmp;
		}

		MyType& operator+=(difference_type n)
		{
			difference_type m = n;
			if (m >= 0)
				while (m--)
					++*this;
			else
				while (m++)
					--*this;
			return *this;
		}
		MyType& operator-=(difference_type n)
		{
			return *this += -n;
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);
		}

		inline
			MyType operator+(difference_type n) const
		{
			MyType tmp = *this;
			return tmp += n;
		}
		inline
			MyType operator-(difference_type n) const
		{
			MyType tmp = *this;
			return tmp -= n;
		}
		inline friend
			difference_type operator-(const MyType& x, const MyType& y)
		{
			return x.key - y.key;
		}

		inline friend
			bool operator==(const MyType& x, const MyType& y)
		{
			return x.key == y.key;
		}
		inline friend
			bool operator!=(const MyType& x, const MyType& y)
		{
			return !(x == y);
		}
		inline friend
			bool operator<(const MyType& x, const MyType& y)
		{
			return x.key < y.key;
		}
		inline friend
			bool operator>(const MyType& x, const MyType& y)
		{
			return y < x;
		}
		inline friend
			bool operator<=(const MyType& x, const MyType& y)
		{
			return !(y < x);
		}
		inline friend
			bool operator>=(const MyType& x, const MyType& y)
		{
			return !(x < y);
		}
	};
	template<class Cont>
	struct DenseMapIterator : public DenseMapConstIterator<Cont>
	{
		using MyType = DenseMapIterator<Cont>;
		using MyBase = DenseMapConstIterator<Cont>;
		using iterator_category = typename MyBase::iterator_category;
		using key_type = typename MyBase::key_type;
		using mapped_type = typename MyBase::mapped_type;
		using difference_type = typename MyBase::difference_type;
		using key_reference = typename Cont::key_const_reference;
		using value_reference = typename Cont::value_reference;
		using key_pointer = typename Cont::key_pointer;
		using value_pointer = typename Cont::value_pointer;
		using reference = typename Cont::reference;
		using pointer = typename Cont::pointer;
		using value_type = typename Cont::value_type;
		using Kpointer = typename MyBase::Kpointer;
		using Vpointer = typename MyBase::Vpointer;
	private:
		inline static
			MyBase& CvtRef(MyType& _Xa)
		{
			return static_cast<MyBase&>(_Xa);
		}
		inline static
			const MyBase& CvtRef(const MyType& _Xa)
		{
			return static_cast<const MyBase&>(_Xa);
		}
	public:
		DenseMapIterator() noexcept = default;
		DenseMapIterator(const MyType&) noexcept = default;
		~DenseMapIterator() = default;
		DenseMapIterator(Kpointer k, Vpointer v)
			: MyBase(k, v)
		{}
		DenseMapIterator& operator=(const MyType&) noexcept = default;

		reference operator*() const
		{
			return reference(key_reference(*MyBase::key), value_reference(*MyBase::value));
		}

		MyType& operator++()
		{
			++CvtRef(*this);
			return *this;
		}
		MyType operator++(int)
		{
			MyType tmp = *this;
			++*this;
			return tmp;
		}
		MyType& operator--()
		{
			--CvtRef(*this);
			return *this;
		}
		MyType operator--(int)
		{
			MyType tmp = *this;
			--*this;
			return tmp;
		}

		MyType& operator+=(difference_type n)
		{
			CvtRef(*this) += n;
			return *this;
		}
		MyType& operator-=(difference_type n)
		{
			return *this += -n;
		}

		reference operator[](difference_type n) const
		{
			return *(*this + n);
		}

		inline
			MyType operator+(difference_type n) const
		{
			MyType tmp = *this;
			return tmp += n;
		}
		inline
			MyType operator-(difference_type n) const
		{
			MyType tmp = *this;
			return tmp -= n;
		}
		inline friend
			difference_type operator-(const MyType& x, const MyType& y)
		{
			return CvtRef(x) - CvtRef(y);
		}

		inline friend
			bool operator==(const MyType& x, const MyType& y)
		{
			return CvtRef(x) == CvtRef(y);
		}
		inline friend
			bool operator!=(const MyType& x, const MyType& y)
		{
			return !(x == y);
		}
		inline friend
			bool operator<(const MyType& x, const MyType& y)
		{
			return CvtRef(x) < CvtRef(y);
		}
		inline friend
			bool operator>(const MyType& x, const MyType& y)
		{
			return y < x;
		}
		inline friend
			bool operator<=(const MyType& x, const MyType& y)
		{
			return !(y < x);
		}
		inline friend
			bool operator>=(const MyType& x, const MyType& y)
		{
			return !(x < y);
		}
	};

	template<typename Key, typename T,
		typename Compare = std::less<>,
		typename KeyAllocator = std::allocator<Key>,
		typename ValueAllocator = std::allocator < T >>
	class DenseMap
	{
	public:
		using MyType = DenseMap<Key, T, Compare, KeyAllocator,
			ValueAllocator>;

		using KStorage = std::vector<Key, KeyAllocator>;
		using VStorage = std::vector<T, ValueAllocator>;
		using KIterator = typename KStorage::iterator;
		using VIterator = typename VStorage::iterator;

		using allocator_type = std::pair<typename KStorage::allocator_type,
										typename VStorage::allocator_type>;

		using key_type = Key;
		using mapped_type = T;

		using key_compare = Compare;

		using key_reference = typename KStorage::reference;
		using key_const_reference = typename KStorage::const_reference;
		using key_pointer = typename KStorage::pointer;
		using key_const_pointer = typename KStorage::const_pointer;

		using value_reference = typename VStorage::reference;
		using value_const_reference = typename VStorage::const_reference;
		using value_pointer = typename VStorage::pointer;
		using value_const_pointer = typename VStorage::const_pointer;

		using reference = std::pair<key_const_reference, value_reference>;
		using const_reference = std::pair<key_const_reference,
			value_const_reference>;
		using pointer = reference;
		using const_pointer = const_reference;

		using value_type = std::pair<const key_type, mapped_type>;
		using size_type = std::common_type_t<
							typename KStorage::size_type,
							typename VStorage::size_type>;
		using difference_type = std::common_type_t<
									typename KStorage::difference_type,
									typename VStorage::difference_type>;
		using iterator = DenseMapIterator<MyType>;
		using const_iterator = DenseMapConstIterator<MyType>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		class value_compare
		{
			friend class DenseMap;
		protected:
			Compare comp;
			value_compare(Compare cmp)
				: comp(cmp)
			{}
		public:
			using result_type = bool;

			template<typename U, typename V, typename W, typename X>
			bool operator()(const std::pair<U, V>& x,
				const std::pair<W, X>& y) const
			{
				return comp(x.first, y.first);
			}
		};
	private:
		KStorage keys;
		VStorage values;
		value_compare cmp;

		class equivelance_compare
		{
			friend class DenseMap;
		protected:
			Compare* comp;
			equivelance_compare(Compare& cmp)
				: comp(&cmp)
			{}
		public:
			using result_type = bool;

			bool operator()(const key_type& x, const key_type& y) const
			{
				return !(*comp)(x, y) && !(*comp)(y, x);
			}
		};

		template<typename K, typename U>
		iterator PutElement(const_iterator pos, K&& key, U&& value)
		{
			auto offset = pos - cbegin();

			keys.insert(keys.cbegin() + offset, std::forward<K>(key));
			values.insert(values.cbegin() + offset, std::forward<U>(value));

			return begin() + offset;
		}
		template<typename I1, typename I2>
		void AppendRange(I1 kfirst, I1 klast, I2 vfirst, I2 vlast,
			std::input_iterator_tag, std::input_iterator_tag)
		{
			auto keyPrevSize = keys.size();
			auto valuePrevSize = values.size();

			keys.insert(keys.cend(), kfirst, klast);
			values.insert(values.cend(), vfirst, vlast);

			if (keys.size() != values.size())
			{
				keys.erase(keys.cbegin() + keyPrevSize, keys.cend());
				values.erase(values.cbegin() + valuePrevSize, values.cend());

				throw std::length_error("insert of ranges with different lengths.");
			}
		}
		template<typename I1, typename I2>
		void AppendRange(I1 kfirst, I1 klast, I2 vfirst, I2 vlast,
			std::forward_iterator_tag, std::forward_iterator_tag)
		{
			auto dist = std::distance(kfirst, klast);
			if (dist != std::distance(vfirst, vlast))
			{
				throw std::length_error("insert of ranges with different lengths.");
			}

			ReserveSpace(dist);

			keys.insert(keys.cend(), kfirst, klast);
			values.insert(values.cend(), vfirst, vlast);
		}
		template<typename I1, typename I2, typename Tag1, typename Tag2>
		void InsertRange(I1 kfirst, I1 klast, I2 vfirst, I2 vlast,
			Tag1, Tag2)
		{
			auto prevSize = size();
			AppendRange(kfirst, klast, vfirst, vlast, Tag1{}, Tag2{});

			MakeOrdered(prevSize);
		}
		template<typename K>
		std::pair<size_type, bool> FindOffset(const K& key) const
		{
			auto pos = std::lower_bound(keys.begin(), keys.end(), key, cmp.comp);
			return std::make_pair(pos - keys.begin(),
				pos != keys.end() && !cmp.comp(key, *pos));
		}
		void ReserveSpace(size_type n)
		{
			keys.reserve(keys.size() + n);
			values.reserve(values.size() + n);
		}
		void MakeOrdered(size_type prevSize)
		{
			using mapping_type = std::pair<KIterator, VIterator>;
			auto mapped_comp =
				[=](const mapping_type& x, const mapping_type& y)
			{
				return this->cmp.comp(*x.first, *y.first);
			};

			std::vector<mapping_type> mappings;
			mappings.reserve(keys.size());
			auto kBegin = keys.begin(); auto kEnd = keys.end();
			auto vBegin = values.begin();

			for (; kBegin != kEnd; ++kBegin, (void)++vBegin)
			{
				mappings.emplace_back(kBegin, vBegin);
			}

			std::stable_sort(mappings.begin() + prevSize, mappings.end(),
				mapped_comp);
			
			std::inplace_merge(mappings.begin(), mappings.begin() + prevSize,
				mappings.end(), mapped_comp);
			
			equivelance_compare compare(cmp.comp);
			auto uniqueMappingEnd = std::unique(mappings.begin(), mappings.end(),
				[=](const mapping_type& x, const mapping_type& y)
			{
				return compare(*x.first, *y.first);
			});

			KStorage tmpKeys;
			VStorage tmpValues;
			tmpKeys.reserve(keys.capacity());
			tmpValues.reserve(keys.capacity());

			std::for_each(mappings.begin(),
				uniqueMappingEnd,
				[&tmpKeys, &tmpValues](mapping_type& x)
			{
				tmpKeys.push_back(std::move(*x.first));
				tmpValues.push_back(std::move(*x.second));
			});

			keys = std::move(tmpKeys);
			values = std::move(tmpValues);
		}
		void CleanUp()
		{
			keys.clear();
			values.clear();
		}
	public:
		DenseMap(const KeyAllocator& kAllocator = KeyAllocator(),
			const ValueAllocator& vAllocator = ValueAllocator())
			: DenseMap(Compare(), kAllocator, vAllocator)
		{}
		explicit DenseMap(const Compare& comp,
			const KeyAllocator& kAllocator = KeyAllocator(),
			const ValueAllocator& vAllocator = ValueAllocator())
			: keys(kAllocator)
			, values(vAllocator)
			, cmp(comp)
		{}
		template<typename I1, typename I2>
		DenseMap(I1 kfirst, I1 klast, I2 vfirst, I2 vlast,
			const Compare& comp = Compare(),
			const KeyAllocator& kAllocator = KeyAllocator(),
			const ValueAllocator& vAllocator = ValueAllocator())
			: keys(kfirst, klast, kAllocator)
			, values(vfirst, vlast, vAllocator)
			, cmp(comp)
		{
			if (keys.size() != values.size())
			{
				CleanUp();
				throw std::length_error("keys and values ranges are of different lengths");
			}
			MakeOrdered(0u);
		}
		template<typename I1, typename I2>
		DenseMap(I1 kfirst, I1 klast, I2 vfirst, I2 vlast,
			const KeyAllocator& kAllocator,
			const ValueAllocator& vAllocator)
			: DenseMap(kfirst, klast, vfirst, vlast, Compare(),
			kAllocator, vAllocator)
		{}
		DenseMap(const MyType& other)
			: keys(other.keys)
			, values(other.values)
			, cmp(other.cmp)
		{}
		DenseMap(const MyType& other, const KeyAllocator& kAllocator,
			const ValueAllocator& vAllocator)
			: keys(other.keys, kAllocator)
			, values(other.values, vAllocator)
			, cmp(other.cmp)
		{}
		DenseMap(MyType&& other)
			: keys(std::move(other.keys))
			, values(std::move(other.values))
			, cmp(std::move(other.cmp))
		{}
		DenseMap(MyType&& other, const KeyAllocator& kAllocator,
			const ValueAllocator& vAllocator)
			: keys(std::move(other.keys), kAllocator)
			, values(std::move(other.values), vAllocator)
			, cmp(std::move(other.cmp))
		{}
		DenseMap(std::initializer_list<value_type> init,
			const Compare& comp = Compare(),
			const KeyAllocator& kAlloc = KeyAllocator(),
			const ValueAllocator& vAlloc = ValueAllocator())
			: keys(kAlloc)
			, values(vAlloc)
			, cmp(comp)
		{
			ReserveSpace(init.size());
			std::for_each(init.begin(), init.end(),
				[=](const value_type& x)
			{
				this->keys.push_back(x.first);
				this->values.push_back(x.second);
			});
			MakeOrdered(0);
		}
		DenseMap(std::initializer_list<value_type> init,
			const KeyAllocator& kAlloc,
			const ValueAllocator& vAlloc)
			: DenseMap(init, Compare(), kAlloc, vAlloc)
		{}
		~DenseMap() = default;
		MyType& operator=(const MyType& x)
		{
			keys = x.keys;
			values = x.values;
			cmp = x.cmp;

			return *this;
		}
		MyType& operator=(MyType&& x)
		{
			keys = std::move(x.keys);
			values = std::move(x.values);
			cmp = std::move(x.cmp);

			return *this;
		}
		MyType& operator=(std::initializer_list<value_type> init)
		{
			keys.clear();
			values.clear();
			ReserveSpace(init.size());

			std::for_each(init.begin(), init.end(),
				[=](const value_type& x)
			{
				keys.push_back(x.first);
				values.push_back(x.second);
			});

			MakeOrdered(0);

			return *this;
		}
		allocator_type get_allocator() const noexcept
		{
			return std::make_pair(keys.get_allocator(), values.get_allocator());
		}

		iterator begin() noexcept
		{
			return iterator(keys.begin(), values.begin());
		}
		const_iterator begin() const noexcept
		{
			auto unsafeThis = const_cast<MyType* const>(this);
			return const_iterator(unsafeThis->keys.begin(),
				unsafeThis->values.begin());
		}
		const_iterator cbegin() const noexcept
		{
			return begin();
		}
		iterator end() noexcept
		{
			return iterator(keys.end(), values.end());
		}
		const_iterator end() const noexcept
		{
			auto unsafeThis = const_cast<MyType* const>(this);
			return const_iterator(unsafeThis->keys.end(),
				unsafeThis->values.end());
		}
		const_iterator cend() const noexcept
		{
			return end();
		}

		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return rbegin();
		}
		reverse_iterator rend() noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(begin());
		}
		const_reverse_iterator crend() const noexcept
		{
			return rend();
		}

		typename VStorage::iterator begin_values() noexcept
		{
			return values.begin();
		}
		typename VStorage::const_iterator begin_values() const noexcept
		{
			return values.begin();
		}
		typename VStorage::const_iterator cbegin_values() const noexcept
		{
			return begin_values();
		}
		typename VStorage::iterator end_values() noexcept
		{
			return values.end();
		}
		typename VStorage::const_iterator end_values() const noexcept
		{
			return values.end();
		}
		typename VStorage::const_iterator cend_values() const noexcept
		{
			return end_values();
		}

		typename VStorage::reverse_iterator rbegin_values() noexcept
		{
			return values.rbegin();
		}
		typename VStorage::const_reverse_iterator rbegin_values() const noexcept
		{
			return values.rbegin();
		}
		typename VStorage::const_reverse_iterator crbegin_values() const noexcept
		{
			return rbegin_values();
		}
		typename VStorage::reverse_iterator rend_values() noexcept
		{
			return values.rend();
		}
		typename VStorage::const_reverse_iterator rend_values() const noexcept
		{
			return values.rend();
		}
		typename VStorage::const_reverse_iterator crend_values() const noexcept
		{
			return rend_values();
		}

		bool empty() const noexcept
		{
			return keys.empty();
		}
		size_type size() const noexcept
		{
			return keys.size();
		}
		size_type max_size() const noexcept
		{
			return std::min(keys.max_size(), values.max_size());
		}
		size_type capacity() const
		{
			return keys.capacity();
		}
		void reserve(size_type n)
		{
			keys.reserve(n);
			values.reserve(n);
		}
		void shrink_to_fit()
		{
			keys.shrink_to_fit();
			values.shrink_to_fit();
		}

		key_compare key_comp() const
		{
			return cmp.comp;
		}
		value_compare value_comp() const
		{
			return cmp;
		}

		template<typename K>
		T& at(const K& key)
		{
			auto offset = FindOffset(key);
			if (!offset.second)
			{
				throw std::out_of_range("Provided key could not be found.");
			}

			return values[offset.first];
		}
		template<typename K>
		const T& at(const K& key) const
		{
			auto offset = FindOffset(key);
			if (!offset.second)
			{
				throw std::out_of_range("Provided key could not be found.");
			}

			return values[offset.first];
		}
		template<typename K>
		T& operator[](K&& key)
		{
			auto found = FindOffset(key);

			return found.second ? (values[found.first]) :
				(*insert(cbegin() + found.first, std::forward<K>(key),
				mapped_type())).second;
		}

		template <typename K>
		iterator find(const K& key)
		{
			auto found = FindOffset(key);
			return begin() + (found.second ? found.first : size());
		}
		template <typename K>
		const_iterator find(const K& key) const
		{
			auto found = FindOffset(key);
			return begin() + (found.second ? found.first : size());
		}
		template <typename K>
		iterator lower_bound(const K& key)
		{
			return find(key);
		}
		template <typename K>
		const_iterator lower_bound(const K& key) const
		{
			return find(key);
		}
		template <typename K>
		iterator upper_bound(const K& key)
		{
			auto found = find(key);
			return found == end() ? begin() : ++found;
		}
		template <typename K>
		const_iterator upper_bound(const K& key) const
		{
			auto found = find(key);
			return found == end() ? begin() : ++found;
		}
		template <typename K>
		std::pair<iterator, iterator> equal_range(const K& key)
		{
			auto found = find(key);
			if (found == end())
			{
				return std::make_pair(found, begin());
			}
			else
			{
				return std::make_pair(found, found + 1);
			}
		}
		template <typename K>
		std::pair<const_iterator, const_iterator> 
			equal_range(const K& key) const
		{
			auto found = find(key);
			if (found == end())
			{
				return std::make_pair(found, begin());
			}
			else
			{
				return std::make_pair(found, found + 1);
			}
		}

		
		template<typename M>
		void append(M&& x)
		{
			return append(std::forward<M>(x).first, std::forward<M>(x).second);
		}
		template <typename K, typename U>
		void append(K&& key, U&& value)
		{
			keys.push_back(std::forward<K>(key));
			values.push_back(std::forward<U>(value));
		}
		template<typename I1, typename I2>
		iterator append(I1 kfirst, I1 klast, I2 vfirst, I2 vlast)
		{
			AppendRange(kfirst, klast, vfirst, vlast,
				typename std::iterator_traits<I1>::iterator_category(),
				typename std::iterator_traits<I2>::iterator_category());
		}

		template<typename M>
		std::pair<iterator, bool> insert(M&& x)
		{
			return insert(std::forward<M>(x).first, std::forward<M>(x).second);
		}
		template<typename M>
		iterator insert(const_iterator pos, M&& x)
		{
			return insert(pos, std::forward<M>(x).first,
							std::forward<M>(x).second);
		}
		template <typename K, typename U, class =
			std::enable_if_t<std::is_constructible<value_type, K&&, U&&>::value>>
		std::pair<iterator, bool> insert(K&& key, U&& value)
		{
			auto found = FindOffset(key);

			return std::make_pair(found.second ? (begin() + found.first) :
				PutElement(cbegin() + found.first, std::forward<K>(key),
				std::forward<U>(value)),
				found.second);
		}
		template <typename K, typename U>
		iterator insert(const_iterator pos, K&& key, U&& value)
		{
			if (size() == 0)
			{
				return PutElement(cend(), std::forward<K>(key),
					std::forward<U>(value));
			}
			else if (pos == cend())
			{
				auto prev = pos - 1;
				if (cmp.comp((*prev).first, key))
				{
					return PutElement(pos, std::forward<K>(key),
						std::forward<U>(value));
				}
			}
			else if (pos == cbegin())
			{
				if (cmp.comp(key, (*pos).first))
				{
					return PutElement(pos, std::forward<K>(key),
						std::forward<U>(value));
				}
			}
			else
			{
				auto prev = pos - 1;

				if (cmp.comp(key, (*pos).first))
				{
					if (cmp.comp((*prev).first, key))
					{
						return PutElement(pos, std::forward<K>(key),
							std::forward<U>(value));
					}
				}
			}

			return insert(std::forward<K>(key), std::forward<U>(value)).first;
		}
		template<typename I1, typename I2>
		void insert(I1 kfirst, I1 klast, I2 vfirst, I2 vlast)
		{
			InsertRange(kfirst, klast, vfirst, vlast,
				typename std::iterator_traits<I1>::iterator_category(),
				typename std::iterator_traits<I2>::iterator_category());
		}
		void insert(std::initializer_list<value_type> elems)
		{
			if (elems.size() == 0) return;

			ReserveSpace(elems.size());
			auto prevSize = keys.size();

			std::for_each(elems.begin(), elems.end(),
				[=](const value_type& x)
			{
				keys.push_back(x.first);
				values.push_back(x.second);
			});

			MakeOrdered(prevSize);
		}

		template<typename M>
		std::pair<iterator, bool> insert_or_assign(M&& x)
		{
			return insert_or_assign(std::forward<M>(x).first, 
									std::forward<M>(x).second);
		}
		template<typename M>
		iterator insert_or_assign(const_iterator pos, M&& x)
		{
			return insert_or_assign(pos, std::forward<M>(x).first,
									std::forward<M>(x).second);
		}
		template<typename K, typename U, class =
			std::enable_if_t<std::is_constructible<value_type, K&&, U&&>::value>>
		std::pair<iterator, bool> insert_or_assign(K&& key, U&& value)
		{
			auto found = FindOffset(key);

			if (found.second)
			{
				values[found.first] = std::forward<U>(value);
				return std::make_pair(begin() + found.first, found.second);
			}
			else
			{
				return std::make_pair(PutElement(cbegin() + found.first, 
										std::forward<K>(key),
										std::forward<U>(value)),
									found.second);
			}
		}
		template<typename K, typename U>
		iterator insert_or_assign(const_iterator pos, K&& key, U&& value)
		{
			if (size() == 0)
			{
				return PutElement(cend(), std::forward<K>(key),
					std::forward<U>(value));
			}
			else if (pos == cend())
			{
				auto prev = begin() + ((pos - 1) - cbegin());
				if (cmp.comp((*prev).first, key))
				{
					return PutElement(pos, std::forward<K>(key),
						std::forward<U>(value));
				}
				else if (!cmp.comp(key, (*prev).first))
				{
					(*prev).second = std::forward<U>(value);
					return prev;
				}
			}
			else if (pos == cbegin())
			{
				if (cmp.comp(key, (*pos).first))
				{
					return PutElement(pos, std::forward<K>(key),
						std::forward<U>(value));
				}
				else if (!cmp.comp((*pos).first, key))
				{
					values[0] = std::forward<U>(value);
					return begin();
				}
			}
			else
			{
				auto position = begin() + (pos - cbegin());
				auto prev = position - 1;

				if (cmp.comp(key, (*position).first))
				{
					if (cmp.comp((*prev).first, key))
					{
						return PutElement(position, std::forward<K>(key),
							std::forward<U>(value));
					}
					else if (!cmp.comp(key, (*prev).first))
					{
						(*prev).second = std::forward<U>(value);
						return prev;
					}
				}
				else if (cmp.comp((*position).first, key))
				{
					(*position).second = std::forward<U>(value);
					return position;
				}
			}

			return insert_or_assign(std::forward<K>(key), std::forward<U>(value)).first;
		}

		iterator erase(const_iterator pos)
		{
			auto offset = pos - cbegin();

			keys.erase(keys.cbegin() + offset);
			values.erase(values.cbegin() + offset);

			return begin() + offset;
		}
		iterator erase(const_iterator first, const_iterator last)
		{
			auto bOffset = first - cbegin();
			auto eOffset = last - cbegin();

			keys.erase(keys.cbegin() + bOffset, keys.cbegin() + eOffset);
			values.erase(values.cbegin() + bOffset, values.cbegin() + eOffset);

			return begin() + bOffset;
		}

		//n is size() prior to appending elements
		void make_ordered(size_type n = 0u)
		{
			MakeOrdered(n);
		}
		void clear()
		{
			CleanUp();
		}
		void swap(MyType& other)
		{
			using std::swap;
			swap(keys, other.keys);
			swap(values, other.values);
			swap(cmp, other.cmp);
		}

		friend
			void swap(const MyType& x, const MyType& y)
		{
			x.swap(y);
		}

		friend
			bool operator==(const MyType& x, const MyType& y)
		{
			return x.size() == y.size() &&
				std::equal(x.cbegin(), x.cend(), y.cbegin());
		}
		friend
			bool operator!=(const MyType& x, const MyType& y)
		{
			return !(x == y);
		}
		friend
			bool operator<(const MyType& x, const MyType& y)
		{
			return std::lexicographical_compare(x.cbegin(), x.cend(),
				y.cbegin(), y.cend());
		}
		friend
			bool operator>(const MyType& x, const MyType& y)
		{
			return y < x;
		}
		friend
			bool operator<=(const MyType& x, const MyType& y)
		{
			return !(y < x);
		}
		friend
			bool operator>=(const MyType& x, const MyType& y)
		{
			return !(x < y);
		}
	};
}

#endif //DENSE_MAP_HPP