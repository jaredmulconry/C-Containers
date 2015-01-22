#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void ConstructorTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	ConstructorTest();
}

void ConstructorTest()
{
	{
		allocator_type<int> alloc;
		vec_type<int> v1;
		vec_type<int> v2(alloc);
		vec_type<int> v3(10);
		vec_type<int> v4(10, 11);
		vec_type<int> v5(v3.begin(), v3.end());
		vec_type<int> v6(v4);
		vec_type<int> v7(std::move(v5));
		vec_type<int> v8(v4, alloc);
		vec_type<int> v9(std::move(v3), alloc);
		vec_type<int> v10{ 11, 22, 33, 44, 55, 66 };

		(void)v1, (void)v2, (void)v6, (void)v7,
			(void)v8, (void)v9, (void)v10;
	}

	{
		allocator_type<bool> alloc;
		vec_type<bool> v1;
		vec_type<bool> v2(alloc);
		vec_type<bool> v3(10);
		vec_type<bool> v4(10, false);
		vec_type<bool> v5(v3.begin(), v3.end());
		vec_type<bool> v6(v4);
		vec_type<bool> v7(std::move(v5));
		vec_type<bool> v8(v4, alloc);
		vec_type<bool> v9(std::move(v3), alloc);
		vec_type<bool> v10{ true, false, false, true, true };

		(void)v1, (void)v2, (void)v6, (void)v7,
			(void)v8, (void)v9, (void)v10;
	}
}