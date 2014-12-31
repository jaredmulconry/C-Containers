#include "dense_map.hpp"
#include "MyMallocator.hpp"
#include <vector>

int main()
{
	std::vector<int> keys{ 1, 5, 7, 3, 6, 77, 11, 5, 2, 9 };
	std::vector<int> values{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	custom_containers::DenseMap<int, int> dm{ std::begin(keys), std::end(keys),
										std::begin(values), std::end(values) };
}