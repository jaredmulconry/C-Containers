#if !defined(DENSE_MAP_HPP)
#define DENSE_MAP_HPP
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

namespace custom_containers
{
	template<typename Cont>
	struct DenseMapConstIterator
	{
		using iterator_category = std::random_access_iterator_tag;
		using MyType = DenseMapConstIterator<Cont>;
		using key_type = typename Cont::key_type;
		using value_type = typename Cont::value_type;
		using difference_type = typename Cont::difference_type;
		using key_reference = typename Cont::key_const_reference;
		using value_reference = typename Cont::value_const_reference;
		using key_pointer = typename Cont::key_const_pointer;
		using value_pointer = typename Cont::value_const_pointer;
		using reference = std::pair<const key_reference, value_reference>;
		using pointer = std::pair<const key_pointer, value_pointer>;
		using Kpointer = typename Cont::key_pointer;
		using Vpointer = typename Cont::value_pointer;
	private:
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
			return reference((key_reference)*key, (value_reference)*value);
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
		using iterator_category = std::random_access_iterator_tag;
		using MyType = DenseMapIterator<Cont>;
		using MyBase = DenseMapConstIterator<Cont>;
		using key_type = typename MyBase::key_type;
		using value_type = typename MyBase::value_type;
		using difference_type = typename MyBase::difference_type;
		using key_reference = typename Cont::key_reference;
		using value_reference = typename Cont::value_reference;
		using key_pointer = typename Cont::key_pointer;
		using value_pointer = typename Cont::value_pointer;
		using reference = std::pair<const key_reference, value_reference>;
		using pointer = std::pair<const key_pointer, value_pointer>;
		using Kpointer = typename MyBase::key_pointer;
		using Vpointer = typename MyBase::value_pointer;
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
			return reference((key_reference)*key, (value_reference)*value);
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

	};
}

#endif //DENSE_MAP_HPP