#include <iostream>
#include <fstream>
#include <functional>

int (*dummy_ret1(int i))[5]
{
	static int ret1[5] = {i, i*2, i*3, i*4, i*5};
	int(*pret)[5] = &ret1;
	return pret;
}

auto dummy_ret2(int i) -> int (*)[5]
{
	static int ret2[5] = {i+1, i+2, i+3, i+4, i+5};
	int(*pret)[5] = &ret2;
	return pret;
}

int add_int(int a, int b)
{
	return a + b;
}

class OtherType
{
public:
	OtherType(int i) : val(i){}
	explicit operator int() {return val;}
	explicit operator bool() {return val != 0;}
private:
	int val;
};

int main()
{
	std::cout<<"test using alias:\n";
	using HT = double;
	using NAME = std::string;
	HT h = 1.78;
	NAME name = "Robert";
	std::cout<<name<<"'s height is "<<h<<std::endl;
	std::cout<<"test using alias done.\n"<<std::endl;

	std::cout<<"test range for:\n";
	std::string nation = "CHINA";
	for(auto c : nation)
		std::cout<<c<<" ";
	std::cout<<"\n";
	for(auto &rc : nation)
		rc = tolower(rc);
	std::cout<<"lower: "<<nation<<std::endl;
	std::cout<<"test range for done.\n";
	
	std::cout<<"test divide:\n";
	std::cout<<"10 / 3 = "<<(10 / 3)<<"\n";
	std::cout<<"-10 / 3 = "<<(-10 / 3)<<"\n";
	std::cout<<"-10 / -3 = "<<(-10 / (-3))<<"\n";
	std::cout<<"10 % 3 = "<<(10 % 3)<<"\n";
	std::cout<<"-10 % 3 = "<<(-10 % 3)<<"\n";
	std::cout<<"-10 % -3 = "<<(-10 % (-3))<<"\n";
	std::cout<<"10 % -3 = "<<(10 % (-3))<<"\n";
	std::cout<<"test divide done.\n"<<std::endl;

	std::cout<<"test trailing return type:\n";
	int (*arr1)[5] = dummy_ret1(1);
	std::cout<<(*arr1)[0]<<'\t'<<(*arr1)[4]<<std::endl;
	int (*arr2)[5] = dummy_ret2(2);
	std::cout<<(*arr2)[0]<<'\t'<<(*arr2)[4]<<std::endl;
	std::cout<<"test trailing return type done.\n";

	std::cout<<"test string filename:\n";
	std::string filename = "regex.cpp";
	std::ifstream in(filename);
	std::string head_ctx;
	in>>head_ctx;
	std::cout<<head_ctx<<std::endl;
	std::cout<<"test string filename done.\n"<<std::endl;

	std::cout<<"test str number cvt:\n";
	int age = 15;
	double weight = 137.5;
	std::string str_age = std::to_string(age);
	std::string str_weight = std::to_string(weight);
	std::cout<<"str age: "<<str_age<<"\tstr weight: "<<str_weight<<std::endl;
	int int_age = std::stoi(str_age);
	double d_weight = std::stod(str_weight);
	std::cout<<"int age: "<<int_age<<"\tdouble weight: "<<d_weight<<std::endl;
	std::cout<<"test str number cvt done.\n"<<std::endl;

	std::cout<<"test bind:\n";
	auto add5 = std::bind(add_int, std::placeholders::_1, 5);
	std::cout<<"add5(6): "<<add5(6)<<std::endl;
	std::cout<<"test bind done.\n"<<std::endl;

	std::cout<<"test range for of dynamic array:\n";
	int *darr = new int[5]{0, 1, 2, 3, 4};
	// wrong. dynamic array don't has a begin method,
	// so we can't use range for here.
	//for(auto i : darr) 	
	//	std::cout<<i<<'\t';
	for(size_t i = 0; i < 5; i++)
		std::cout<<darr[i]<<'\t';
	std::cout<<'\n';
	std::cout<<"test range for of dynamic array done.\n";

	std::cout<<"test explicit type cvt:\n";
	OtherType c(10);
	//int i = c + 10; // wrong. can't implicit cvt c to int.
	int j = static_cast<int>(c) + 10;
	std::cout<<"OtherType(10) + 10: "<<j<<"\n";
	if(c)
	{
		std::cout<<"OtherType can be cvt to bool implicitly in if clause.\n";
	}
	std::cout<<"test explicit type cvt done.\n"<<std::endl;

}
