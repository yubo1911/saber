#include <iostream>

using namespace std;

class TestClass
{
public:
	TestClass() = default;
	TestClass(const int i, const char c): member_i(i), member_c(c) {}
	TestClass(const int i): TestClass(i, 0) { member_c = 'T';}
	TestClass(const TestClass&) = default;
	//TestClass(const TestClass&) = delete;
	TestClass& operator=(const TestClass&);
	
	virtual void print_msg() {cout<<member_i<<'\t'<<member_c<<endl;}
	virtual void final_foo() final {}

	int member_i = 0;
	char member_c = '\0';
};


class SubTestClass final: public TestClass
{
	public:
		using TestClass::TestClass;
		SubTestClass(const int i): TestClass(i, 'S') {}
		void print_msg() override;
		//void print_msg(char c) override;
		//‘void SubTestClass::print_msg(char)’ marked override, but does not override
		
		//void final_foo() {}
		//overriding final function ‘virtual void TestClass::final_foo()’
};

//class SubSubTestClass: public SubTestClass {}; 
// cannot derive from ‘final’ base ‘SubTestClass’ in derived type ‘SubSubTestClass’

void SubTestClass::print_msg() 
{
	cout<<"i: "<<member_i<<'\t'<<"c: "<<member_c<<endl;
}

TestClass& TestClass::operator=(const TestClass&) = default;

class TestClass2
{
public:
	TestClass2() = default;
	TestClass2(const int i) {cout<<"construct with i: "<<i<<endl;}
};

class MultiSubClass: public TestClass, public TestClass2
{
public:
	using TestClass::TestClass;
	using TestClass2::TestClass2;
	// conflicts with version inherited from ‘TestClass’
	MultiSubClass(const int i): TestClass(i) {}
	MultiSubClass() = default;
};

int main()
{
	cout<<"test sizeof class member:\n";
	cout<<sizeof(TestClass::member_i)<<"\t"<<sizeof(TestClass::member_c)<<endl;

	cout<<"test =default class construct:\n";
	TestClass tc; // may cause error if no default construct.
	cout<<tc.member_i<<'\t'<<(short)tc.member_c<<endl;
	cout<<"test =default done."<<endl;

	cout<<"test delegating constructor:\n";
	TestClass tc2(2);
	cout<<tc2.member_i<<'\t'<<tc2.member_c<<endl;
	
	cout<<"test allocator:\n";
	allocator<TestClass> alloc;
	auto p = alloc.allocate(10);
	alloc.construct(p, 10);
	cout<<p->member_i<<'\t'<<p->member_c<<endl;

	cout<<"test =default class copy construct:\n";
	TestClass tc3(tc2);
	TestClass tc4 = tc2;
	cout<<tc3.member_i<<'\t'<<tc3.member_c<<endl;
	cout<<tc4.member_i<<'\t'<<tc4.member_c<<endl;
	
	cout<<"test =delete class copy construct:\n";
	//TestClass tc5(tc2); // error: use of deleted function ‘TestClass::TestClass(const TestClass&)’
	//cout<<tc5.member_i<<'\t'<<tc5.member_c<<endl;
	cout<<"test =delete done."<<endl;

	cout<<"test override:\n";
	TestClass *stc_ptr = new SubTestClass(10); 
	stc_ptr->print_msg();
	SubTestClass stc(10);
	TestClass tc6 = (TestClass)stc;
	tc6.print_msg();

	cout<<"test inherit base class contructor:\n";
	SubTestClass stc2(1024, 'H');
	stc2.print_msg();

	cout<<"test multi inherit constructor:\n";
	MultiSubClass mtc(1024);
	mtc.print_msg();
	return 0;
}

