#if !defined(RANDOMISED_INPUT_HPP)
#define RANDOMISED_INPUT_HPP
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <limits>
#include <random>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace InputRandomiser
{
	template<typename T>
	inline std::vector<T> GetInput(std::ptrdiff_t);
	namespace detail
	{
		template<typename T, typename Traits, typename Alloc>
		void generate_random_input(std::ptrdiff_t n,
			std::vector<std::basic_string<T, Traits, Alloc>>& res)
		{
			static const std::ptrdiff_t textLength = 32;
			std::vector<T> inputText;

			for (std::ptrdiff_t i = 0; i < n; ++i)
			{
				inputText = GetInput<T>(textLength);
				res.emplace_back(inputText.begin(), inputText.end());
			}
		}

		template<typename T>
		std::vector<T> GetInputNumeric(std::ptrdiff_t n, std::mt19937& rng, std::true_type)
		{
			std::vector<T> res;
			res.reserve(n);

			using num_traits = std::numeric_limits<T>;
			static thread_local std::uniform_int_distribution<T> dist{ 
													num_traits::lowest(), 
													num_traits::max() };

			std::generate_n(std::back_inserter(res), n, [&rng]()
			{
				return dist(rng);
			});

			return res;
		}
		template<>
		std::vector<bool> GetInputNumeric<bool>(std::ptrdiff_t n, std::mt19937& rng, std::true_type)
		{
			std::vector<bool> res;
			res.reserve(n);

			static const int bitsPerInput = sizeof(std::mt19937::result_type) * 8;
			
			auto blockCount = n % bitsPerInput;

			for (std::ptrdiff_t i = 0; i < blockCount; ++i)
			{
				auto bits = rng();
				for (std::ptrdiff_t j = 0; j < bitsPerInput; ++j)
				{
					res.push_back((bits & (1 << j)) != 0);
				}
			}

			n -= blockCount * bitsPerInput;

			auto bits = rng();
			for (std::ptrdiff_t j = 0; j < n; ++j)
			{
				res.push_back((bits & (1 << j)) != 0);
			}

			return res;
		}
		template<>
		std::vector<char> GetInputNumeric<char>(std::ptrdiff_t n, std::mt19937& rng, std::true_type)
		{
			std::vector<char> res;
			res.reserve(n);

			//33 to 126
			static thread_local std::uniform_int_distribution<short> dist{ 33, 126 };

			for (std::ptrdiff_t i = 0; i < n; ++i)
			{
				res.push_back(char(dist(rng)));
			}

			return res;
		}
		template<typename T>
		//T is presumably floating-point
		std::vector<T> GetInputNumeric(std::ptrdiff_t n, std::mt19937& rng, std::false_type)
		{
			std::vector<T> res;

			using num_traits = std::numeric_limits<T>;
			static thread_local std::uniform_real_distribution<T> dist{num_traits::lowest(),
																num_traits::max()};

			std::generate_n(std::back_inserter(res), n, [&rng]()
			{
				return dist(rng);
			});

			return res;
		}

		template<typename T>
		//T is Arithmetic
		inline std::vector<T> GetInputInternal(std::ptrdiff_t n, std::true_type)
		{
			static thread_local std::mt19937 rng;
			return GetInputNumeric<T>(n, rng, typename std::is_integral<T>::type());
		}

		template<typename T>
		//T is not Arithmetic
		std::vector<T> GetInputInternal(std::ptrdiff_t n, std::false_type)
		{
			std::vector<T> res;
			res.reserve(n);

			generate_random_input(n, res);

			return res;
		}
	}

	template<typename T>
	inline std::vector<T> GetInput(std::ptrdiff_t n)
	{
		static_assert(!std::is_array<T>::value, "Array input is not supported.");
		static_assert(!std::is_pointer<T>::value, "Pointer input is not supported.");

		return detail::GetInputInternal<T>(n, typename std::is_arithmetic<T>::type());
	}
}

#endif //RANDOMISED_INPUT_HPP