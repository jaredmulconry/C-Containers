#include "dense_map.hpp"
#include "MyMallocator.hpp"
#include "randomised_input.hpp"
#include <chrono>
#include <cstddef>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <thread>
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

	using namespace std::chrono;

	{
		const int inputSize = 32 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin();
		C cont;

		auto start = high_resolution_clock::now();
		for (; kBegin != kEnd; ++kBegin, ++vBegin)
		{
			insert_element(cont, *kBegin, *vBegin);
		}
		auto finish = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(finish - start);

		times.insert(std::make_pair("32 * 1024 key-value inserts", elapsedTime));
	}
	{
		const int inputSize = 32 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin();
		C cont;		

		auto start = high_resolution_clock::now();

		reserve(cont, inputSize);
		for (; kBegin != kEnd; ++kBegin, ++vBegin)
		{
			insert_element(cont, *kBegin, *vBegin);
		}
		auto finish = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(finish - start);

		times.insert(std::make_pair("32 * 1024 key-value inserts (w/ reserve)", elapsedTime));
	}
	{
		const int inputSize = 32 * 1024;
		auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
		auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);
		auto kBegin = keyInput.begin(); auto kEnd = keyInput.end();
		auto vBegin = valueInput.begin();
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

		times.insert(std::make_pair("32 * 1024 key-value inserts vs append", elapsedTime));
	}
}

template<typename C>
std::string RunTest(const char* name)
{
	std::string ret;
	timings_type times;
	GetTimings<C>(times);

	ret += name;
	ret.push_back('\n');
	for (const auto& x : times)
	{
		ret += x.first;
		ret.push_back(' ');
		ret += std::to_string(x.second.count());
		ret += "us\n";
	}
	
	return ret;
}

void DumpResults(std::vector<std::future<std::string>>& x)
{
	using namespace std::literals;

	auto remaining = x.size();
	while (remaining != 0)
	{
		for (auto& o : x)
		{
			if (o.valid())
			{
				auto status = o.wait_for(10ms);
				if (status != std::future_status::timeout)
				{
					--remaining;
					std::cout << o.get();
				}
			}
		}

		std::this_thread::yield();
	}
}

int main()
{
	std::vector<std::future<std::string>> timingResults;

	timingResults.push_back(std::async(RunTest<std_map<int, int>>, "std_map<int, int>"));
	timingResults.push_back(std::async(RunTest<dense_map<int, int>>, "dense_map<int, int>"));
	
	timingResults.push_back(std::async(RunTest<std_map<std::string, int>>, "std_map<std::string, int>"));
	timingResults.push_back(std::async(RunTest<dense_map<std::string, int>>, "dense_map<std::string, int>"));
	
	timingResults.push_back(std::async(RunTest<std_map<int, std::string>>, "std_map<int, std::string>"));
	timingResults.push_back(std::async(RunTest<dense_map<int, std::string>>, "dense_map<int, std::string>"));
	
	timingResults.push_back(std::async(RunTest<std_map<std::string, std::string>>, "std_map<std::string, std::string>"));
	timingResults.push_back(std::async(RunTest<dense_map<std::string, std::string>>, "dense_map<std::string, std::string>"));

	DumpResults(timingResults);
}