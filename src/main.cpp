#include <iostream>
#include "MyMallocator.hpp"
#include <type_traits>
#include "vector.hpp"

void ConstructorTest();
void AssignmentTest();
void IteratorTest();
void DimensionTest();
void AccessorTest();
void AddRemoveTest();

template<typename T, typename A, int N>
void PrintMyVec(const custom_std::vector<T, A>&, const char(&)[N]);

using allocator_type = MyMallocator<int>;
using vec_type = custom_std::vector < int, allocator_type >;

int main()
{
	std::cout.sync_with_stdio(false);

	ConstructorTest();
	AssignmentTest();
	IteratorTest();
	DimensionTest();
	AccessorTest();
	AddRemoveTest();

	std::cin.get();
}

void ConstructorTest()
{
	allocator_type alloc;
	vec_type v1;
	vec_type v2(alloc);
	vec_type v3(10);
	vec_type v4(10, 11);
	vec_type v5(v3.begin(), v3.end());
	vec_type v6(v4);
	vec_type v7(std::move(v5));
	vec_type v8(v4, alloc);
	vec_type v9(std::move(v3), alloc);
	vec_type v10{ 11, 22, 33, 44, 55, 66 };

	(void)v1, (void)v2, (void)v6, (void)v7,
		(void)v8, (void)v9, (void)v10;
}
void AssignmentTest()
{
	vec_type v1;
	vec_type v2;
	vec_type v3;

	v1 = v2;
	v2 = std::move(v3);
	v3 = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };

	v1.assign(v3.begin(), v3.end());
	v2.assign(16, 42);
	v3.assign({ 1992304, 5707245, 234985, 2626, 666 });

}
void IteratorTest()
{
	struct X { int a; };
	custom_std::vector<X, MyMallocator<X>> v(1);
	auto allocInstance = v.get_allocator();

	auto i1 = v.begin();
	(void)v.end();
	auto i3 = v.rbegin();
	(void)v.rend();
	auto i5 = v.cbegin();
	(void)v.cend();
	auto i7 = v.crbegin();
	(void)v.crend();

	(void)*i1, (void)*i3, (void)*i5, (void)*i7;
	(void)i1->a, (void)i3->a, (void)i5->a, (void)i7->a;
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
	i2 == i2, i4 == i4, i6 == i6, i8 == i8;
	i2 < i2, i4 < i4, i6 < i6, i8 < i8;

	(void)allocInstance;
}
void DimensionTest()
{
	vec_type v;

	v.size();
	v.max_size();
	v.resize(32);
	v.resize(64, 42);
	v.capacity();
	v.empty();
	v.reserve(1024);
	v.shrink_to_fit();
}
void AccessorTest()
{
	vec_type v(32, 42);
	const vec_type& vr = v;

	v[0], vr[0], v.at(0), vr.at(0);
	v.front(), vr.front(), v.back(), vr.back();
	v.data(), vr.data();
}
void AddRemoveTest()
{
	vec_type v1;
	int a = 24;

	v1.emplace_back(42);
	v1.push_back(a);
	v1.push_back(42);
	v1.pop_back();
	v1.emplace(v1.begin(), 11);
	v1.insert(v1.end() - 1, a);
	v1.insert(v1.end() - 1, 33);
	v1.insert(v1.end(), 16, 24);
	int vs[] = {2, 4, 6, 8, 10};
	v1.insert(v1.end(), vs, vs + std::extent<decltype(vs)>());
	v1.erase(v1.begin() + v1.size() / 2);
	v1.erase(v1.begin(), v1.begin() + v1.size() / 3);

	vec_type v2;
	v2.swap(v1);
	v2.clear();
}

template<typename T, typename A, int N>
void PrintMyVec(const custom_std::vector<T, A>& v, const char(&name)[N])
{
	using std::cout;
	using std::endl;

	cout << name << '(' << v.size() << ", " << v.capacity() << ')' << " {";
	for (const auto& x : v)
	{
		cout << x << ", ";
	}
	cout << "}" << endl;
}
