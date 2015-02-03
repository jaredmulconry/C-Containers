#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void RelationalTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	RelationalTest();
}

void RelationalTest()
{
	{
		vec_type<int> v1{ 3, 5, 6, 6, 9, 23, 11 };
		vec_type<int> v2{ 3, 5, 6, 6, 9, 23, 15 };

		(void)(v1 == v1);
		(void)(v1 == v2);
        (void)(v1 != v2);
        (void)(v1 < v2);
		(void)(v1 > v2);
		(void)(v1 <= v1);
		(void)(v2 >= v2);
	}

	{
		vec_type<bool> v1{ false, false, false, true, true, true, true };
		vec_type<bool> v2{ false, false, false, true, false, true, true };

		(void)(v1 == v1);
		(void)(v1 == v2);
		(void)(v1 != v2);
		(void)(v1 < v2);
		(void)(v1 > v2);
		(void)(v1 <= v1);
		(void)(v2 >= v2);
	}
}
