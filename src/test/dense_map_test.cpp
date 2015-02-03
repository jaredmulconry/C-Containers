#include "dense_map.hpp"
#include <future>
#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include <thread>
#include <vector>

void ConstructorTest();
void AssignmentTest();
void IteratorTest();
void DimensionTest();
void AccessorTest();
void AddRemoveTest();
void RelationalTest();

template <typename T>
using comparison_type = std::less<T>;
template <typename K, typename T>
using dense_map = custom_containers::DenseMap<K, T, comparison_type<void>,
											  MyMallocator<K>, MyMallocator<T>>;
template <typename K, typename T>
using value_type = typename dense_map<K, T>::value_type;

template <typename T, typename U>
using KeyAllocator = typename dense_map<T, U>::allocator_type::first_type;
template <typename T, typename U>
using ValueAllocator = typename dense_map<T, U>::allocator_type::second_type;

int main()
{
	std::vector<std::future<void>> futures;
	futures.reserve(8);

	futures.push_back(std::async(ConstructorTest));
	futures.push_back(std::async(AssignmentTest));
	futures.push_back(std::async(IteratorTest));
	futures.push_back(std::async(DimensionTest));
	futures.push_back(std::async(AccessorTest));
	futures.push_back(std::async(AddRemoveTest));
	futures.push_back(std::async(RelationalTest));

	std::this_thread::yield();

	test_helpers::WaitAndConvert(futures, 10);
}

void ConstructorTest()
{
	std::vector<int> k{3, 4, 7, 11, 14, 25, 9, 1, 1, 6};
	std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};
	dense_map<int, int> m1;
	dense_map<int, int> m2{comparison_type<void>()};
	dense_map<int, int> m3(k.begin(), k.end(), v.begin(), v.end());
	dense_map<int, int> m4(v.begin(), v.end(), k.begin(), k.end(),
						   KeyAllocator<int, int>(),
						   ValueAllocator<int, int>());
	dense_map<int, int> m5(m3);
	dense_map<int, int> m6(m4, KeyAllocator<int, int>(),
						   ValueAllocator<int, int>());
	dense_map<int, int> m7(std::move(m3));
	dense_map<int, int> m8(std::move(m4), KeyAllocator<int, int>(),
						   ValueAllocator<int, int>());
	dense_map<int, int> m9(init);
	dense_map<int, int> m10(init, KeyAllocator<int, int>(),
							ValueAllocator<int, int>());

	(void)m1, (void)m2, (void)m3, (void)m4, (void)m5, (void)m6, (void)m7,
		(void)m8, (void)m9, (void)m10;
}
void AssignmentTest()
{
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};

	dense_map<int, int> m1(init);
	dense_map<int, int> m2(init);
	dense_map<int, int> m3(init);

	m1 = m2;
	m2 = std::move(m3);
	m3 = init;
}
void IteratorTest()
{
	int a = 33;
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};

	dense_map<int, int> v(init);
	auto allocInstance = v.get_allocator();

	auto i1 = v.begin();
	(void)v.end();
	auto i3 = v.rbegin();
	(void)v.rend();
	auto i5 = v.cbegin();
	(void)v.cend();
	auto i7 = v.crbegin();
	(void)v.crend();
	(void)v.begin_values();
	(void)v.end_values();
	(void)v.rbegin_values();
	(void)v.rend_values();
	(void)v.cbegin_values();
	(void)v.cend_values();
	(void)v.crbegin_values();
	(void)v.crend_values();

	(*i1).second = a, (*i3).second = a, (void)*i5, (void)*i7;
	(void)++i1, (void)++i3, (void)++i5, (void)++i7;
	(void)i1++, (void)i3++, (void)i5++, (void)i7++;
	(void)--i1, (void)--i3, (void)--i5, (void)--i7;
	(void)i1--, (void)i3--, (void)i5--, (void)i7--;
	(void)(i1 + 1), (void)(i3 + 1), (void)(i5 + 1), (void)(i7 + 1);
	(void)(i1 - 1), (void)(i3 - 1), (void)(i5 - 1), (void)(i7 - 1);
	(void)(i1 - i1), (void)(i3 - i3), (void)(i5 - i5), (void)(i7 - i7);
	i1 = i1, i3 = i3, i5 = i5, i7 = i7;
	auto i2 = i1;
	auto i4 = i3;
	auto i6 = i5;
	auto i8 = i7;

	i2[0], i4[0], i6[0], i8[0];
	(void)(i2 == i2), (void)(i4 == i4), (void)(i6 == i6), (void)(i8 == i8);
	(void)(i2 < i2), (void)(i4 < i4), (void)(i6 < i6), (void)(i8 < i8);

	(void)allocInstance;
}
void DimensionTest()
{
	dense_map<int, int> m1;

	m1.size();
	m1.max_size();
	m1.capacity();
	m1.empty();
	m1.reserve(1024);
	m1.shrink_to_fit();
}
void AccessorTest()
{
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};

	dense_map<int, int> m(init);
	const dense_map<int, int>& mr = m;

	m[0] = 42, m.at(0), mr.at(0);
	(void)m.find(3);
	(void)mr.find(11);
	(void)m.lower_bound(5);
	(void)mr.lower_bound(3);
	(void)m.upper_bound(2);
	(void)mr.upper_bound(5);
	(void)m.equal_range(5);
	(void)mr.equal_range(1);
}
void AddRemoveTest()
{
	std::vector<int> k{3, 4, 7, 11, 14, 25, 9, 1, 1, 6};
	std::vector<int> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};

	dense_map<int, int> m1(init);
	int a = 24;
	int b = 42;

	m1.insert(value_type<int, int>{a, b});
	m1.insert(m1.begin() + 1, value_type<int, int>{2, 5});
	m1.insert(42, 24);
	m1.insert(m1.end() - 1, a, b);
	m1.insert(m1.end() - 1, 33, 22);
	m1.insert(k.begin(), k.end(), v.begin(), v.end());
	m1.insert(init);

	m1.insert_or_assign(value_type<int, int>{a, b});
	m1.insert_or_assign(m1.begin() + 1, value_type<int, int>{2, 5});
	m1.insert_or_assign(42, 24);
	m1.insert_or_assign(m1.end() - 1, a, b);
	m1.insert_or_assign(m1.end() - 1, 33, 22);

	m1.erase(m1.begin() + m1.size() / 2);
	m1.erase(m1.begin(), m1.begin() + m1.size() / 3);

	dense_map<int, int> m2;
	m2.swap(m1);
	m2.clear();
}
void RelationalTest()
{
	auto init = std::initializer_list<value_type<int, int>>{{3, 5},
															{2, 4},
															{1, 1},
															{4, 4},
															{1, 3}};

	dense_map<int, int> m1{init};
	dense_map<int, int> m2{init};

	(void)(m1 == m1);
	(void)(m1 == m2);
	(void)(m1 != m2);
	(void)(m1 < m2);
	(void)(m1 > m2);
	(void)(m1 <= m1);
	(void)(m2 >= m2);

	(void)m1.key_comp();
	(void)m2.value_comp();
}
