#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <utility>
using namespace std;

// class member initialization
class InitClass
{
	public:
		void print_class()
		{
			cout<<field1<<'\t'<<field2<<'\t'<<field3<<'\t'<<field4<<endl;
		}
	private:
		int field1 = 1;
		int field2;
		double field3 = 1.0;
		double field4;
};

// return value using list initialization
vector<string> error_msg(int typ)
{
	switch(typ)
	{
		case 1:
			return {"type1", "msg1"};
		case 2:
			return {"type2", "msg2"};
		default:
			return {};
	}
}

// return pair using list initialization
pair<string, string> get_pair(int typ)
{
	if(typ == 1) return {"key", "value"};
	return pair<string, string>("default key", "default value");
}

// class member(of class type) initialization
class InitClassMgr
{
	public:
		vector<InitClass> init_objs = {InitClass()};
};

int main()
{
	// simple list initialization
	int single_int1 = 0;
	int single_int2 = {0};
	cout<<"test list initialization:\n"<<single_int1<<'\t'<<single_int2<<endl;

	// class member initialization
	InitClass test_class;
	cout<<"test class member initialization:\n";
	test_class.print_class();

	// vector/list list initialization
	vector<string> v1 = {"ab", "cd", "ef"};
	list<string> l2{"gh", "ij", "kl"};
	//vector<string> v3("mn", "op", "qr"); // wrong initialization format
	cout<<"test vector/list list initialization:\n"<<v1[1]<<'\t'<<l2.front()<<endl;

	// map/set list initialization
	map<string, string> m1 =
	{
		{"a", "A"},
		{"b", "B"},
		{"c", "C"}
	};
	m1.insert({"d", "D"});
	set<string> s1 = {"a", "b", "c"};
	cout<<"test map/set list initialization:\n"<<m1["d"]<<'\t'<<*s1.find("b")<<endl;

	// return value using list initialization
	vector<string> err_msg1 = error_msg(1);
	vector<string> err_msg2 = error_msg(2);
	vector<string> err_msg3 = error_msg(3);
	cout<<"test return value list initialization:\n"<<err_msg1[1]<<'\t'<<err_msg2[1]<<'\t'<<err_msg3.empty()<<endl;
	
	// return pair using list initialization
	pair<string, string> p1 = get_pair(1);
	pair<string, string> p2 = get_pair(2);
	cout<<"test return pair list initialization:\n"<<p1.first<<'\t'<<p2.first<<endl;

	// class member(of class type) initialization
	InitClassMgr mgr;
	cout<<"test class member of class type initialization:\n";
	mgr.init_objs[0].print_class();

	// new allocator using list initialization
	vector<int> *pv = new vector<int>{0, 1, 2, 3, 4};
	int *pi = new int[5]{0, 1, 2, 3, 4};
	cout<<"test new alloator using list initialization:\n"<<(*pv)[2]<<'\t'<<pi[2]<<endl;
	
	return 0;
}
