#include <lcthw/bstrlib.h>
#include <lcthw/hashmap.h>
#include <lcthw/hashmap_algos.h>
#include <lcthw/darray.h>
#include "minunit.h"

struct tagbstring test1 = bsStatic("test data 1");
struct tagbstring test2 = bsStatic("test data 2");
struct tagbstring test3 = bsStatic("test data 3");

char *test_fnvla()
{
	uint32_t hash = Hashmap_fnvla_hash(&test1);
	mu_assert(hash != 0, "Bash hash.");

	hash = Hashmap_fnvla_hash(&test2);
	mu_assert(hash != 0, "Bash hash.");
	
	hash = Hashmap_fnvla_hash(&test3);
	mu_assert(hash != 0, "Bash hash.");

	return NULL;
}

char *test_adler32()
{
	uint32_t hash = Hashmap_adler32_hash(&test1);
	mu_assert(hash != 0, "Bad hash.");
	
	hash = Hashmap_adler32_hash(&test2);
	mu_assert(hash != 0, "Bad hash.");
	
	hash = Hashmap_adler32_hash(&test3);
	mu_assert(hash != 0, "Bad hash.");

	return NULL;
}

char *test_djb()
{
	uint32_t hash = Hashmap_djb_hash(&test1);
	mu_assert(hash != 0, "Bad hash.");
	
	hash = Hashmap_djb_hash(&test2);
	mu_assert(hash != 0, "Bad hash.");
	
	hash = Hashmap_djb_hash(&test3);
	mu_assert(hash != 0, "Bad hash.");

	return NULL;
}

#define BUCKETS 100
#define BUFFER_LEN 20
#define NUM_KEYS BUCKETS * 1000
enum { ALGO_FNVLA, ALGO_ADLER32, ALGO_DJB};

int gen_keys(DArray *keys, int num_keys)
{
	int i = 0;
	FILE *urand = fopen("/dev/urandom", "r");
	check(urand != NULL, "Failed to open /dev/urandom");
}
