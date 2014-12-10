#ifndef _Vector_hpp
#define _Vector_hpp
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace custom_std
{
	template<typename _MyVec>
	class _Vector_const_iterator
	{
	public:
		typedef _Vector_const_iterator<_MyVec>			_MyTa;
		typedef ::std::random_access_iterator_tag		iterator_category;
		typedef typename _MyVec::value_type				value_type;
		typedef typename _MyVec::const_reference		reference;
		typedef typename _MyVec::const_pointer			pointer;
		typedef typename _MyVec::difference_type		difference_type;
		typedef typename _MyVec::pointer				_Tptr;
	private:
		_Tptr _Pos;
	public:
		_Vector_const_iterator() noexcept = default;
		_Vector_const_iterator(const _Vector_const_iterator&) noexcept = default;
		~_Vector_const_iterator() noexcept = default;
		template<typename _Iter, class =
			typename ::std::enable_if<::std::is_base_of<_MyTa,
			_Iter>::value>::type>
			explicit _Vector_const_iterator(const _Iter& _Iderived) noexcept
			:_Pos(static_cast<const _MyTa&>(_Iderived)._Pos)
		{}
		explicit _Vector_const_iterator(_Tptr _Xp) noexcept
			: _Pos(_Xp)
		{}
		_Vector_const_iterator& operator=(const _Vector_const_iterator&)noexcept = default;

		reference operator*() const
		{
			return *this->_Pos;
		}
		pointer operator->() const
		{
			return ::std::pointer_traits<pointer>::pointer_to(**this);
		}

		_MyTa& operator++()
		{
			++this->_Pos;
			return *this;
		}
		_MyTa operator++(int)
		{
			_MyTa _Tmp = *this;
			++*this;
			return _Tmp;
		}
		_MyTa& operator--()
		{
			--this->_Pos;
			return *this;
		}
		_MyTa operator--(int)
		{
			_MyTa _Tmp = *this;
			--*this;
			return _Tmp;
		}

		_MyTa& operator+=(difference_type _Xn)
		{
			difference_type _Xm = _Xn;
			if (_Xm >= 0)
				while (_Xm--)
					++*this;
			else
				while (_Xm++)
					--*this;
			return *this;
		}
		_MyTa& operator-=(difference_type _Xn)
		{
			return *this += -_Xn;
		}

		reference operator[](difference_type _Xn)
		{
			return *(*this + _Xn);
		}

		inline
			_MyTa operator+(difference_type _Xn) const
		{
			_MyTa _Tmp = *this;
			return _Tmp += _Xn;
		}
		inline
			_MyTa operator-(difference_type _Xn) const
		{
			_MyTa _Tmp = *this;
			return _Tmp -= _Xn;
		}
		inline friend
			difference_type operator-(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xa._Pos - _Xb._Pos;
		}

		inline friend
			bool operator==(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xa._Pos == _Xb._Pos;
		}
		inline friend
			bool operator!=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xa == _Xb);
		}
		inline friend
			bool operator<(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xa._Pos < _Xb._Pos;
		}
		inline friend
			bool operator>(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xb < _Xa;
		}
		inline friend
			bool operator<=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xb < _Xa);
		}
		inline friend
			bool operator>=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xa < _Xb);
		}
	};


	template<typename _MyVec>
	class _Vector_iterator : public _Vector_const_iterator < _MyVec >
	{
	public:
		typedef _Vector_iterator<_MyVec>				_MyTa;
		typedef _Vector_const_iterator<_MyVec>			_MyBase;
		typedef typename _MyVec::value_type				value_type;
		typedef typename _MyVec::reference				reference;
		typedef typename _MyVec::pointer				pointer;
		typedef typename _MyVec::difference_type		difference_type;
		typedef typename _MyBase::iterator_category		iterator_category;
		typedef typename _MyBase::_Tptr					_Tptr;
	private:
		inline static
			_MyBase& _CvtRef(_MyTa& _Xa)
		{
			return static_cast<_MyBase&>(_Xa);
		}
		inline static
			const _MyBase& _CvtRef(const _MyTa& _Xa)
		{
			return static_cast<const _MyBase&>(_Xa);
		}
	public:

		_Vector_iterator() noexcept = default;
		_Vector_iterator(const _Vector_iterator&) noexcept = default;
		~_Vector_iterator() noexcept = default;
		explicit _Vector_iterator(_Tptr _Xp) noexcept
			: _Vector_const_iterator<_MyVec>(_Xp)
		{}
		_Vector_iterator& operator=(const _Vector_iterator&)noexcept = default;

		reference operator*() const
		{
			return (reference)(**((_MyBase* const)this));
		}
		pointer operator->() const
		{
			return ::std::pointer_traits<pointer>::pointer_to(**this);
		}

		_MyTa& operator++()
		{
			++_MyTa::_CvtRef(*this);
			return *this;
		}
		_MyTa operator++(int)
		{
			_MyTa _Tmp = *this;
			++_MyTa::_CvtRef(*this);
			return _Tmp;
		}
		_MyTa& operator--()
		{
			--_MyTa::_CvtRef(*this);;
			return *this;
		}
		_MyTa operator--(int)
		{
			_MyTa _Tmp = *this;
			--_MyTa::_CvtRef(*this);;
			return _Tmp;
		}
		_MyTa& operator+=(difference_type _Xn)
		{
			_MyTa::_CvtRef(*this) += _Xn;
			return *this;
		}
		_MyTa& operator-=(difference_type _Xn)
		{
			return *this += -_Xn;
		}

		reference operator[](difference_type _Xn)
		{
			return *(*this + _Xn);
		}

		inline
			_MyTa operator+(difference_type _Xn) const
		{
			_MyTa _Tmp = *this;
			return _Tmp += _Xn;
		}
		inline
			_MyTa operator-(difference_type _Xn) const
		{
			_MyTa _Tmp = *this;
			return _Tmp -= _Xn;
		}
		inline friend
			difference_type operator-(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _MyTa::_CvtRef(_Xa) - _MyTa::_CvtRef(_Xb);
		}

		inline friend
			bool operator==(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _MyTa::_CvtRef(_Xa) == _MyTa::_CvtRef(_Xb);
		}
		inline friend
			bool operator!=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xa == _Xb);
		}
		inline friend
			bool operator<(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _MyTa::_CvtRef(_Xa) < _MyTa::_CvtRef(_Xb);
		}
		inline friend
			bool operator>(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xb < _Xa;
		}
		inline friend
			bool operator<=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xb < _Xa);
		}
		inline friend
			bool operator>=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xa < _Xb);
		}
	};

	template<typename _Ta, typename _Alloc = std::allocator<_Ta>>
	class vector
	{
	public:
		typedef vector<_Ta, _Alloc>	_MyTa;
		typedef ::std::allocator_traits<_Alloc>	_AllocTraits;
		typedef typename _AllocTraits::template rebind_alloc<_Ta>	_AllocTa;
		typedef ::std::allocator_traits<_AllocTa> _TaAllocTraits;
		typedef _Alloc	allocator_type;
		typedef _Ta	value_type;
		typedef value_type&	reference;
		typedef const value_type&	const_reference;
		typedef typename _TaAllocTraits::pointer	pointer;
		typedef typename _TaAllocTraits::const_pointer	const_pointer;
		typedef typename _TaAllocTraits::size_type	size_type;
		typedef typename _TaAllocTraits::difference_type	difference_type;
		typedef _Vector_iterator<_MyTa>	iterator;
		typedef _Vector_const_iterator<_MyTa>	const_iterator;
		typedef ::std::reverse_iterator<iterator>	reverse_iterator;
		typedef ::std::reverse_iterator<const_iterator>	const_reverse_iterator;
	private:
		pointer		_First;
		pointer		_Last;
		pointer		_End;
		_AllocTa	_Al;

		inline void _DestroyObj(pointer _Xp)
		{
			_TaAllocTraits::destroy(this->_Al, _Xp);
		}
		template<typename... _Args>
		inline void _ConstructElem(pointer _Xp, _Args&&... _Xargs)
		{
			_TaAllocTraits::construct(this->_Al, _Xp,
				std::forward<_Args>(_Xargs)...);
		}
		inline _Ta* _Allocate(::std::size_t _Xn)
		{
			return _TaAllocTraits::allocate(this->_Al, _Xn);
		}
		inline void _Deallocate(pointer _Xp, ::std::size_t _Xn)
		{
			_TaAllocTraits::deallocate(this->_Al, _Xp, _Xn);
		}

		bool _IsValidCapacity(size_type _Xn)
		{
			if (_Xn == 0) return false;
			if (_Xn > this->max_size())
			{
				throw ::std::length_error("vector<T> exceeded max_size");
			}
			return true;
		}

		void _DestroyRange(pointer _XiFirst, pointer _XiLast)
		{
			for (; _XiFirst != _XiLast; ++_XiFirst)
				_DestroyObj(_XiFirst);
		}

		template<typename _InputIt>
		void _ConstructIterators(_InputIt _XiFirst, _InputIt _XiLast,
			::std::input_iterator_tag)
		{
			try
			{
				for (; _XiFirst != _XiLast; ++_XiFirst)
					this->emplace_back(*_XiFirst);
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		template<typename _FwdIt>
		void _ConstructIterators(_FwdIt _XiFirst, _FwdIt _XiLast,
			::std::forward_iterator_tag)
		{
			auto _Count = size_type(::std::distance(_XiFirst, _XiLast));
			if (!this->_IsValidCapacity(_Count)) return;
			if (this->capacity() < _Count)
			{
				_Count = this->_GrowCapacity(_Count);
				this->_Cleanup();
				this->_Reserve(_Count);
			}

			try
			{
				this->_Last = this->_CopyRange(_XiFirst, _XiLast, this->_Last);
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		void _ConstructRangeValue(pointer _XoFirst, pointer _XoLast,
			const _Ta& _Xval)
		{
			pointer _Lfirst = _XoFirst;
			try
			{
				for (; _XoFirst != _XoLast; ++_XoFirst)
					this->_ConstructElem(_XoFirst, _Xval);
			}
			catch (...)
			{
				this->_DestroyRange(_Lfirst, _XoFirst);
				throw;
			}
		}
		void _ConstructRangeDefault(pointer _XoFirst, pointer _XoLast)
		{
			pointer _Lfirst = _XoFirst;
			try
			{
				for (; _XoFirst != _XoLast; ++_XoFirst)
					this->_ConstructElem(_XoFirst);
			}
			catch (...)
			{
				this->_DestroyRange(_Lfirst, _XoFirst);
				throw;
			}
		}

		template<typename _FwdIt>
		pointer _CopyRange(_FwdIt _XiFirst, _FwdIt _XiLast, pointer _XoFirst)
		{
			pointer _Lfirst = _XoFirst;
			try
			{
				for (; _XiFirst != _XiLast; ++_XiFirst, (void)++_XoFirst)
					this->_ConstructElem(_XoFirst, *_XiFirst);
			}
			catch (...)
			{
				this->_DestroyRange(_Lfirst, _XoFirst);
				throw;
			}

			return _XoFirst;
		}
		template<typename _InputIt>
		pointer _AssignRange(_InputIt _XiFirst, _InputIt _XiLast, pointer _XoFirst)
		{
			for (; _XiFirst != _XiLast; ++_XiFirst, (void)++_XoFirst)
				*_XoFirst = *_XiFirst;
			return _XoFirst;
		}

		void _PropagateSwap(vector& _Xa, ::std::true_type)
		{
			using ::std::swap;

			swap(this->_Al, _Xa._Al);
			swap(this->_First, _Xa._First);
			swap(this->_Last, _Xa._Last);
			swap(this->_End, _Xa._End);
		}
		inline 
			void _PropagateSwap(vector&, ::std::false_type)
		{
			::std::terminate();
		}

		size_type _GrowCapacity(size_type _Xn)
		{
			auto _Capacity = this->capacity();
			_Capacity = this->max_size() - _Capacity / 2 < _Capacity ?
				0 : _Capacity + _Capacity / 2;
			if (_Capacity < _Xn)
			{
				_Capacity = _Xn;
			}
			return _Capacity;
		}
		void _Reserve(size_type _Xn)
		{
			if (_Xn < this->size() || this->max_size() - this->size() < _Xn)
			{
				throw ::std::length_error("vector<T> exceeded max_size");
				return;
			}
			if (_Xn == this->capacity())
			{
				return;
			}

			pointer _Buf = this->_Allocate(_Xn);
			size_type _Lsize = this->size();

			if (this->_First != pointer())
			{
				try
				{
					this->_Transfer(this->_First, this->_Last, _Buf);
				}
				catch (...)
				{
					this->_Deallocate(_Buf, _Xn);
					throw;
				}

				this->_DestroyRange(this->_First, this->_Last);
				this->_Deallocate(this->_First, this->_End - this->_First);
			}

			this->_End = _Buf + _Xn;
			this->_Last = _Buf + _Lsize;
			this->_First = _Buf;
		}

		template<typename _InputIt>
		void _Transfer(_InputIt _XiFirst, _InputIt _XiLast, pointer _XoFirst,
			::std::true_type)
		{
			for (; _XiFirst != _XiLast; ++_XiFirst, (void)++_XoFirst)
				this->_ConstructElem(_XoFirst, ::std::move(*_XiFirst));
		}
		template<typename _InputIt>
		void _Transfer(_InputIt _XiFirst, _InputIt _XiLast, pointer _XoFirst,
			::std::false_type)
		{
			pointer _Lfirst = _XoFirst;
			try
			{
				for (; _XiFirst != _XiLast; ++_XiFirst, (void)++_XoFirst)
					this->_ConstructElem(_XoFirst, *_XiFirst);
			}
			catch (...)
			{
				this->_DestroyRange(_Lfirst, _XoFirst);
				throw;
			}
		}
		template<typename _InputIt>
		void _Transfer(_InputIt _XiFirst, _InputIt _XiLast,
			pointer _XoFirst)
		{
			this->_Transfer(_XiFirst, _XiLast, _XoFirst,
				typename ::std::is_nothrow_move_constructible<_Ta>::type());
		}

		template<typename _InputIt>
		void _InsertRange(const_iterator _Xpos, _InputIt _XiFirst, 
			_InputIt _XiLast, ::std::input_iterator_tag)
		{
			if (_XiFirst == _XiLast) return;

			auto _OldSize = this->size();
			auto _Off = _Xpos - this->cbegin();

			try
			{
				for (; _XiFirst != _XiLast; ++_XiFirst)
				{
					this->emplace_back(*_XiFirst);
				}
			}
			catch (...)
			{
				this->erase(this->begin() + _OldSize, this->end());
				throw;
			}

			::std::rotate(this->begin() + _Off, this->begin() + _OldSize,
				this->end());
		}
		template<typename _InputIt>
		void _InsertRange(const_iterator _Xpos, _InputIt _XiFirst,
			_InputIt _XiLast, ::std::forward_iterator_tag)
		{
			auto _Dist = ::std::distance(_XiFirst, _XiLast);
			auto _Off = _Xpos - this->cbegin();
			if (_XiFirst == _XiLast) return;

			if (this->size() + _Dist > this->capacity())
			{
				auto _NewSize = this->size() + _Dist;
				auto _NewCapacity = this->_GrowCapacity(_NewSize);
				pointer _Buf = this->_Allocate(_NewCapacity);

				pointer _Progress = _Buf;
				try
				{
					this->_Transfer(this->begin(), this->begin() + _Off,
						_Progress);
					_Progress = _Buf + _Off;
					_Progress = this->_CopyRange(_XiFirst, _XiLast, _Progress);
					this->_Transfer(this->begin() + _Off, this->end(),
						_Progress);
				}
				catch (...)
				{
					this->_DestroyRange(_Buf, _Progress);
					this->_Deallocate(_Buf, _NewCapacity);
					throw;
				}

				this->_DestroyRange(this->_First, this->_Last);
				this->_Deallocate(this->_First, this->capacity());

				this->_End = _Buf + _NewCapacity;
				this->_Last = _Buf + _NewSize;
				this->_First = _Buf;
			}
			else
			{
				this->_CopyRange(_XiFirst, _XiLast, this->_Last);
				::std::rotate(this->begin() + _Off, this->end(), 
					this->end() + _Dist);
				this->_Last += _Dist;
			}
		}

		void _Cleanup()
		{
			if (this->_First == pointer()) return;
			this->_DestroyRange(this->_First, this->_Last);
			this->_Deallocate(this->_First, this->_End - this->_First);
			this->_End = pointer();
			this->_Last = pointer();
			this->_First = pointer();
		}
	public:
		explicit vector(const _Alloc& _Xalloc = _Alloc())
			: _First()
			, _Last()
			, _End()
			, _Al(_Xalloc)
		{}
		explicit vector(size_type _Xn)
			:vector()
		{
			if (!this->_IsValidCapacity(_Xn)) return;
			this->_Reserve(_Xn);
			try
			{
				this->_ConstructRangeDefault(this->_First, this->_First + _Xn);
				this->_Last += _Xn;
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		vector(size_type _Xn, const _Ta& _Xval,
			const _Alloc& _Xalloc = _Alloc())
			:vector(_Xalloc)
		{
			if (!this->_IsValidCapacity(_Xn)) return;
			this->_Reserve(_Xn);
			try
			{
				this->_ConstructRangeValue(this->_First, this->_First + _Xn,
					_Xval);
				this->_Last += _Xn;
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		template<typename _InputIt, class =
			typename ::std::enable_if < !::std::is_integral<_InputIt>::value 
										>::type>
			vector(_InputIt _XiFirst, _InputIt _XiLast,
			const _Alloc& _Xalloc = _Alloc())
			: vector(_Xalloc)
		{
			this->_ConstructIterators(_XiFirst, _XiLast,
				typename ::std::iterator_traits<_InputIt>::iterator_category());
		}
		vector(const vector& _Xa)
			:vector(_TaAllocTraits::select_on_container_copy_construction(_Xa._Al))
		{
			auto _Xsize = _Xa.size();
			if (!this->_IsValidCapacity(_Xsize)) return;
			this->_Reserve(_Xsize);

			try
			{
				this->_Last = this->_CopyRange(_Xa.begin(), _Xa.end(), this->_Last);
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		vector(vector&& _Xa)
			: vector(::std::move(_Xa._Al))
		{
			_First = ::std::move(_Xa._First);
			_Last = ::std::move(_Xa._Last);
			_End = ::std::move(_Xa._End);
			_Xa._First = _Xa._Last = _Xa._End = pointer();
		}
		vector(const vector& _Xa, const _Alloc& _Xalloc)
			: vector(_Xalloc)
		{
			auto _Xsize = _Xa.size();
			if (!this->_IsValidCapacity(_Xsize)) return;
			this->_Reserve(_Xsize);

			try
			{
				this->_Last = this->_CopyRange(_Xa.begin(), _Xa.end(), this->_Last);
			}
			catch (...)
			{
				this->_Cleanup();
				throw;
			}
		}
		vector(vector&& _Xa, const _Alloc& _Xalloc)
			: vector(_Xalloc)
		{
			this->_First = ::std::move(_Xa._First);
			this->_Last = ::std::move(_Xa._Last);
			this->_End = ::std::move(_Xa._End);

			_Xa._First = _Xa._Last = _Xa._End = pointer();
		}
		vector(::std::initializer_list<_Ta> _Xelems,
			const _Alloc& _Xalloc = _Alloc())
			: vector(_Xalloc)
		{
			this->insert(this->begin(), _Xelems.begin(), _Xelems.end());
		}
		~vector()
		{
			this->_Cleanup();
		}
		vector& operator=(const vector& _Xa)
		{
			if (this == &_Xa) return *this;

			if (this->_Al != _Xa._Al &&
				_TaAllocTraits::propagate_on_container_copy_assignment::value)
			{
				this->_Cleanup();
				this->_Al = _Xa._Al;
			}

			if (_Xa.empty())
			{
				this->clear();
			}
			else if (_Xa.size() <= this->size())
			{
				auto _NewLast = this->_AssignRange(_Xa._First, _Xa._Last, this->_First);
				this->_DestroyRange(_NewLast, this->_Last);
				this->_Last = this->_First + _Xa.size();
			}
			else if (_Xa.size() <= this->capacity())
			{
				auto _CopyLast = _Xa._First + this->size();
				this->_AssignRange(_Xa._First, _CopyLast, this->_First);
				this->_Last = this->_CopyRange(_CopyLast, _Xa._Last, this->_Last);
			}
			else
			{
				if (this->_First != pointer())
				{
					this->_Cleanup();
				}

				if (this->_IsValidCapacity(_Xa.size()))
				{
					this->_Reserve(_Xa.size());
					try
					{
						this->_CopyRange(_Xa._First, _Xa._Last, this->_First);
					}
					catch (...)
					{
						this->_Cleanup();
						throw;
					}
				}
			}

			return *this;
		}
		vector& operator=(vector&& _Xa)
		{
			if (this == &_Xa) return *this;
			this->_Cleanup();

			if (this->_Al != _Xa._Al &&
				_TaAllocTraits::propagate_on_container_move_assignment::value)
			{
				this->_Al = _Xa._Al;
			}

			if (this->_Al == _Xa._Al)
			{
				this->_First = ::std::move(_Xa._First);
				this->_Last = ::std::move(_Xa._Last);
				this->_End = ::std::move(_Xa._End);

				_Xa._First = nullptr;
				_Xa._Last = nullptr;
				_Xa._End = nullptr;
			}
			else
			{
				this->_Reserve(_Xa.size());
				this->_Transfer(_Xa._First, _Xa._Last, this->_First);
				this->_Last = this->_First + this->size();
			}

			return *this;
		}
		vector& operator=(::std::initializer_list<_Ta> _Xelems)
		{
			this->assign(_Xelems.begin(), _Xelems.end());
			return *this;
		}
		template<typename _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value 
									>::type>
			void assign(_InputIter _XiFirst, _InputIter _XiLast)
		{
			this->clear();
			this->insert(this->begin(), _XiFirst, _XiLast);
		}
		void assign(size_type _Xn, const _Ta& _Xval)
		{
			this->clear();
			insert(this->begin(), _Xn, _Xval);
		}
		void assign(::std::initializer_list<_Ta> _Xelems)
		{
			assign(_Xelems.begin(), _Xelems.end());
		}
		allocator_type get_allocator() const noexcept
		{
			return _Al;
		}

		iterator begin() noexcept
		{
			return iterator(this->_First);
		}
		const_iterator begin() const noexcept
		{
			return const_iterator(this->_First);
		}
		iterator end() noexcept
		{
			return iterator(this->_Last);
		}
		const_iterator end() const noexcept
		{
			return const_iterator(this->_Last);
		}
		reverse_iterator rbegin() noexcept
		{
			return reverse_iterator(this->end());
		}
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(this->end());
		}
		reverse_iterator rend() noexcept
		{
			return reverse_iterator(this->begin());
		}
		const_reverse_iterator rend() const noexcept
		{
			return const_reverse_iterator(this->begin());
		}
		const_iterator cbegin() const noexcept
		{
			return const_iterator(this->_First);
		}
		const_iterator cend() const noexcept
		{
			return const_iterator(this->_Last);
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(this->cend());
		}
		const_reverse_iterator crend() const noexcept
		{
			return const_reverse_iterator(this->cbegin());
		}

		size_type size() const noexcept
		{
			return this->_Last - this->_First;
		}
		size_type max_size() const noexcept
		{
			return _TaAllocTraits::max_size(this->_Al);
		}
		void resize(size_type _Xsize)
		{
			if (_Xsize < this->size())
			{
				pointer _NewLast = this->_Last - _Xsize;
				this->_DestroyRange(_NewLast, this->_Last);
				this->_Last = _NewLast;
			}
			else if (this->size() < _Xsize)
			{
				auto _SizeDiff = _Xsize - this->size();
				this->_Reserve(this->_GrowCapacity(this->size() + _Xsize));
				try
				{
					this->_ConstructRangeDefault(this->_Last, this->_Last + _SizeDiff);
				}
				catch (...)
				{
					this->_Cleanup();
					throw;
				}
				this->_Last += _SizeDiff;
			}
		}
		void resize(size_type _Xsize, const _Ta& _Xval)
		{
			if (_Xsize > this->size())
				this->insert(this->end(), _Xsize - this->size(), _Xval);
			else if (_Xsize < this->size())
				this->erase(this->begin() + _Xsize, this->end());
			else
			{
			}

			/*if (_Xsize < this->size())
			{
				pointer _NewLast = this->_Last - _Xsize;
				this->_DestroyRange(_NewLast, this->_Last);
				this->_Last = _NewLast;
			}
			else if (this->size() < _Xsize)
			{
				auto _SizeDiff = _Xsize - this->size();
				this->_Reserve(this->_GrowCapacity(this->size() + _Xsize));
				try
				{
					this->_ConstructRangeValue(this->_Last, this->_Last + _SizeDiff, _Xval);
				}
				catch (...)
				{
					this->_Cleanup();
					throw;
				}
				this->_Last += _SizeDiff;
			}*/
		}
		size_type capacity() const noexcept
		{
			return this->_End - this->_First;
		}
		bool empty() const noexcept
		{
			return this->_First == this->_Last;
		}
		void reserve(size_type _Xn)
		{
			if (_Xn < this->capacity()) return;
			if (_Xn > this->max_size())
			{
				throw ::std::length_error("_Xn > max_size");
			}
			this->_Reserve(this->_GrowCapacity(_Xn));
		}
		void shrink_to_fit()
		{
			this->_Reserve(this->size());
		}

		reference operator[](size_type _Xn)
		{
			return *(this->begin() + _Xn);
		}
		const_reference operator[](size_type _Xn) const
		{
			return *(this->begin() + _Xn);
		}
		const_reference at(size_type _Xn) const
		{
			if (_Xn >= this->size())
				throw ::std::out_of_range("_Xn >= size()");
			return (*this)[_Xn];
		}
		reference at(size_type _Xn)
		{
			if (_Xn >= this->size())
				throw ::std::out_of_range("_Xn >= size()");
			return (*this)[_Xn];
		}
		reference front()
		{
			return *this->begin();
		}
		const_reference front() const
		{
			return *this->begin();
		}
		reference back()
		{
			auto _RetIter = this->end();
			--_RetIter;
			return *_RetIter;
		}
		const_reference back() const
		{
			auto _RetIter = this->end();
			--_RetIter;
			return *_RetIter;
		}

		_Ta* data() noexcept
		{
			return this->_First;
		}
		const _Ta* data() const noexcept
		{
			return this->_First;
		}

		template <class... _Args> void emplace_back(_Args&&... _Xargs)
		{
			if (this->_Last == this->_End)
			{
				auto _NewSize = this->size() + 1;
				auto _PrevSize = this->size();
				auto _NewCapacity = this->_GrowCapacity(_NewSize);
				pointer _Buf = this->_Allocate(_NewCapacity);

				bool _TransferSuccess = false;
				try
				{
					this->_Transfer(this->begin(), this->end(), _Buf);
					_TransferSuccess = true;
					this->_ConstructElem(_Buf + _PrevSize,
						::std::forward<_Args>(_Xargs)...);
				}
				catch (...)
				{
					if (_TransferSuccess)
					{
						this->_DestroyRange(_Buf, _Buf + _PrevSize);
					}
					this->_Deallocate(_Buf, _NewCapacity);
					throw;
				}

				this->_DestroyRange(this->_First, this->_Last);
				this->_Deallocate(this->_First, this->capacity());

				this->_End = _Buf + _NewCapacity;
				this->_Last = _Buf + _NewSize;
				this->_First = _Buf;
			}
			else
			{
				this->_ConstructElem(this->_Last,
					::std::forward<_Args>(_Xargs)...);
				++this->_Last;
			}
		}
		void push_back(const _Ta& _Xa)
		{
			this->emplace_back(_Xa);
		}
		void push_back(_Ta&& _Xa)
		{
			this->emplace_back(::std::move(_Xa));
		}
		void pop_back()
		{
			this->_DestroyObj(this->_Last - 1);
			--this->_Last;
		}

		template <class... _Args> iterator emplace(const_iterator _Xpos,
			_Args&&... _XArgs)
		{
			auto _Off = _Xpos - this->cbegin();
			this->emplace_back(::std::forward<_Args>(_XArgs)...);
			::std::rotate(this->begin() + _Off, this->end() - 1, this->end());
			return this->begin() + _Off;
		}
		iterator insert(const_iterator _Xpos, const _Ta& _Xa)
		{
			return this->emplace(_Xpos, _Xa);
		}
		iterator insert(const_iterator _Xpos, _Ta&& _Xa)
		{
			return this->emplace(_Xpos, ::std::move(_Xa));
		}
		iterator insert(const_iterator _Xpos, size_type _Xn, const _Ta& _Xval)
		{
			auto _Off = _Xpos - this->begin();

			if (_Xn == 0)
			{}
			else if ((size_type)(this->_End - this->_Last) < _Xn)
			{
				auto _Tmp = _Xval;

				auto _NewSize = this->size() + _Xn;
				auto _NewCapacity = this->_GrowCapacity(_NewSize);
				pointer _Buf = this->_Allocate(_NewCapacity);

				pointer _Progress = _Buf;
				try
				{
					this->_Transfer(this->begin(), this->begin() + _Off, 
						_Progress);
					_Progress = _Buf + _Off;
					if (_Off + _Xn > this->size())
					{
						auto _TransferCount = (_Off + _Xn) - this->size();
						for (auto _Ctr = _TransferCount; _Ctr != 0; --_Ctr,
							++_Progress)
						{
							*_Progress = _Tmp;
						}
						_Xn -= _TransferCount;
					}
					
					this->_ConstructRangeValue(_Progress, _Progress + _Xn, 
						_Tmp);
					_Progress = _Progress + _Xn;
					
					this->_Transfer(this->begin() + _Off, this->end(), 
						_Progress);
				}
				catch(...)
				{
					this->_DestroyRange(_Buf, _Progress);
					this->_Deallocate(_Buf, _NewCapacity);
					throw;
				}

				this->_DestroyRange(this->_First, this->_Last);
				this->_Deallocate(this->_First, this->capacity());

				this->_End = _Buf + _NewCapacity;
				this->_Last = _Buf + _NewSize;
				this->_First = _Buf;
			}
			else
			{
				this->_ConstructRangeValue(this->_Last, this->_Last + _Xn, 
					_Xval);
				::std::rotate(this->begin() + _Off, this->end(), 
					this->end() + _Xn);
				this->_Last = this->_Last + _Xn;
			}

			return this->begin() + _Off;
		}
		template<typename _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value 
										>::type>
		iterator insert(const_iterator _Xpos, _InputIter _XiFirst, 
			_InputIter _XiLast)
		{
			auto _Off = _Xpos - this->cbegin();

			this->_InsertRange(_Xpos, _XiFirst, _XiLast,
				typename ::std::iterator_traits<_InputIter>::iterator_category());

			return this->begin() + _Off;
		}
		iterator insert(const_iterator _Xpos,
			::std::initializer_list<_Ta> _Xelems)
		{
			return this->insert(_Xpos, _Xelems.begin(), _Xelems.end());
		}
		iterator erase(const_iterator _Xpos)
		{
			auto _Off = _Xpos - this->cbegin();
			::std::move(this->begin() + _Off + 1, this->end(), 
				this->begin() + _Off);
			this->_DestroyObj(this->_Last - 1);
			--this->_Last;
			return this->begin() + _Off;
		}
		iterator erase(const_iterator _XiFirst, const_iterator _XiLast)
		{
			auto _Off = _XiFirst - this->cbegin();
			if (_XiFirst == this->cbegin() && _XiLast == this->cend())
			{
				this->clear();
			}
			else if (_XiFirst != _XiLast)
			{
				auto _Xfirst = this->begin() + (_XiFirst - this->cbegin());
				auto _DistToLast = _XiLast - this->cbegin();
				auto _Xlast = this->begin() + _DistToLast;

				auto _Pos = ::std::move(_Xlast, this->end(), _Xfirst);
				auto _PosPtr = this->_First + (_Pos - this->begin());
				this->_DestroyRange(_PosPtr, this->_Last);
				this->_Last = _PosPtr;
			}
			
			return this->begin() + _Off;
		}
		void swap(vector& _Xa)
		{
			using ::std::swap;

			if (this == &_Xa) return;
			if (this->_Al == _Xa._Al)
			{
				swap(this->_First, _Xa._First);
				swap(this->_Last, _Xa._Last);
				swap(this->_End, _Xa._End);
				return;
			}
			this->_PropagateSwap(_Xa, 
				typename _TaAllocTraits::propagate_on_container_swap::type());
		}
		void clear() noexcept
		{
			this->_DestroyRange(this->_First, this->_Last);
			this->_Last = this->_First;
		}
	};

	template <class _Ta, class _Alloc>
	bool operator==(const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return _Xa.size() == _Xb.size() && 
			::std::equal(_Xa.begin(), _Xa.end(),
			_Xb.begin(), _Xb.end());
	}
	template <class _Ta, class _Alloc>
	bool operator!=(const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return !(_Xa == _Xb);
	}
	template <class _Ta, class _Alloc>
	bool operator< (const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return ::std::lexicographical_compare(_Xa.begin(), _Xa.end(),
					_Xb.begin(), _Xb.end());
	}
	
	template <class _Ta, class _Alloc>
	bool operator> (const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return _Xb < _Xa;
	}
	template <class _Ta, class _Alloc>
	bool operator>=(const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return !(_Xa < _Xb);
	}
	template <class _Ta, class _Alloc>
	bool operator<=(const vector<_Ta, _Alloc>& _Xa, const vector<_Ta, _Alloc>& _Xb)
	{
		return !(_Xb < _Xa);
	}

	template <class _Ta, class _Alloc>
	void swap(vector<_Ta, _Alloc>& _Xa, vector<_Ta, _Alloc>& _Xb)
	{
		_Xa.swap(_Xb);
	}
}

#endif //_Vector_hpp