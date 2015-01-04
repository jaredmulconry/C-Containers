#include "dense_map.hpp"
#include "test_helpers.hpp"
#include "MyMallocator.hpp"
#include "randomised_input.hpp"
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <functional>
#include <future>
#include <iterator>
#include <map>
#include <string>
#include <thread>
#include <vector>

template<typename K, typename T>
using std_map = std::map<K, T, std::less<>, MyMallocator<std::pair<const K, T>>>;

template<typename K, typename T>
using dense_map = custom_containers::DenseMap < K, T, std::less<>,
										MyMallocator<K>, MyMallocator < T >> ;

template<typename K, typename T>
inline void reserve(dense_map<K, T>& x, std::ptrdiff_t n)
{
	x.reserve(n);
}
template<typename K, typename T>
inline void reserve(std_map<K, T>&, std::ptrdiff_t)
{}

template<typename K, typename T>
inline void make_ordered(dense_map<K, T>& x, std::ptrdiff_t prevSize)
{
	x.make_ordered(prevSize);
}
template<typename K, typename T>
inline void make_ordered(std_map<K, T>&, std::ptrdiff_t)
{}

struct insert_element
{
	template<typename Key, typename Val, typename K, typename T>
	inline std::pair<typename std_map<Key, Val>::iterator, bool> operator()
									(std_map<Key, Val>& c, K&& k, T&& v) const
	{
		return c.insert(std::make_pair(std::forward<K>(k), std::forward<T>(v)));
	}
	template<typename Key, typename Val, typename K, typename T>
	inline std::pair<typename dense_map<Key, Val>::iterator, bool> operator()
									(dense_map<Key, Val>& c, K&& k, T&& v) const
	{
		return c.insert(std::forward<K>(k), std::forward<T>(v));
	}
};

struct append_element
{
	using result_type = void;

	template<typename Key, typename Val, typename K, typename T>
	inline void operator()(std_map<Key, Val>& c, K&& k, T&& v) const
	{
		insert_element{}(c, std::forward<K>(k), std::forward<T>(v));
	}
	template<typename Key, typename Val, typename K, typename T>
	inline void operator()(dense_map<Key, Val>& c, K&& k, T&& v) const
	{
		c.append(std::forward<K>(k), std::forward<T>(v));
	}
};

template<typename C, typename I1, typename I2, typename Op>
void ExecuteTestCase(test_helpers::timings_type& times, const std::string& testName, 
	I1 first1, I1 last1, I2 first2, Op&& op)
{
	using namespace std::chrono;
	C cont;

	auto start = high_resolution_clock::now();
	op(cont, first1, last1, first2);
	auto finish = high_resolution_clock::now();
	auto elapsedTime = duration_cast<microseconds>(finish - start);

	times.insert(times.cend(), std::make_pair(testName, elapsedTime));
}

template<typename C>
void GetTimings(test_helpers::timings_type& times, std::ptrdiff_t inputSize)
{
	using key_type = typename C::key_type;
	using mapped_type = typename C::mapped_type;

	const auto keyInput = InputRandomiser::GetInput<key_type>(inputSize);
	const auto valueInput = InputRandomiser::GetInput<mapped_type>(inputSize);

	auto sortedKeyInput = keyInput;
	std::sort(std::begin(sortedKeyInput), std::end(sortedKeyInput),
				typename C::key_compare());

	//insert tests
	ExecuteTestCase<C>(times, "insert", 
			keyInput.begin(), keyInput.end(), valueInput.begin(),
		[](C& c, auto first1, auto last1, auto first2)
		{
			test_helpers::DualRangeApply(c, first1, last1, first2, insert_element{});
		});

	ExecuteTestCase<C>(times, "insert sorted",
			sortedKeyInput.begin(), sortedKeyInput.end(), valueInput.begin(),
		[](C& c, auto first1, auto last1, auto first2)
	{
		test_helpers::DualRangeApply(c, first1, last1, first2, insert_element{});
	});
	{
		auto kInput = keyInput; auto vInput = valueInput;
		ExecuteTestCase<C>(times, "insert move",
				kInput.begin(), kInput.end(), vInput.begin(),
			[](C& c, auto first1, auto last1, auto first2)
		{
			test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
								std::make_move_iterator(last1), 
								std::make_move_iterator(first2), 
								insert_element{});
		});
	}
	ExecuteTestCase<C>(times, "insert (w/ reserve)",
		keyInput.begin(), keyInput.end(), valueInput.begin(),
		[&inputSize](C& c, auto first1, auto last1, auto first2)
	{
		reserve(c, inputSize);
		test_helpers::DualRangeApply(c, first1, last1, first2, insert_element{});
	});
	ExecuteTestCase<C>(times, "insert (w/ reserve) sorted",
		sortedKeyInput.begin(), sortedKeyInput.end(), valueInput.begin(),
		[&inputSize](C& c, auto first1, auto last1, auto first2)
	{
		reserve(c, inputSize);
		test_helpers::DualRangeApply(c, first1, last1, first2, insert_element{});
	});
	{
		auto kInput = keyInput; auto vInput = valueInput;
		ExecuteTestCase<C>(times, "insert (w/ reserve) move",
			kInput.begin(), kInput.end(), vInput.begin(),
			[&inputSize](C& c, auto first1, auto last1, auto first2)
		{
			reserve(c, inputSize);
			test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
								std::make_move_iterator(last1), 
								std::make_move_iterator(first2), 
								insert_element{});
		});
	}

	//append tests
	ExecuteTestCase<C>(times, "append",
		keyInput.begin(), keyInput.end(), valueInput.begin(),
		[](C& c, auto first1, auto last1, auto first2)
	{
		test_helpers::DualRangeApply(c, first1, last1, first2, append_element{});
	});

	ExecuteTestCase<C>(times, "append sorted",
		sortedKeyInput.begin(), sortedKeyInput.end(), valueInput.begin(),
		[](C& c, auto first1, auto last1, auto first2)
	{
		test_helpers::DualRangeApply(c, first1, last1, first2, append_element{});
	});
	{
		auto kInput = keyInput; auto vInput = valueInput;
		ExecuteTestCase<C>(times, "append move",
			kInput.begin(), kInput.end(), vInput.begin(),
			[](C& c, auto first1, auto last1, auto first2)
		{
			test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
							std::make_move_iterator(last1), 
							std::make_move_iterator(first2), 
							append_element{});
		});
	}
	ExecuteTestCase<C>(times, "append (w/ reserve)",
		keyInput.begin(), keyInput.end(), valueInput.begin(),
		[&inputSize](C& c, auto first1, auto last1, auto first2)
	{
		reserve(c, inputSize);
		test_helpers::DualRangeApply(c, first1, last1, first2, append_element{});
	});
	ExecuteTestCase<C>(times, "append (w/ reserve) sorted",
		sortedKeyInput.begin(), sortedKeyInput.end(), valueInput.begin(),
		[&inputSize](C& c, auto first1, auto last1, auto first2)
	{
		reserve(c, inputSize);
		test_helpers::DualRangeApply(c, first1, last1, first2, append_element{});
	});
	{
		auto kInput = keyInput; auto vInput = valueInput;
		ExecuteTestCase<C>(times, "append (w/ reserve) move",
			kInput.begin(), kInput.end(), vInput.begin(),
			[&inputSize](C& c, auto first1, auto last1, auto first2)
		{
			reserve(c, inputSize);
			test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
								std::make_move_iterator(last1), 
								std::make_move_iterator(first2), 
								append_element{});
		});
	}
}

template<typename C>
test_helpers::TestData RunTest(const char* name, std::ptrdiff_t testSize)
{
	test_helpers::timings_type times;
	GetTimings<C>(times, testSize);
	return{ std::move(times), name, testSize };
}

int main()
{
	using namespace std::literals::chrono_literals;
	std::vector<std::future<test_helpers::TestData>> timingResults;

	std::ptrdiff_t testSize = 64 * 1024;

	timingResults.push_back(std::async(RunTest<std_map<int, int>>, "std_map<int, int>", testSize));
	timingResults.push_back(std::async(RunTest<dense_map<int, int>>, "dense_map<int, int>", testSize));
	
	timingResults.push_back(std::async(RunTest<std_map<std::string, int>>, "std_map<std::string, int>", testSize));
	timingResults.push_back(std::async(RunTest<dense_map<std::string, int>>, "dense_map<std::string, int>", testSize));
	
	timingResults.push_back(std::async(RunTest<std_map<int, std::string>>, "std_map<int, std::string>", testSize));
	timingResults.push_back(std::async(RunTest<dense_map<int, std::string>>, "dense_map<int, std::string>", testSize));
	
	timingResults.push_back(std::async(RunTest<std_map<std::string, std::string>>, "std_map<std::string, std::string>", testSize));
	timingResults.push_back(std::async(RunTest<dense_map<std::string, std::string>>, "dense_map<std::string, std::string>", testSize));

	std::this_thread::sleep_for(2s);

	test_helpers::DumpResults(test_helpers::WaitAndConvert(timingResults, 10));
}