#include <iostream>
#include <fstream>

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
}
