#include <iostream>
#include <memory>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	shared_ptr<string> p1 = make_shared<string>("");
	if(p1 && p1->empty())
		*p1 = "hello";

	auto p2 = make_shared<string>("world");
	cout<<*p1<<' '<<*p2<<endl;
	cout<<p1.use_count()<<' '<<p2.use_count()<<endl;
	
	auto p3 = p2;
	p2 = p1;
	cout<<p1.use_count()<<' '<<p2.use_count()<<' '<<p3.use_count()<<endl;

}


