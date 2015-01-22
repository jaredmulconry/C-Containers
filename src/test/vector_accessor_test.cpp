#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void AccessorTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	AccessorTest();
}

void AccessorTest()
{
	{
		vec_type<int> v(32, 42);
		const vec_type<int>& vr = v;

		v[0], vr[0], v.at(0), vr.at(0);
		v.front(), vr.front(), v.back(), vr.back();
		v.data(), vr.data();
	}

	{
		vec_type<bool> v(32, true);
		const vec_type<bool>& vr = v;

		v[0], vr[0], v.at(0), vr.at(0);
		v.front(), vr.front(), v.back(), vr.back();
		v.flip();
	}
}
