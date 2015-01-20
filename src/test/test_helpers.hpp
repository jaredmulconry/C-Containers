#if !defined(TEST_HELPERS_HPP)
#define TEST_HELPERS_HPP
#include <chrono>
#include <cstddef>
#include <future>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace test_helpers
{
	using timings_type =
		std::vector<std::pair<std::string, std::chrono::microseconds>>;

	struct TestData
	{
		timings_type times;
		std::string name;
		std::ptrdiff_t size;

		TestData() = default;
		TestData(timings_type testTimes, std::string testName,
				 std::ptrdiff_t testSize)
			: times(std::move(testTimes))
			, name(std::move(testName))
			, size(std::move(testSize))
		{
		}
	};

	template <typename C, typename I1, typename I2, typename Op>
	void DualRangeApply(C& c, I1 first1, I1 last1, I2 first2, Op&& op)
	{
		for(; first1 != last1; ++first1, ++first2) {
			op(c, *first1, *first2);
		}
	}

	template <typename T>
	std::vector<T> WaitAndConvert(std::vector<std::future<T>>& handles,
								  std::ptrdiff_t msWait)
	{
		if(handles.size() == 0) return {};

		std::vector<T> data;
		data.reserve(handles.size());

		using namespace std::chrono;

		auto remaining = handles.size();
		while(remaining != 0) {
			for(auto& o : handles) {
				if(o.valid()) {
					auto status = o.wait_for(milliseconds(msWait));
					if(status != std::future_status::timeout) {
						--remaining;
						data.push_back(o.get());
					}
				}
			}

			std::this_thread::sleep_for(seconds(1));
		}

		return data;
	}
	void WaitAndConvert(std::vector<std::future<void>>& handles,
						std::ptrdiff_t msWait);

	void DumpResults(const std::vector<TestData>& data);
}

#endif// TEST_HELPERS_HPP