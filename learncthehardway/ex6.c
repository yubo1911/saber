#include <stdio.h>

int main(int argc, char *argv[])
{
	int distance = 100;
	float power = 2.345f;
	double super_power = 56789.4532;
	char initial = 'A';
	char first_name[] = "Zed";
	char last_name[] = "Shaw";

	printf("You are %.5d miles away.\n", distance);
	printf("You have %.6f levels of power.\n", power);
	printf("You have %.3f awesome super powers.\n", super_power);
	printf("I have an initial %10c.\n", initial);
	printf("I have a first name %10s.\n", first_name);
	printf("I have a last name %10s.\n", last_name);
	printf("My whole name is %s.%c. %s.\n", first_name, initial, last_name);

	return 0;
}
