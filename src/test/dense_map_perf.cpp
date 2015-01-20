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

template <typename K, typename T>
using std_map =
	std::map<K, T, std::less<>, MyMallocator<std::pair<const K, T>>>;

template <typename K, typename T>
using dense_map = custom_containers::DenseMap<K, T, std::less<>,
											  MyMallocator<K>, MyMallocator<T>>;

template <typename K, typename T>
inline void reserve(dense_map<K, T>& x, std::ptrdiff_t n)
{
	x.reserve(n);
}
template <typename K, typename T>
inline void reserve(std_map<K, T>&, std::ptrdiff_t)
{
}

template <typename K, typename T>
inline void make_ordered(dense_map<K, T>& x, std::ptrdiff_t prevSize)
{
	x.make_ordered(prevSize);
}
template <typename K, typename T>
inline void make_ordered(std_map<K, T>&, std::ptrdiff_t)
{
}

struct insert_element
{
	template <typename Key, typename Val, typename K, typename T>
	inline std::pair<typename std_map<Key, Val>::iterator, bool>
		operator()(std_map<Key, Val>& c, K&& k, T&& v) const
	{
		return c.insert(std::make_pair(std::forward<K>(k), std::forward<T>(v)));
	}
	template <typename Key, typename Val, typename K, typename T>
	inline std::pair<typename dense_map<Key, Val>::iterator, bool>
		operator()(dense_map<Key, Val>& c, K&& k, T&& v) const
	{
		return c.insert(std::forward<K>(k), std::forward<T>(v));
	}
};
struct insert_range_element
{
	template <typename Key, typename Val, typename I1, typename I2>
	inline void operator()(std_map<Key, Val>& c, I1 first, I1 last, I2) const
	{
		c.insert(first, last);
	}
	template <typename Key, typename Val, typename I1, typename I2>
	inline void operator()(dense_map<Key, Val>& c, I1 first1, I1 last1,
						   I2 first2) const
	{
		c.insert(first1, last1, first2, first2 + std::distance(first1, last1));
	}
};
struct append_element
{
	using result_type = void;

	template <typename Key, typename Val, typename K, typename T>
	inline void operator()(std_map<Key, Val>& c, K&& k, T&& v) const
	{
		insert_element{}(c, std::forward<K>(k), std::forward<T>(v));
	}
	template <typename Key, typename Val, typename K, typename T>
	inline void operator()(dense_map<Key, Val>& c, K&& k, T&& v) const
	{
		c.append(std::forward<K>(k), std::forward<T>(v));
	}
};

template <typename K, typename T, typename I1, typename I2, typename Op>
void ExecuteTestRange(std_map<K, T>& cont, test_helpers::timings_type& times,
					  const std::string& testName, I1 first1, I1 last1,
					  I2 first2, Op&& op)
{
	using namespace std::chrono;

	std::vector<typename std_map<K, T>::value_type> input;
	for(; first1 != last1; ++first1, ++first2) {
		input.emplace_back(*first1, *first2);
	}

	auto start = high_resolution_clock::now();
	op(cont, input.begin(), input.end(), input.end());
	auto finish = high_resolution_clock::now();
	auto elapsedTime = duration_cast<microseconds>(finish - start);

	times.insert(times.cend(), std::make_pair(testName, elapsedTime));
}

template <typename K, typename T, typename I1, typename I2, typename Op>
void ExecuteTestRange(dense_map<K, T>& cont, test_helpers::timings_type& times,
					  const std::string& testName, I1 first1, I1 last1,
					  I2 first2, Op&& op)
{
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	op(cont, first1, last1, first2);
	auto finish = high_resolution_clock::now();
	auto elapsedTime = duration_cast<microseconds>(finish - start);

	times.insert(times.cend(), std::make_pair(testName, elapsedTime));
}

template <typename C, typename I1, typename I2, typename Op>
void ExecuteTestCase(test_helpers::timings_type& times,
					 const std::string& testName, I1 first1, I1 last1,
					 I2 first2, Op&& op)
{
	using namespace std::chrono;
	C cont;

	auto start = high_resolution_clock::now();
	op(cont, first1, last1, first2);
	auto finish = high_resolution_clock::now();
	auto elapsedTime = duration_cast<microseconds>(finish - start);

	times.insert(times.cend(), std::make_pair(testName, elapsedTime));
}

template <typename C, typename K, typename T>
void GetTimings(test_helpers::timings_type& times,
				const std::vector<K>& keyInput,
				const std::vector<T>& valueInput)
{
	auto inputSize = static_cast<std::ptrdiff_t>(keyInput.size());
	auto sortedKeyInput = keyInput;
	std::sort(std::begin(sortedKeyInput), std::end(sortedKeyInput),
			  typename C::key_compare());

	// insert tests
	ExecuteTestCase<C>(times, "insert", keyInput.begin(), keyInput.end(),
					   valueInput.begin(),
					   [](C& c, auto first1, auto last1, auto first2)
					   {
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														insert_element{});
					   });

	ExecuteTestCase<C>(times, "insert sorted", sortedKeyInput.begin(),
					   sortedKeyInput.end(), valueInput.begin(),
					   [](C& c, auto first1, auto last1, auto first2)
					   {
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														insert_element{});
					   });
	{
		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestCase<C>(
			times, "insert move", kInput.begin(), kInput.end(), vInput.begin(),
			[](C& c, auto first1, auto last1, auto first2)
			{
				test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
											 std::make_move_iterator(last1),
											 std::make_move_iterator(first2),
											 insert_element{});
			});
	}
	ExecuteTestCase<C>(times, "insert (w/ reserve)", keyInput.begin(),
					   keyInput.end(), valueInput.begin(),
					   [&inputSize](C& c, auto first1, auto last1, auto first2)
					   {
						   reserve(c, inputSize);
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														insert_element{});
					   });
	ExecuteTestCase<C>(times, "insert (w/ reserve) sorted",
					   sortedKeyInput.begin(), sortedKeyInput.end(),
					   valueInput.begin(),
					   [&inputSize](C& c, auto first1, auto last1, auto first2)
					   {
						   reserve(c, inputSize);
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														insert_element{});
					   });
	{
		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestCase<C>(
			times, "insert (w/ reserve) move", kInput.begin(), kInput.end(),
			vInput.begin(),
			[&inputSize](C& c, auto first1, auto last1, auto first2)
			{
				reserve(c, inputSize);
				test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
											 std::make_move_iterator(last1),
											 std::make_move_iterator(first2),
											 insert_element{});
			});
	}

	// insert range tests
	{
		C cont;
		ExecuteTestRange(cont, times, "insert range", keyInput.begin(),
						 keyInput.end(), valueInput.begin(),
						 insert_range_element{});
	}
	{
		C cont;
		ExecuteTestRange(cont, times, "insert range sorted",
						 sortedKeyInput.begin(), sortedKeyInput.end(),
						 valueInput.begin(), insert_range_element{});
	}
	{
		C cont;
		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestRange(cont, times, "insert range move", kInput.begin(),
						 kInput.end(), vInput.begin(),
						 [](C& c, auto first1, auto last1, auto first2)
						 {
							 insert_range_element{}(c, std::make_move_iterator(
														   first1),
													std::make_move_iterator(
														last1),
													std::make_move_iterator(
														first2));
						 });
	}
	{
		C cont;

		ExecuteTestRange(cont, times, "insert range (w/ reserve)",
						 keyInput.begin(), keyInput.end(), valueInput.begin(),
						 [&inputSize](C& c, auto first1, auto last1,
									  auto first2)
						 {
							 reserve(c, inputSize);
							 insert_range_element{}(c, first1, last1, first2);
						 });
	}
	{
		C cont;

		ExecuteTestRange(cont, times, "insert range (w/ reserve) sorted",
						 sortedKeyInput.begin(), sortedKeyInput.end(),
						 valueInput.begin(),
						 [&inputSize](C& c, auto first1, auto last1,
									  auto first2)
						 {
							 reserve(c, inputSize);
							 insert_range_element{}(c, first1, last1, first2);
						 });
	}
	{
		C cont;

		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestRange(
			cont, times, "insert range (w/ reserve) move", kInput.begin(),
			kInput.end(), vInput.begin(),
			[&inputSize](C& c, auto first1, auto last1, auto first2)
			{
				reserve(c, inputSize);
				insert_range_element{}(c, std::make_move_iterator(first1),
									   std::make_move_iterator(last1),
									   std::make_move_iterator(first2));
			});
	}

	// append tests
	ExecuteTestCase<C>(times, "append", keyInput.begin(), keyInput.end(),
					   valueInput.begin(),
					   [](C& c, auto first1, auto last1, auto first2)
					   {
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														append_element{});
					   });

	ExecuteTestCase<C>(times, "append sorted", sortedKeyInput.begin(),
					   sortedKeyInput.end(), valueInput.begin(),
					   [](C& c, auto first1, auto last1, auto first2)
					   {
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														append_element{});
					   });
	{
		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestCase<C>(
			times, "append move", kInput.begin(), kInput.end(), vInput.begin(),
			[](C& c, auto first1, auto last1, auto first2)
			{
				test_helpers::DualRangeApply(c, std::make_move_iterator(first1),
											 std::make_move_iterator(last1),
											 std::make_move_iterator(first2),
											 append_element{});
			});
	}
	ExecuteTestCase<C>(times, "append (w/ reserve)", keyInput.begin(),
					   keyInput.end(), valueInput.begin(),
					   [&inputSize](C& c, auto first1, auto last1, auto first2)
					   {
						   reserve(c, inputSize);
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														append_element{});
					   });
	ExecuteTestCase<C>(times, "append (w/ reserve) sorted",
					   sortedKeyInput.begin(), sortedKeyInput.end(),
					   valueInput.begin(),
					   [&inputSize](C& c, auto first1, auto last1, auto first2)
					   {
						   reserve(c, inputSize);
						   test_helpers::DualRangeApply(c, first1, last1,
														first2,
														append_element{});
					   });
	{
		auto kInput = keyInput;
		auto vInput = valueInput;
		ExecuteTestCase<C>(
			times, "append (w/ reserve) move", kInput.begin(), kInput.end(),
			vInput.begin(),
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

template <typename C, typename K, typename T>
test_helpers::TestData RunTest(const char* name, const std::vector<K>& keyInput,
							   const std::vector<T>& valueInput)
{
	test_helpers::timings_type times;
	GetTimings<C>(times, keyInput, valueInput);
	return {std::move(times), name, static_cast<std::ptrdiff_t>(keyInput.size())};
}

int main()
{
	using namespace std::literals::chrono_literals;
	std::vector<std::future<test_helpers::TestData>> timingResults;

	std::ptrdiff_t testSize = 64 * 1024;
	auto intInput1 = InputRandomiser::GetInput<int>(testSize);
	auto intInput2 = InputRandomiser::GetInput<int>(testSize);
	auto stringInput1 = InputRandomiser::GetInput<std::string>(testSize);
	auto stringInput2 = InputRandomiser::GetInput<std::string>(testSize);

	timingResults.push_back(
		std::async(std::launch::async, RunTest<std_map<int, int>, int, int>,
				   "std_map<int, int>", intInput1, intInput2));
	timingResults.push_back(
		std::async(std::launch::async, RunTest<dense_map<int, int>, int, int>,
				   "dense_map<int, int>", intInput1, intInput2));

	timingResults.push_back(
		std::async(std::launch::async,
				   RunTest<std_map<std::string, int>, std::string, int>,
				   "std_map<std::string, int>", stringInput1, intInput2));
	timingResults.push_back(
		std::async(std::launch::async,
				   RunTest<dense_map<std::string, int>, std::string, int>,
				   "dense_map<std::string, int>", stringInput1, intInput2));

	timingResults.push_back(
		std::async(std::launch::async,
				   RunTest<std_map<int, std::string>, int, std::string>,
				   "std_map<int, std::string>", intInput1, stringInput2));
	timingResults.push_back(
		std::async(std::launch::async,
				   RunTest<dense_map<int, std::string>, int, std::string>,
				   "dense_map<int, std::string>", intInput1, stringInput2));

	timingResults.push_back(std::async(
		std::launch::async,
		RunTest<std_map<std::string, std::string>, std::string, std::string>,
		"std_map<std::string, std::string>", stringInput1, stringInput2));
	timingResults.push_back(std::async(
		std::launch::async,
		RunTest<dense_map<std::string, std::string>, std::string, std::string>,
		"dense_map<std::string, std::string>", stringInput1, stringInput2));

	std::this_thread::sleep_for(2s);

	test_helpers::DumpResults(test_helpers::WaitAndConvert(timingResults, 10));
}