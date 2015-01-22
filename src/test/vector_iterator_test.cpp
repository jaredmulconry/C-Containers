#include "MyMallocator.hpp"
#include "test_helpers.hpp"
#include "vector.hpp"
#include <future>
#include <thread>
#include <vector>

void IteratorTest();

template<typename T>
using allocator_type = MyMallocator<T>;
template<typename T>
using vec_type = custom_std::vector < T, allocator_type<T> >;

int main()
{
	IteratorTest();
}

void IteratorTest()
{
	{
		struct X { int a; };
		vec_type<X> v(1);
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

	{
		vec_type<bool> v(1);
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
}
