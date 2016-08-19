#include <utility>
#include <iostream>

class IntVec
{
public:
	IntVec() = default;
	IntVec(size_t capacity);
	IntVec(IntVec &rhs);
	IntVec(IntVec &&rhs) noexcept;
	IntVec &operator=(IntVec &&rhs) noexcept;
	~IntVec();

	int push_back(int val);
	void print_info();

	int *pointer;
	size_t size;
	size_t capacity;
};

IntVec::IntVec(IntVec &rhs)
{
	this->capacity = rhs.capacity;
	this->size = rhs.size;
	this->pointer = new int[this->capacity];
	for(size_t i = 0; i < size; i++)
		this->pointer[i] = rhs.pointer[i];
	std::cout<<"IntVect copy constructor.\n";
}

IntVec::IntVec(size_t capacity)
	: capacity(capacity), size(0)
{
	this->pointer = new int[capacity];
}

IntVec::IntVec(IntVec &&rhs) noexcept
	: capacity(rhs.capacity), size(rhs.size), pointer(rhs.pointer)
{
		rhs.pointer = nullptr;
		rhs.capacity = rhs.size = 0;
		std::cout<<"IntVect move constructor.\n";
}

IntVec &IntVec::operator=(IntVec &&rhs) noexcept
{
	if(this != &rhs)
	{
		if(this->pointer)
			delete [] this->pointer;
		this->pointer = rhs.pointer;
		this->capacity = rhs.capacity;
		this->size = rhs.size;
		rhs.pointer = nullptr;
		rhs.capacity = rhs.size = 0;
	}
	std::cout<<"IntVect move assign constructor.\n";
	return *this;
}
IntVec::~IntVec()
{
	if(this->pointer)
		delete [] this->pointer;
}

int IntVec::push_back(int val)
{
	if(!this->pointer || this->capacity <= 0)
	{
		return -1;
	}
	if(this->size < capacity)
	{
		this->pointer[this->size] = val;
		this->size++;
		return 0;
	}

	return -1;
}

void IntVec::print_info()
{
	std::cout<<"capacity: "<<this->capacity<<std::endl;
	std::cout<<"size: "<<this->size<<std::endl;
	if(this->pointer != nullptr)
		std::cout<<"pointer: "<<this->pointer<<std::endl;
	else
		std::cout<<"pointer: nullptr"<<std::endl;
}

int main(int argc, char *argv[])
{
	std::cout<<"test move constructor:\n";
	std::allocator<std::string> alloc;
	size_t size = 5;
	auto old_strs = alloc.allocate(size);
	for(size_t i = 0; i < size; i++)
	{
		alloc.construct(old_strs + i, "abcde");
	}
	std::cout<<"old_strs[0]: "<<old_strs[0]<<std::endl;
	auto new_strs = alloc.allocate(size);
	for(size_t i = 0; i < size; i++)
	{
		alloc.construct(new_strs + i, std::move(*(old_strs + i)));
	}
	std::cout<<"new_strs[0]: "<<new_strs[0]<<std::endl;
	std::cout<<"old_strs[0]: "<<old_strs[0]<<std::endl;
	for(size_t i = 0; i < size; i++)
	{
		alloc.destroy(old_strs + i);
	}
	alloc.deallocate(old_strs, size);
	std::cout<<"test move constructor done.\n"<<std::endl;

	std::cout<<"test rvalue reference:\n";
	int j = 42;
	int &lr = j;
	//int &&rr = j; // Wrong. Can't bind a rvalue ref to a lvalue.
	//int &lr2 = i * 42; // Wrong. Can't bind a lvalue ref to a rvalue.
	const int &lr3 = j * 42;
	int &&rr2 = j * 42;
	//int &&rr3 = rr2; // Wrong. rr2 is a rvalue ref and rvalue ref is a lvalue.
	int &lr4 = rr2;
	std::cout<<j<<'\t'<<lr<<'\t'<<lr3<<'\t'<<rr2<<'\t'<<lr4<<std::endl;
	std::cout<<"test rvalue ref done.\n"<<std::endl;

	std::cout<<"test std::move:\n";
	std::string str5 = "asdf";
	std::string &lr5 = str5;
	std::string &&rr5 = std::move(str5);
	rr5[0] = 'b';
	lr5[1] = 'z';
	std::cout<<rr5<<'\t'<<lr5<<'\t'<<str5<<std::endl;
	std::cout<<"test std::move done.\n"<<std::endl;
	
	std::cout<<"test custom move copy constructor/move assign operator.\n";
	IntVec iv1(10);
	for(size_t i = 0; i < 5; i++)
		iv1.push_back(i);
	std::cout<<"-------iv1:\n";
	iv1.print_info();

	IntVec iv2(std::move(iv1));
	std::cout<<"-------iv2:\n";
	iv2.print_info();
	std::cout<<"-------iv1:\n";
	iv1.print_info();

	IntVec iv3 = iv2;
	std::cout<<"-------iv3:\n";
	iv3.print_info();
	std::cout<<"-------iv2:\n";
	iv2.print_info();

	IntVec iv4(5);
	std::cout<<"-------iv4:\n";
	iv4.print_info();
	iv4 = std::move(iv2);
	std::cout<<"-------iv4:\n";
	iv4.print_info();
	std::cout<<"-------iv2:\n";
	iv2.print_info();
}
