#ifndef _mymallocator_hpp_
#define _mymallocator_hpp_
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

template<typename T>
struct MyMallocator
{
	typedef T value_type;
	typedef std::true_type propagate_on_container_copy_assignment;
	typedef std::true_type propagate_on_container_move_assignment;
	typedef std::true_type propagate_on_container_swap;
	
	MyMallocator() = default;
	template<typename U> MyMallocator(const MyMallocator<U>&){}
	
	T* allocate(std::size_t n)
	{
		if(n == 0) return nullptr;
		if(n == static_cast<std::size_t>(-1) / sizeof(T)) throw std::bad_array_new_length();
		void *const p = malloc(n * sizeof(T));
		if(p == nullptr) throw std::bad_alloc();
		return static_cast<T*>(p);
	}
	void deallocate(T* p, std::size_t ) const
	{
		free(p);
	}

	template<typename U>
	inline
		bool operator==(const MyMallocator<U>&) const
	{
		return true;
	}
	template<typename U>
	inline
		bool operator!=(const MyMallocator<U>& o) const
	{
		return !(*this == o);
	}
};

#endif //_mymallocator_hpp_