#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void DimensionTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	DimensionTest();
}

void DimensionTest()
{
	{
		vec_type<int> v;

		v.size();
		v.max_size();
		v.resize(32);
		v.resize(64, 42);
		v.capacity();
		v.empty();
		v.reserve(1024);
		v.shrink_to_fit();
	}

	{
		vec_type<bool> v;

		v.size();
		v.max_size();
		v.resize(32);
		v.resize(64, true);
		v.capacity();
		v.empty();
		v.reserve(1024);
		v.shrink_to_fit();
	}
}
