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
		~_Vector_const_iterator() = default;
		template<typename _Iter, class =
			typename ::std::enable_if<::std::is_base_of<_MyTa,
			_Iter>::value>::type>
			explicit _Vector_const_iterator(const _Iter& _XiDerived) noexcept
			:_Pos(static_cast<const _MyTa&>(_XiDerived)._Pos)
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

		reference operator[](difference_type _Xn) const
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
		~_Vector_iterator() = default;
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

		reference operator[](difference_type _Xn) const
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

	template<typename _Ta, typename _Alloc = ::std::allocator<_Ta>>
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

		template<typename _InputIter>
		void _ConstructIterators(_InputIter _XiFirst, _InputIter _XiLast,
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
		template<typename _ForwardIter>
		void _ConstructIterators(_ForwardIter _XiFirst, _ForwardIter _XiLast,
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

		template<typename _ForwardIter>
		pointer _CopyRange(_ForwardIter _XiFirst, _ForwardIter _XiLast, 
			pointer _XoFirst)
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
		template<typename _InputIter>
		pointer _AssignRange(_InputIter _XiFirst, _InputIter _XiLast, 
			pointer _XoFirst)
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

		template<typename _InputIter>
		void _Transfer(_InputIter _XiFirst, _InputIter _XiLast, pointer _XoFirst,
			::std::true_type)
		{
			for (; _XiFirst != _XiLast; ++_XiFirst, (void)++_XoFirst)
				this->_ConstructElem(_XoFirst, ::std::move(*_XiFirst));
		}
		template<typename _InputIter>
		void _Transfer(_InputIter _XiFirst, _InputIter _XiLast, pointer _XoFirst,
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
		template<typename _InputIter>
		void _Transfer(_InputIter _XiFirst, _InputIter _XiLast,
			pointer _XoFirst)
		{
			this->_Transfer(_XiFirst, _XiLast, _XoFirst,
				typename ::std::is_nothrow_move_constructible<_Ta>::type());
		}

		template<typename _InputIter>
		void _InsertRange(const_iterator _Xpos, _InputIter _XiFirst,
			_InputIter _XiLast, ::std::input_iterator_tag)
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
		template<typename _ForwardIter>
		void _InsertRange(const_iterator _Xpos, _ForwardIter _XiFirst,
			_ForwardIter _XiLast, ::std::forward_iterator_tag)
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
		template<typename _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value
										>::type>
			vector(_InputIter _XiFirst, _InputIter _XiLast,
			const _Alloc& _Xalloc = _Alloc())
			: vector(_Xalloc)
		{
			this->_ConstructIterators(_XiFirst, _XiLast,
				typename ::std::iterator_traits<_InputIter>::iterator_category());
		}
		vector(const vector& _Xa)
			:vector(_TaAllocTraits::select_on_container_copy_construction(_Xa._Al))
		{
			auto _Size = _Xa.size();
			if (!this->_IsValidCapacity(_Size)) return;
			this->_Reserve(_Size);

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
			::std::equal(_Xa.begin(), _Xa.end(), _Xb.begin());
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

	typedef unsigned int	_VecBoolStoreType;
	const int _VecBoolBitCount = 8 * sizeof(_VecBoolStoreType);

	template<typename _MyVec>
	class _Vector_bool_iterator_storage
	{
	public:
		typedef _Vector_bool_iterator_storage<_MyVec> _MyTa;
		typedef typename _MyVec::size_type	size_type;
		typedef _VecBoolStoreType	_StoreType;
		typedef _StoreType*	_StorePtr;

		_StorePtr _Pos;
		size_type _Off;

		_Vector_bool_iterator_storage() noexcept = default;
		_Vector_bool_iterator_storage(const _MyTa&) noexcept = default;
		~_Vector_bool_iterator_storage() = default;
		_Vector_bool_iterator_storage(_StorePtr _Xpos, size_type _Xoff)
			: _Pos(_Xpos)
			, _Off(_Xoff)
		{}
		_Vector_bool_iterator_storage& operator=(const _MyTa&)noexcept = default;
	};

	template<typename _MyVec>
	class _Vector_bool_const_iterator : 
		public _Vector_bool_iterator_storage<_MyVec>
	{
	public:
		typedef _Vector_bool_const_iterator<_MyVec>		_MyTa;
		typedef _Vector_bool_iterator_storage<_MyVec>	_MyBase;
		typedef ::std::random_access_iterator_tag		iterator_category;
		typedef typename _MyVec::value_type				value_type;
		typedef typename _MyVec::size_type				size_type;
		typedef typename _MyVec::difference_type		difference_type;
		typedef typename _MyVec::const_reference		const_reference;
		typedef const_reference							reference;
		typedef const_reference*						pointer;
		typedef _VecBoolStoreType						_StoreType;
		typedef _StoreType*								_StorePtr;
		typedef typename _MyVec::reference				_RefProx;

	private:
		void _IncrementPos()
		{
			if (this->_Off < _VecBoolBitCount - 1)
			{
				++this->_Off;
			}
			else
			{
				this->_Off = 0;
				++this->_Pos;
			}
		}
		void _DecrementPos()
		{
			if (this->_Off != 0)
			{
				--this->_Off;
			}
			else
			{
				this->_Off = _VecBoolBitCount - 1;
				--this->_Pos;
			}
		}
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
		_Vector_bool_const_iterator() noexcept = default;
		_Vector_bool_const_iterator(const _MyTa&) noexcept = default;
		~_Vector_bool_const_iterator() = default;
		_Vector_bool_const_iterator(_StorePtr _Xpos, size_type _Xoff)
			:_Vector_bool_iterator_storage<_MyVec>(_Xpos, _Xoff)
		{}
		template<typename _Iter, class =
			typename ::std::enable_if<::std::is_base_of<_MyTa,
			_Iter>::value>::type>
		explicit _Vector_bool_const_iterator(const _Iter& _Iderived)
			: _Vector_bool_const_iterator(
				static_cast<const _MyTa&>(_Iderived)._Pos,
				static_cast<const _MyTa&>(_Iderived)._Off)
		{}
		_Vector_bool_const_iterator& operator=(const _MyTa& _Xa)noexcept
		{
			_CvtRef(*this) = _CvtRef(_Xa);
			return *this;
		}

		const_reference operator*() const
		{
			return _RefProx(*this);
		}

		_MyTa& operator++()
		{
			_IncrementPos();
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
			_DecrementPos();
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

		reference operator[](difference_type _Xn) const
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
			return _VecBoolBitCount * (_Xa._Pos - _Xb._Pos)
				+ static_cast<difference_type>(_Xa._Off)
				- static_cast<difference_type>(_Xb._Off);
		}

		inline friend
			bool operator==(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xa._Pos == _Xb._Pos &&
				_Xa._Off == _Xb._Off;
		}
		inline friend
			bool operator!=(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return !(_Xa == _Xb);
		}
		inline friend
			bool operator<(const _MyTa& _Xa, const _MyTa& _Xb)
		{
			return _Xa._Pos < _Xb._Pos ||
					(_Xa._Pos == _Xb._Pos &&
					_Xa._Off < _Xb._Off);
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
	class _Vector_bool_iterator : public _Vector_bool_const_iterator<_MyVec>
	{
	public:
		typedef _Vector_bool_iterator<_MyVec>			_MyTa;
		typedef _Vector_bool_const_iterator<_MyVec>		_MyBase;
		typedef typename _MyVec::value_type				value_type;
		typedef typename _MyVec::reference				reference;
		typedef typename _MyVec::pointer				pointer;
		typedef typename _MyVec::size_type				size_type;
		typedef typename _MyVec::difference_type		difference_type;
		typedef typename _MyBase::iterator_category		iterator_category;
		typedef _VecBoolStoreType						_StoreType;
		typedef _StoreType*								_StorePtr;
		typedef reference								_RefProx;

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
		_Vector_bool_iterator() noexcept = default;
		_Vector_bool_iterator(const _MyTa&) noexcept = default;
		~_Vector_bool_iterator() = default;
		_Vector_bool_iterator(_StorePtr _Xpos, size_type _Xoff)
			: _MyBase(_Xpos, _Xoff)
		{}
		_Vector_bool_iterator& operator=(const _MyTa& _Xa)noexcept
		{
			_CvtRef(*this) = _CvtRef(_Xa);
			return *this;
		}

		reference operator*() const
		{
			return _RefProx(*this);//(reference)(**((_MyBase* const)this));
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

		reference operator[](difference_type _Xn) const
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

	template<typename _Alloc>
	class vector<bool, _Alloc>
	{
	public:
		typedef vector<bool, _Alloc>	_MyTa;
		typedef _VecBoolStoreType	_StoreType;
		typedef bool	const_reference;
		typedef bool	value_type;
		typedef _Alloc	allocator_type;
		typedef ::std::allocator_traits<_Alloc>	_AllocTraits;
		typedef typename _AllocTraits::template rebind_alloc<_StoreType>	_AllocTa;
		typedef ::std::allocator_traits<_AllocTa> _TaAllocTraits;
		typedef typename _TaAllocTraits::size_type	size_type;
		typedef typename _TaAllocTraits::difference_type	difference_type;
		typedef vector<_StoreType, _AllocTa>	_VecType;
		typedef _Vector_bool_iterator<_MyTa>	iterator;
		typedef _Vector_bool_const_iterator<_MyTa>	const_iterator;
		typedef iterator	pointer;
		typedef const_iterator	const_pointer;
		typedef ::std::reverse_iterator<iterator>	reverse_iterator;
		typedef ::std::reverse_iterator<const_iterator>	const_reverse_iterator;
		class reference : public _Vector_bool_iterator_storage<vector>
		{
			friend class vector;
			typedef _Vector_bool_iterator_storage<vector> _MyBase;
			typedef _VecBoolStoreType	_StoreType;

			_StoreType _Mask() const
			{
				return _StoreType(1) << this->_Off;
			}
			reference() noexcept = default;
		public:
			reference(const _MyBase& _Xa)
				: _MyBase(_Xa)
			{}
			~reference() = default;
			operator bool() const noexcept
			{
				return (*this->_Pos & this->_Mask()) != 0;
			}
			reference& operator=(const bool _Xa) noexcept
			{
				if (_Xa)
				{
					*this->_Pos |= this->_Mask();
				}
				else
				{
					*this->_Pos &= (~this->_Mask());
				}
				return *this;
			}
			reference& operator=(const reference& _Xa) noexcept
			{
				return *this = bool(_Xa);
			}
			void flip() noexcept
			{
				*this->_Pos ^= this->_Mask();
			}
		};

	private:
		_VecType _Store;
		size_type _Size;

		template<typename _InputIter>
		void _InsertRange(const_iterator _Xpos, _InputIter _XiFirst,
						_InputIter _XiLast, ::std::input_iterator_tag)
		{
			auto _Off = _Xpos - this->begin();

			for (; _XiFirst != _XiLast; ++_XiFirst, (void)_Off)
			{
				this->insert(this->begin() + _Off, *_XiFirst);
			}
		}
		template<typename _ForwardIter>
		void _InsertRange(const_iterator _Xpos, _ForwardIter _XiFirst,
			_ForwardIter _XiLast, ::std::forward_iterator_tag)
		{
			auto _Dist = ::std::distance(_XiFirst, _XiLast);
			auto _Off = this->_MakeSpace(_Xpos, _Dist);
			::std::copy(_XiFirst, _XiLast, this->begin() + _Off);
		}
		size_type _MakeSpace(const_iterator _Xpos, size_type _Xn)
		{
			auto _Off = _Xpos - this->begin();

			if (_Xn == 0)
			{
			}
			else if (this->max_size() - this->size() < _Xn)
			{
				throw ::std::length_error("vector<bool> is too large");
			}
			else
			{
				this->_Store.resize(this->_ToUnits(this->size() + _Xn));
				if (this->empty())
				{
					this->_Size += _Xn;
				}
				else
				{
					auto _PrevEnd = this->end();
					this->_Size += _Xn;
					::std::copy_backward(this->begin() + _Off, _PrevEnd,
						this->end());
				}
			}

			return _Off;
		}
		void _Cleanup()
		{
			this->_Store.clear();
			this->_Size = 0;
		}
		static size_type _ToUnits(size_type _Xn)
		{
			return (_Xn + _VecBoolBitCount - 1) / _VecBoolBitCount;
		}
	public:
		explicit vector(const _Alloc& _Xalloc = _Alloc())
			: _Store(_Xalloc)
			, _Size()
		{}
		explicit vector(size_type _Xn, const bool& _Xval = bool(),
			const _Alloc& _Xalloc = _Alloc())
			:_MyTa(_Xalloc)
		{
			if (_Xn == 0) return;

			_StoreType _Val = _Xval ? -1 : 0;
			this->_Store.resize(this->_ToUnits(_Xn), _Val);
			_Size = _Xn;
		}
		template <class _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value
			>::type>
		vector(_InputIter _XiFirst, _InputIter _XiLast,
			const _Alloc& _Xalloc = _Alloc())
			:_MyTa(_Xalloc)
		{
			this->insert(this->begin(), _XiFirst, _XiLast);
		}
		vector(const _MyTa& _Xa)
			:_Store(_Xa._Store)
			,_Size(_Xa._Size)
		{}
		vector(_MyTa&& _Xa)
			:_Store(::std::move(_Xa._Store))
			,_Size(::std::move(_Xa._Size))
		{
			_Xa._Size = 0;
		}
		vector(const _MyTa& _Xa, const _Alloc& _Xalloc)
			:_Store(_Xa._Store, _Xalloc)
			,_Size(_Xa._Size)
		{}
		vector(_MyTa&& _Xa, const _Alloc& _Xalloc)
			:_Store(::std::move(_Xa._Store), _Xalloc)
			,_Size(::std::move(_Xa._Size))
		{
			_Xa._Size = 0;
		}
		vector(::std::initializer_list<bool> _Xelems,
			const _Alloc& _Xalloc = _Alloc())
			:_MyTa(_Xalloc)
		{
			this->insert(this->begin(), _Xelems.begin(), _Xelems.end());
		}
		~vector() = default;
		_MyTa& operator=(const _MyTa& _Xa)
		{
			this->_Store = _Xa._Store;
			this->_Size = _Xa._Size;
			return *this;
		}
		_MyTa& operator=(_MyTa&& _Xa)
		{
			if (this != &_Xa)
			{
				this->clear();
				this->_Store = ::std::move(_Xa._Store);
				this->_Size = ::std::move(_Xa._Size);
				_Xa._Size = 0;
			}
			return *this;
		}
		_MyTa operator=(::std::initializer_list<bool> _Xelems)
		{
			this->assign(_Xelems.begin(), _Xelems.end());
			return *this;
		}
		template <class _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value
			>::type>
			void assign(_InputIter _XiFirst, _InputIter _XiLast)
		{
			this->erase(this->begin(), this->end());
			this->insert(this->begin(), _XiFirst, _XiLast);
		}
		void assign(size_type _Xn, const bool& _Xval)
		{
			this->erase(this->begin(), this->end());
			this->insert(this->begin(), _Xn, _Xval);
		}
		void assign(::std::initializer_list<bool> _Xelems)
		{
			this->assign(_Xelems.begin(), _Xelems.end());
		}
		allocator_type get_allocator() const noexcept
		{
			return this->_Store.get_allocator();
		}

		iterator begin() noexcept
		{
			return iterator(this->_Store.data(), 0);
		}
		const_iterator begin() const noexcept
		{
			return const_iterator(const_cast<_StoreType*>(this->_Store.data()), 0);
		}
		iterator end() noexcept
		{
			return this->begin() + this->size();
		}
		const_iterator end() const noexcept
		{
			return this->begin() + this->size();
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
			return this->begin();
		}
		const_iterator cend() const noexcept
		{
			return this->end();
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return this->rbegin();
		}
		const_reverse_iterator crend() const noexcept
		{
			return this->rend();
		}

		size_type size() const noexcept
		{
			return this->_Size;
		}
		size_type max_size() const noexcept
		{
			auto _MaxSize = this->_Store.max_size();
			
			return _MaxSize < size_type(-1) / _VecBoolBitCount ?
				_MaxSize * _VecBoolBitCount : size_type(-1);
		}
		void resize(size_type _Xn, bool _Xval = false)
		{
			if (this->size() < _Xn)
			{
				this->insert(this->end(), _Xn - this->size(), _Xval);
			}
			else
			{
				this->erase(this->begin() + _Xn, this->end());
			}
		}
		size_type capacity() const noexcept
		{
			return this->_Store.capacity() * _VecBoolBitCount;
		}
		bool empty() const noexcept
		{
			return this->_Size == size_type(0);
		}
		void reserve(size_type n)
		{
			this->_Store.reserve(this->_ToUnits(n));
		}
		void shrink_to_fit()
		{
			this->_Store.shrink_to_fit();
		}
		
		reference operator[](size_type _Xn)
		{
			return *(this->begin() + _Xn);
		}
		const_reference operator[](size_type _Xn) const
		{
			return *(this->cbegin() + _Xn);
		}
		const_reference at(size_type _Xn) const
		{
			if (this->size() < _Xn)
				throw ::std::out_of_range("_Xn >= size()");
			return (*this)[_Xn];
		}
		reference at(size_type _Xn)
		{
			if (this->size() < _Xn)
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
			return *(this->end() - 1);
		}
		const_reference back() const
		{
			return *(this->end() - 1);
		}
		
		void push_back(const bool& x)
		{
			this->insert(this->end(), x);
		}
		void pop_back()
		{
			this->erase(this->end() - 1);
		}
		iterator insert(const_iterator _Xpos, const bool& _Xval)
		{
			return this->insert(_Xpos, size_type(1), _Xval);
		}
		iterator insert(const_iterator _Xpos, size_type _Xn, const bool& _Xval)
		{
			auto _Off = _MakeSpace(_Xpos, _Xn);
			::std::fill(this->begin() + _Off, this->begin() + (_Off + _Xn),
				_Xval);
			return this->begin() + _Off;
		}
		template <class _InputIter, class =
			typename ::std::enable_if < !::std::is_integral<_InputIter>::value
			>::type>
		iterator insert(const_iterator _Xpos, _InputIter _XiFirst, 
						_InputIter _XiLast)
		{
			auto _Off = _Xpos - this->begin();
			this->_InsertRange(_Xpos, _XiFirst, _XiLast,
				typename ::std::iterator_traits<_InputIter>::iterator_category());
			return this->begin() + _Off;
		}
		iterator insert(const_iterator _Xpos, 
						::std::initializer_list<bool> _Xelems)
		{
			return this->insert(_Xpos, _Xelems.begin(), _Xelems.end());
		}
		iterator erase(const_iterator _Xpos)
		{
			return this->erase(_Xpos, _Xpos + 1);
		}
		iterator erase(const_iterator _XiFirst, const_iterator _Xilast)
		{
			auto _Dist = _Xilast - _XiFirst;
			auto _Off = _XiFirst - this->cbegin();
			auto _First = this->begin() + _Off;
			auto _Last = this->begin() + (_Off + _Dist);

			::std::copy(_Last, this->end(), _First);
			this->_Size -= _Dist;
			auto _Count = this->_ToUnits(this->size());
			if (_Count < this->_Store.size())
			{
				this->_Store.erase(this->_Store.begin() + _Count, 
									this->_Store.end());
			}
			if (_Count != 0)
			{
				this->_Store[_Count - 1] &= (_StoreType(1) <<
											this->_Size % _VecBoolBitCount) - 1;
			}

			return this->begin() + _Off;
		}
		void swap(_MyTa& _Xa)
		{
			using ::std::swap;

			this->_Store.swap(_Xa._Store);
			swap(this->_Size, _Xa._Size);
		}
		static void swap(reference _Xa, reference _Xb) noexcept
		{
			bool _Tmp = _Xa;
			_Xa = _Xb;
			_Xb = _Tmp;
		}
		void flip() noexcept
		{
			for (auto& _Xelem : this->_Store)
			{
				_Xelem = ~_Xelem;
			}
		}
		void clear() noexcept
		{
			this->erase(this->begin(), this->end());
		}
	};
}

#endif //_Vector_hpp
