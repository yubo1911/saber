#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <boost/interprocess/managed_shared_memory.hpp>

int main(void)
{
	using namespace boost::interprocess;

	struct shm_remove
	{
		shm_remove() { shared_memory_object::remove("MyMemory");}
		~shm_remove(){ shared_memory_object::remove("MyMemory");}
	} remover;
	
	managed_shared_memory segment(create_only, "MyMemory", 65535);
	std::string name;
	for(int i=0; i<100; ++i)
	{
		char cname[16];
		snprintf(cname, 16, "child%d", i);
		name = cname;
		segment.construct<std::string>(cname)(name);
	}
	pid_t fpid;
	fpid = fork();
	if(fpid < 0)
	{
		printf("error in fork...");
	}
	else if(fpid == 0)
	{
		// child
		managed_shared_memory segment(open_only, "MyMemory");
		for(int i=0; i<100; ++i)
		{
			char cname[16];
			snprintf(cname, 16, "child%d", i);
			std::string *name = segment.find<std::string>(cname).first;
			printf("child found in shared memory %s\n", name->c_str());
		}
	}
	sleep(1);
}
