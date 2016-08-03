#include <iostream>
#include <memory>
#include <string>

using namespace std;

void process(shared_ptr<int> ptr)
{
	cout<<"in process use_count:"<<ptr.use_count()<<endl;
}

void print_at_delete(int *p)
{
	cout<<"deleting..."<<p<<'\t'<<*p<<endl;
	delete p;
}

unique_ptr<int> clone(int p)
{
	return unique_ptr<int>(new int(p));
}

void process_unique_ptr(unique_ptr<int> up)
{
	cout<<"process unique ptr: "<<*up<<endl;
}

int main(int argc, char *argv[])
{
	cout<<"test shared_ptr base usage:"<<endl;
	shared_ptr<string> p1 = make_shared<string>("");
	if(p1 && p1->empty())
		*p1 = "hello";

	auto p2 = make_shared<string>("world");
	cout<<*p1<<' '<<*p2<<endl;

	cout<<"test shared_ptr use_count:"<<endl;
	cout<<"p1 cnt:"<<p1.use_count()<<"\tp2 cnt:"<<p2.use_count()<<endl;
	
	auto p3 = p2;
	cout<<"p1 cnt:"<<p1.use_count()<<"\tp2 cnt:"<<p2.use_count()<<"\tp3 cnt:"<<p3.use_count()<<endl;
	p2 = p1;
	cout<<"p1 cnt:"<<p1.use_count()<<"\tp2 cnt:"<<p2.use_count()<<"\tp3 cnt:"<<p3.use_count()<<endl;

	cout<<"test shared_ptr and new:"<<endl;
	shared_ptr<int> p4(new int(1024));
	//shared_ptr<int> p5 = new int(1024); // wrong, no implicit constructor
	cout<<*p4<<endl;

	cout<<"don't mix shared_ptr and normal pointer:"<<endl;
	shared_ptr<int> p5(new int(1024));
	process(p5);
	int v5 = *p5;
	cout<<"v5: "<<v5<<endl;

	int *p6 = new int(1024);
	process(shared_ptr<int>(p6));
	int v6 = *p6;
	cout<<"v6: "<<v6<<endl;

	cout<<"test shared_ptr reset:"<<endl;
	cout<<"p1 cnt:"<<p1.use_count()<<"\tp2 cnt:"<<p2.use_count()<<"\tp3 cnt:"<<p3.use_count()<<endl;
	p1.reset(new string("cpp11"));
	cout<<"p1 cnt:"<<p1.use_count()<<"\tp2 cnt:"<<p2.use_count()<<"\tp3 cnt:"<<p3.use_count()<<endl;
	
	cout<<"test shared_ptr deleter:"<<endl;
	int *p7 = new int(1024);
	shared_ptr<int> p8(p7, print_at_delete);
	p8 = make_shared<int>(1025);

	cout<<"test unique_ptr base usage:"<<endl;
	unique_ptr<int> up1(new int(1024));
	cout<<"up1: "<<*up1<<endl;
	unique_ptr<int> up2(up1.release());
	cout<<"up2: "<<*up2<<endl;
	//unique_ptr<int> up3(up1); // wrong, unique_ptr can not copy
	//up2 = up1; // wrong, unique_ptr can not copy
	unique_ptr<int> up4(new int(1025));
	up4.reset(up2.release());
	cout<<"up4: "<<*up4<<endl;

	cout<<"test unique_ptr parameter and return value:"<<endl;
	auto up5 = clone(1024);
	cout<<"up5: "<<*up5<<endl;
	process_unique_ptr(move(up5));
	//cout<<"up5 after process: "<<*up5<<endl; // would cause segmentfault

	cout<<"test unique_ptr deleter:"<<endl;
	int *p9 = new int(1024);
	unique_ptr<int, decltype(print_at_delete) *> up6(p9, print_at_delete);
	unique_ptr<int> up7(new int(1025));
	up6.reset(up7.release());

	cout<<"test weak_ptr basic usage:"<<endl;
	auto p10 = make_shared<int>(1024);
	weak_ptr<int> wp1(p10);
	cout<<"p10 use_count: "<<p10.use_count()<<endl;
	//p10.reset(new int(1025)); // this will cause wp1.lock() return a false obj
	shared_ptr<int> p11 = wp1.lock();
	if(p11) cout<<"wp1: "<<*p11<<" use count: "<<p11.use_count()<<endl;
}


