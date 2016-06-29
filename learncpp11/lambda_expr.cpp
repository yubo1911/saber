#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	// lambda: [capture list] (parameter list) -> return type { function body }
	cout<<"Test simple lambda f1:\n";
	auto f1 = [] { return 1; };
	auto f2 = [] () { return 2; };
	cout<<f1()<<'\t'<<f2()<<endl;

	cout<<"Test lambda with capture and parameter:\n";
	int test_data[] = {1, 5, 9, 7, 3, 19, 13, 17};
	int border = 8;
	auto f3 = [border](const int &i){ if(i > border) cout<<i<<'\t'; };
	for_each(begin(test_data), end(test_data), f3);
	cout<<endl;

	cout<<"Test lambda with capture reference:\n";
	auto f4 = [&border](const int &i){ if(i > border) cout<<i<<'\t'; };
	border = 6;
	for_each(begin(test_data), end(test_data), f4);
	cout<<endl;
	
	cout<<"Test lambda with implicit capture:\n";
	char space = ' ';
	auto f5 = [=](const int &i){ if(i > border) cout<<i<<'\t'; };
	auto f6 = [&](const int &i){ if(i > border) cout<<i<<'\t'; };
	auto f7 = [&, space](const int &i){ if(i > border) cout<<i<<space; };
	border = 0;
	for_each(begin(test_data), end(test_data), f5);
	cout<<endl;
	for_each(begin(test_data), end(test_data), f6);
	cout<<endl;
	for_each(begin(test_data), end(test_data), f7);
	cout<<endl;

	cout<<"Test lambda with implicit capture:\n";
	auto f8 = [&, space](const int &i) mutable { if(i > border) {cout<<i<<space; space='\t';} };
	for_each(begin(test_data), end(test_data), f8);
	cout<<endl;
	cout<<1<<space<<2<<endl;

	cout<<"Test lambda with return type:\n";
	auto f9 = [](const int i){if(i % 3) return i * 3; else return i;};
	transform(begin(test_data), end(test_data), begin(test_data), f9);
	border = 0;
	for_each(begin(test_data), end(test_data), f6);
	cout<<endl;
	auto f10 = [](const int i) -> double
	{if(i % 5) return i * 5.0; else return i;};
	transform(begin(test_data), end(test_data), begin(test_data), f10);
	for_each(begin(test_data), end(test_data), f6);
	cout<<endl;
}
