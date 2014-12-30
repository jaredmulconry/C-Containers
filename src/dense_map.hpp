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
	template<typename Cont>
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
		using reference = std::pair<key_reference, value_reference>;
		using pointer = std::pair<key_reference, value_reference>;
		using value_type = reference;
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
			,value(v)
		{}
		template<typename I, class =
			typename ::std::enable_if_t<::std::is_base_of<MyType, I>::value>>
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
	template<typename Cont>
	struct DenseMapIterator : public DenseMapConstIterator<Cont>
	{
		friend typename Cont;
		using iterator_category = std::random_access_iterator_tag;
		using MyType = DenseMapIterator<Cont>;
		using MyBase = DenseMapConstIterator<Cont>;
		using key_type = typename MyBase::key_type;
		using mapped_type = typename MyBase::mapped_type;
		using difference_type = typename MyBase::difference_type;
		using key_reference = typename Cont::key_const_reference;
		using value_reference = typename Cont::value_reference;
		using key_pointer = typename Cont::key_pointer;
		using value_pointer = typename Cont::value_pointer;
		using reference = std::pair<key_reference, value_reference>;
		using value_type = reference;
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
			return reference(key_reference(*key), value_reference(*value));
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
	template<typename Cont>
	struct DenseMapInternalIterator : public DenseMapConstIterator<Cont>
	{
		using iterator_category = std::random_access_iterator_tag;
		using MyType = DenseMapInternalIterator<Cont>;
		using MyBase = DenseMapConstIterator<Cont>;
		using key_type = typename MyBase::key_type;
		using mapped_type = typename MyBase::mapped_type;
		using difference_type = typename MyBase::difference_type;
		using key_reference = typename Cont::key_reference;
		using value_reference = typename Cont::value_reference;
		using key_pointer = typename Cont::key_pointer;
		using value_pointer = typename Cont::value_pointer;
		using reference = std::pair<key_reference, value_reference>;
		using value_type = reference;
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
		DenseMapInternalIterator() noexcept = default;
		DenseMapInternalIterator(const MyType&) noexcept = default;
		~DenseMapInternalIterator() = default;
		DenseMapInternalIterator(Kpointer k, Vpointer v)
			: MyBase(k, v)
		{}
		DenseMapInternalIterator& operator=(const MyType&) noexcept = default;

		reference operator*() const
		{
			return reference(key_reference(*key), value_reference(*value));
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
		typename ValueAllocator = std::allocator<T>>
	class DenseMap
	{
	public:
		using MyType = DenseMap<Key, T, Compare, KeyAllocator,
										ValueAllocator>;
		using KAllocTraits = std::allocator_traits<KeyAllocator>;
		using KeyAlloc = typename KAllocTraits::template rebind_alloc<Key>;
		using KeyAllocTraits = std::allocator_traits<KeyAlloc>;
		using VAllocTraits = std::allocator_traits<ValueAllocator>;
		using ValueAlloc = typename KAllocTraits::template rebind_alloc<T>;
		using ValueAllocTraits = std::allocator_traits<ValueAlloc>;
		using allocator_type = std::pair<KeyAlloc, ValueAlloc>;

		using key_type = Key;
		using mapped_type = T;

		using key_reference = std::reference_wrapper<key_type>;
		using key_const_reference = std::reference_wrapper<const key_type>;
		using key_pointer = typename KeyAllocTraits::pointer;
		using key_const_pointer = typename KeyAllocTraits::const_pointer;

		using value_reference = std::reference_wrapper<mapped_type>;
		using value_const_reference = std::reference_wrapper<const mapped_type>;
		using value_pointer = typename ValueAllocTraits::pointer;
		using value_const_pointer = typename ValueAllocTraits::const_pointer;

		using reference = std::pair<key_const_reference, value_reference>;
		using const_reference = std::pair<key_const_reference, 
														value_const_reference>;
		using InternalReference = std::pair<key_reference, value_reference>;

		using value_type = reference;
		using size_type = typename ValueAllocTraits::size_type;
		using difference_type = typename ValueAllocTraits::difference_type;
		using iterator = DenseMapIterator<MyType>;
		using const_iterator = DenseMapConstIterator<MyType>;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		using InternalIterator = DenseMapInternalIterator<MyType>;

		using KStorage = std::vector<Key, KeyAlloc>;
		using VStorage = std::vector<T, ValueAlloc>;
		using KIterator = typename KStorage::iterator;
		using VIterator = typename VStorage::iterator;

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
			using first_argument_type = reference;
			using second_argument_type = reference;
			bool operator()(const first_argument_type& x,
				const second_argument_type& y) const
			{
				return comp(x.first, y.first);
			}
		};
	private:
		class sorting_compare
		{
			friend class DenseMap;
		protected:
			value_compare* comp;
			explicit sorting_compare(value_compare& cmp)
				:comp(&cmp)
			{}
		public:
			using result_type = bool;
			using first_argument_type = InternalReference;
			using second_argument_type = InternalReference;
			bool operator()(const first_argument_type& x,
				const second_argument_type& y) const
			{
				return (*comp)(reference(std::cref(x.first), x.second), 
								reference(std::cref(y.first), y.second));
			}
		};

		KStorage keys;
		VStorage values;
		value_compare cmp;

		InternalIterator Ibegin() noexcept
		{
			return InternalIterator(keys.begin(), values.begin());
		}
		InternalIterator Iend() noexcept
		{
			return InternalIterator(keys.end(), values.end());
		}
		InternalIterator ICap() noexcept
		{
			auto cap = keys.capacity();
			return InternalIterator(keys.begin() + cap, values.begin() + cap);
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
			if(keys.size() != values.size())
			{
				CleanUp();
				throw std::length_error("keys and values ranges are of different lengths");
			}

			std::sort(Ibegin(), Iend(), sorting_compare(cmp));
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
		~DenseMap() = default;
		MyType& operator=(const MyType& x)
		{
			keys = x.keys;
			values = x.values;
			cmp = x.cmp;
		}
		MyType& operator=(MyType&& x)
		{
			keys = std::move(x.keys);
			values = std::move(x.values);
			cmp = std::move(x.cmp);
		}
		allocator_type get_allocator() const noexcept
		{
			return std::make_pair(keys.get_allocator(), values.get_allocator());
		}
	};
}

#endif //DENSE_MAP_HPP