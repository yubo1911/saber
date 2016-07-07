#include <iostream>
#include <vector>
#include <initializer_list>
#include <array>
#include <forward_list>

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
	
}
