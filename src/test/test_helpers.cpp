#include "test_helpers.hpp"
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>

namespace test_helpers
{
	void DumpResults(const std::vector<TestData>& data)
	{
		using vec_str = std::vector<std::string>;
		using pairvec_str = std::pair<vec_str, std::size_t>;

		if (data.size() == 0) return;

		const std::size_t columnCount = data.size() + 1;
		const std::size_t columnHeight = data[0].times.size() + 1;

		std::vector<pairvec_str> output;
		
		output.reserve(columnCount);

		{
			pairvec_str::first_type testNames;
			pairvec_str::second_type longestLine = 0;
			testNames.reserve(columnHeight);

			testNames.emplace_back();

			std::transform(data[0].times.cbegin(), data[0].times.cend(),
							std::back_inserter(testNames), 
				[&longestLine](timings_type::const_reference x)
				{
					auto str = x.first;
					longestLine = std::max(longestLine, str.size() + 1);
					return str;
				});

			output.emplace_back(std::move(testNames), longestLine);
		}

		auto dFirst = data.begin();
		for (std::size_t i = 1; i < columnCount; ++i, ++dFirst)
		{
			pairvec_str::first_type testResults;
			pairvec_str::second_type longestLine = 0;
			testResults.reserve(columnHeight);

			{
				auto str = dFirst->name + '(' + std::to_string(dFirst->size) +
					')';
				longestLine = str.size() + 1;
				testResults.push_back(std::move(str));
			}

			std::transform(dFirst->times.begin(), dFirst->times.end(),
							std::back_inserter(testResults),
				[&longestLine](timings_type::const_reference x)
				{
					auto str = std::to_string(x.second.count());
					longestLine = std::max(longestLine, str.size() + 1);
					return str;
				});
			output.emplace_back(std::move(testResults), longestLine);
		}

		for (std::size_t i = 0; i < columnHeight; ++i)
		{
			for (auto& res : output)
			{
				std::cout << std::setw(res.second) << std::left << res.first[i];
			}

			std::cout << std::endl;
		}
	}
}
