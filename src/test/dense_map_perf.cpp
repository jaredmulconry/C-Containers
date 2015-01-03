#include "dense_map.hpp"
#include "MyMallocator.hpp"
#include "randomised_input.hpp"
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using timings_type = std::map<std::string, std::chrono::microseconds>;

template<typename K, typename T>
using std_map = std::map<K, T, std::less<>, MyMallocator<std::pair<const K, T>>>;

template<typename K, typename T>
using dense_map = custom_containers::DenseMap < K, T, std::less<>,
										MyMallocator<K>, MyMallocator < T >> ;

template<typename K, typename T>
void reserve(dense_map<K, T>& x, std::ptrdiff_t n)
{
	x.reserve(n);
}
template<typename K, typename T>
void reserve(std_map<K, T>&, std::ptrdiff_t)
{}

template<typename K, typename T>
void make_ordered(dense_map<K, T>& x, std::ptrdiff_t prevSize)
{
	x.make_ordered(prevSize);
}
template<typename K, typename T>
void make_ordered(std_map<K, T>&, std::ptrdiff_t)
{}

template<typename Key, typename Val, typename K, typename T>
std::pair<typename std_map<Key, Val>::iterator, bool> insert_element(std_map<Key, Val>& c, K&& k, T&& v)
{
	return c.insert(std::make_pair(std::forward<K>(k), std::forward<T>(v)));
}
template<typename Key, typename Val, typename K, typename T>
std::pair<typename dense_map<Key, Val>::iterator, bool> insert_element(dense_map<Key, Val>& c, K&& k, T&& v)
{
	return c.insert(std::forward<K>(k), std::forward<T>(v));
}

template<typename Key, typename Val, typename K, typename T>
void append_element(std_map<Key, Val>& c, K&& k, T&& v)
{
	insert_element(c, std::forward<K>(k), std::forward<T>(v));
}
template<typename Key, typename Val, typename K, typename T>
void append_element(dense_map<Key, Val>& c, K&& k, T&& v)
{
	c.append(std::forward<K>(k), std::forward<T>(v));
}

template<typename C>
void GetTimings(timings_type& times)
{
	using key_type = typename C::key_type;
	using mapped_type = typename C::mapped_type;
	using value_type = typename C::value_type;

	using namespace std::chrono;

	/*{
		const int inputSize = 64 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin(); auto vEnd = valueInput.end();
		C cont;

		auto start = high_resolution_clock::now();
		for (; kBegin != kEnd; ++kBegin, ++vBegin)
		{
			insert_element(cont, *kBegin, *vBegin);
		}
		auto finish = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(finish - start);

		times.insert(std::make_pair("64 * 1024 key-value inserts", elapsedTime));
	}
	{
		const int inputSize = 64 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin(); auto vEnd = valueInput.end();
		C cont;		

		auto start = high_resolution_clock::now();

		reserve(cont, inputSize);
		for (; kBegin != kEnd; ++kBegin, ++vBegin)
		{
			insert_element(cont, *kBegin, *vBegin);
		}
		auto finish = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(finish - start);

		times.insert(std::make_pair("64 * 1024 key-value inserts (w/ reserve)", elapsedTime));
	}*/
	{
		const int inputSize = 256 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin(); auto vEnd = valueInput.end();
		C cont;

		std::sort(kBegin, kEnd, std::greater_equal<>());

		auto start = high_resolution_clock::now();
		reserve(cont, inputSize);
		for (; kBegin != kEnd; ++kBegin, ++vBegin)
		{
			append_element(cont, *kBegin, *vBegin);
		}
		make_ordered(cont, 0);
		auto finish = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(finish - start);

		times.insert(std::make_pair("64 * 1024 key-value inserts vs append", elapsedTime));
	}
}

int main()
{
	using namespace std;

	timings_type times;
	{
		GetTimings<std_map<int, int>>(times);

		cout << "std::map" << endl;
		for (const auto& x : times)
		{
			cout << x.first << ' ' << x.second.count() << endl;
		}
	}

	times.clear();

	{
		GetTimings<dense_map<int, int>>(times);

		cout << "dense_map" << endl;
		for (const auto& x : times)
		{
			cout << x.first << ' ' << x.second.count() << endl;
		}
	}
}