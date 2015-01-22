#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void AddRemoveTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	AddRemoveTest();
}

void AddRemoveTest()
{
	{
		vec_type<int> v1;
		int a = 24;

		v1.emplace_back(42);
		v1.push_back(a);
		v1.push_back(42);
		v1.pop_back();
		v1.emplace(v1.begin(), 11);
		v1.insert(v1.end() - 1, a);
		v1.insert(v1.end() - 1, 33);
		v1.insert(v1.end(), 16, 24);
		int vs[] = { 2, 4, 6, 8, 10 };
		v1.insert(v1.end(), vs, vs + std::extent<decltype(vs)>());
		v1.erase(v1.begin() + v1.size() / 2);
		v1.erase(v1.begin(), v1.begin() + v1.size() / 3);

		vec_type<int> v2;
		v2.swap(v1);
		v2.clear();
	}

	{
		vec_type<bool> v1;
		bool a = true;

		v1.push_back(a);
		v1.push_back(false);
		v1.pop_back();
		v1.insert(v1.end() - 1, a);
		v1.insert(v1.end() - 1, false);
		v1.insert(v1.end(), 16, true);
		bool vs[] = { false, false, false, false, true };
		v1.insert(v1.end(), vs, vs + std::extent<decltype(vs)>());
		vec_type<bool>::swap(v1[0], v1[3]);
		v1.erase(v1.begin() + v1.size() / 2);
		v1.erase(v1.begin(), v1.begin() + v1.size() / 3);

		vec_type<bool> v2;
		v2.swap(v1);
		v2.clear();
	}
}
