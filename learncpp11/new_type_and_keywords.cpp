#include <iostream>
using namespace std;

int out_i = 0;
constexpr int sz() { return 42; }
constexpr int new_sz(int cnt) { return sz() * cnt; }

// noexcept
void no_except() noexcept
{
	throw 1;
}

void no_except2() noexcept(noexcept(no_except()))
{}

int main()
{
	// long long
	long large = 1 << 31;
	long long long_large = 1 << 31;
	long long long_long_large = 1 << 63;
	cout<<"test long long: "<<large<<'\t'<<long_large<<'\t'<<long_long_large<<endl;

	// nullptr
	int *p1 = nullptr;
	int *p2 = 0;
	int *p3 = NULL;
	cout<<"test nullptr: "<<(p1 == p2)<<'\t'<<(p1 == p3)<<endl;

	// constexpr
	constexpr int mf = 20;
	constexpr int limit = mf + 1;
	constexpr int *p4 = &out_i;
	// the following would cause a make error 
	
	// since large is not a constexpr
	//constexpr int wrong = large + 1; 
	
	// since &in_j is not a constexpr;
	//int in_j = 0;
	//constexpr int *p5 = &in_j;

	// the following two expression is not the same!
	constexpr int *p6 = nullptr; // a const pointer point to an int
	// p6 = &out_i; // error: p6 is a constexpr
	const int *p7 = nullptr; // a pointer point to a const int

	constexpr int size = sz();
	constexpr int nsize = new_sz(mf);
	//constexpr int wrong_size = new_sz(out_i); // error: out_i is not a constexpr
	cout<<"test constexpr: "<<mf<<'\t'<<limit<<'\t'<<p4<<'\t'<<size<<'\t'<<nsize<<'\t'<<p6<<'\t'<<p7<<endl;
	
	// noexcept
	// the following call will cause terminate
	//no_except();
	cout<<"test noexcept: "<<noexcept(no_except())<<'\t'<<noexcept(no_except2())<<endl;
	return 0;
}
