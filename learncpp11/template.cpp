#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <utility>
#include <vector>

int add(int i, int j) { return i + j; }
struct divide
{
	int operator()(int i, int j)
	{
		return i / j;
	}
};

template<typename T> class Bar
{
friend T;
protected:
	int val = 100;
};

class Foo
{
public:
	void print_bar(Bar<Foo> &bar) {std::cout<<"bar:\t"<<bar.val<<std::endl;}
};
	
template<typename T> using twin = std::pair<T, T>;
template<typename T> using str_int = std::pair<T, int>;

template<typename T, typename F=std::less<T>>
int compare(const T &v1, const T &v2, F f=F())
{
	if(f(v1, v2)) return -1;
	if(f(v2, v1)) return 1;
	return 0;
}

template<typename It>
//decltype(*beg) get_begin(It beg)
auto get_begin(It beg) -> decltype(*beg)
{
	return *beg;
}

template<typename T>
std::ostream &print_variadic(std::ostream &os, const T &t)
{
	return os<<t<<std::endl;
}

template<typename T, typename... Args>
std::ostream &print_variadic(std::ostream &os, const T &t, const Args&... rest)
{
	os<<t<<"(remain size: "<<sizeof...(Args)<<"), ";
	return print_variadic(os, rest...);
}

int main()
{
	std::cout<<"test function<T>:\n";
	/* wrong. lambda and some other functions can't be represented by function ptr.
	std::map<std::string, int(*)(int, int)>> binops = {
		{"+", add},
		{"-", std::minus<int>()},
		{"*", [](int i, int j) {return i * j; }},
		{"/", divide()},
	};
	 */
	std::map<std::string, std::function<int(int, int)>> binops = {
		{"+", add},
		{"-", std::minus<int>()},
		{"*", [](int i, int j) {return i * j; }},
		{"/", divide()},
	};
	std::cout<<"+:\t"<<binops["+"](1, 2)<<"\n";
	std::cout<<"-:\t"<<binops["-"](1, 2)<<"\n";
	std::cout<<"*:\t"<<binops["*"](1, 2)<<"\n";
	std::cout<<"/:\t"<<binops["/"](1, 2)<<"\n";
	std::cout<<"test function<T> done.\n"<<std::endl;

	std::cout<<"test friend template type:\n";
	Bar<Foo> bar;
	Foo foo;
	foo.print_bar(bar);
	std::cout<<"test friend template type done.\n"<<std::endl;

	std::cout<<"test template alias:\n";
	twin<std::string> twin_str = {"abc", "def"};
	std::cout<<"twin_str:\t"<<twin_str.first<<'\t'<<twin_str.second<<std::endl;
	str_int<std::string> strno = {"abc", 100};
	std::cout<<"strno:\t"<<strno.first<<'\t'<<strno.second<<std::endl;
	std::cout<<"test template alias done.\n"<<std::endl;

	std::cout<<"test default template parameter:\n";
	std::cout<<"compare int 1 2:\t"<<compare(1, 2)<<std::endl;
	std::cout<<"compare int 2.0 1.0:\t"<<compare(2.0, 1.0)<<std::endl;
	//std::cout<<"compare int 2.0 1:\t"<<compare(2.0, 1)<<std::endl; // wrong. can't determine which type is T
	std::cout<<"test default template parameter done.\n"<<std::endl;

	std::cout<<"test tail return type of template:\n";
	std::vector<int> numbers = {1, 2, 3, 4, 5};
	std::cout<<"get_begin:\t"<<get_begin(numbers.begin())<<std::endl;
	std::cout<<"test tail return type of template done.\n";

	std::cout<<"test variadic template:\n";
	print_variadic(std::cout, 100, "s", 56.76, 101);
	std::cout<<"test variadic template done.\n";
}
