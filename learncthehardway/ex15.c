#include <stdio.h>

int main(int argc, char *argv[])
{
	int ages[] = {10, 20, 15, 18, 23};
	char *names[] = {
		"Alan", "Frank",
		"John", "Lucy", "Tom"
	};

	int count = sizeof(ages) / sizeof(int);
	int i = 0;

	for (i=0; i<count; ++i)
	{
		printf("%s has %d years alive.\n",
				names[i], ages[i]);
	}

	printf("---\n");

	int *cur_age = ages;
	char **cur_name = names;

	for (i=0; i<count; ++i)
	{
		printf("%s is %d years old.\n", 
				*(cur_name + i), *(cur_age + i));
	}
	
	printf("---\n");
	
	for (i=0; i<count; ++i)
	{
		printf("%s is %d years old again.\n", 
				cur_name[i], cur_age[i]);
	}
	
	printf("---\n");

	for (cur_name=names, cur_age=ages; (cur_age - ages)<count; cur_name++, cur_age++)
	{
		printf("%s lived %d years so far.\n",
				*cur_name, *cur_age);
	}

	return 0;
}
