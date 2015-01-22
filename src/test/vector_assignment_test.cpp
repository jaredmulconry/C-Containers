#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void AssignmentTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	AssignmentTest();
}

void AssignmentTest()
{
	{
		vec_type<int> v1;
		vec_type<int> v2;
		vec_type<int> v3;

		v1 = v2;
		v2 = std::move(v3);
		v3 = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };

		v1.assign(v3.begin(), v3.end());
		v2.assign(16, 42);
		v3.assign({ 1992304, 5707245, 234985, 2626, 666 });
	}

	{
		vec_type<bool> v1;
		vec_type<bool> v2;
		vec_type<bool> v3;

		v1 = v2;
		v2 = std::move(v3);
		v3 = { false, true, true, true, true, false, false, true };

		v1.assign(v3.begin(), v3.end());
		v2.assign(16, true);
		v3.assign({ false, false, true, true, true, false, true });
	}
}
