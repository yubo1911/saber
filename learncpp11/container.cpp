#include <iostream>
#include <vector>
#include <initializer_list>
#include <array>
#include <forward_list>
#include <utility>
#include <string>

using namespace std;

void print_list(initializer_list<int> il)
{
	for(auto it = il.begin(); it != il.end(); it++)
	{
		cout<<*it<<'\t';
		//*it = 100; // wrong. initializer_list element is read-only.
	}
	cout<<endl;
}

class TestData
{
	public:
		TestData(string name, int age, double salary): name(name), age(age), salary(salary)
		{}
	private:
		string name;
		int age;
		double salary;
};

int main()
{
	cout<<"Test cbegin cend:\n";
	vector<int> c1 = {0, 1, 2, 3, 4};
	auto it1_1 = c1.begin();
	auto it1_2 = c1.cbegin();
	*it1_1 = 4;
	//*it1_2 = 5; // wrong, const iterator's value can't be changed via this iterator.
	cout<<*it1_1<<'\t'<<*it1_2<<endl;

	cout<<"Test begin end:\n";
	int c2[] = {0, 1, 2, 3, 4};
	auto it2_1 = begin(c2);
	auto it2_2 = end(c2);
	while(it2_1 != it2_2)
	{
		cout<<*it2_1<<'\t';
		it2_1++;
	}
	cout<<endl;

	cout<<"Test {} assignment:\n";
	vector<int> c3;
	c3 = {0, 1, 2, 3, 4};
	for(auto it3_1 = c3.begin(); it3_1 != c3.end(); it3_1++)
		cout<<*it3_1<<'\t';
	cout<<endl;

	cout<<"Test initializer_list:\n";
	print_list({0, 1, 2, 3, 4});
	print_list({0, 1, 2, 3, 4, 5});

	cout<<"Test array:\n";
	array<int, 5> c4 = {0, 1, 2, 3, 4};
	c4[3] = 100; // can't insert since the array size is fixed.
	for(auto it4_1 = c4.begin(); it4_1 != c4.end(); it4_1++)
	{
		cout<<*it4_1<<'\t';
	}
	cout<<endl;
	
	cout<<"Test forward_list:\n";
	forward_list<int> c5 = {3, 4};
	c5.push_front(2);
	c5.push_front(1);
	auto it5_1 = c5.before_begin();
	c5.insert_after(it5_1, 0);
	for(auto it5_2 = c5.begin(); it5_2 != c5.end(); it5_2++)
	{
		cout<<*it5_2<<'\t';
	}
	cout<<endl;

	cout<<"Test swap:\n";
	vector<int> c6 = {0, 1, 2, 3, 4};
	vector<int> c7 = {5, 6, 7, 8, 9};
	auto it6_1 = c6.begin();
	auto it7_1 = c7.begin();
	swap(c6, c7);
	for(auto it6_2 = c6.begin(); it6_2 != c6.end(); it6_2++)
		cout<<*it6_2<<'\t';
	cout<<endl;
	
	for(auto it7_2 = c7.begin(); it7_2 != c7.end(); it7_2++)
		cout<<*it7_2<<'\t';
	cout<<endl;

	cout<<(it6_1 == c7.begin())<<'\t'<<(it7_1 == c6.begin())<<endl;

	array<int, 5> c8 = {0, 1, 2, 3, 4};
	array<int, 5> c9 = {5, 6, 7, 8, 9};
	auto it8_1 = c8.begin();
	auto it9_1 = c9.begin();
	swap(c8, c9);
	cout<<(it8_1 == c8.begin())<<'\t'<<(it9_1 == c9.begin())<<endl;
	
	cout<<"Test emplace:\n";
	vector<TestData> c10;
	c10.emplace_back("yubo", 26, 100000000000.0);
	//c10.push_back("laowang", 56, 10.5); // wrong. no 3 params push_back
	c10.push_back(TestData("laowang", 56, 10.5));
	cout<<c10.size()<<endl;
}
