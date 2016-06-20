#include "minunit.h"
#include <lcthw/string_algos.h>
#include <lcthw/bstrlib.h>
#include <time.h>

struct tagbstring IN_STR = bsStatic("I have ALPHA beta ALPHA and oranges ALPHA");
struct tagbstring ALPHA = bsStatic("ALPHA");
const int TEST_TIME = 1;

char *test_find_and_scan()
{
	StringScanner *scan = StringScanner_create(&IN_STR);
	mu_assert(scan != NULL, "Failed to create scanner.");

	int find_i = String_find(&IN_STR, &ALPHA);
	mu_assert(find_i > 0, "Failed to find 'ALPHA' in test string.");

	int scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > 0, "Failed to find 'ALPHA' with scan.");
	mu_assert(scan_i == find_i, "find and scan don't match.");

	scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > find_i, "Should find another ALPHA with scan.");

	scan_i = StringScanner_scan(scan, &ALPHA);
	mu_assert(scan_i > find_i, "Should find another ALPHA with scan.");

	mu_assert(StringScanner_scan(scan, &ALPHA) == -1, "Shouldn't find it.");

	StringScanner_destroy(scan);

	return NULL; 
}

char *test_binstr_performance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);

	do
	{
		for(i = 0; i <  1000; i++)
		{
			found_at = binstr(&IN_STR, 0, &ALPHA);
			mu_assert(found_at != BSTR_ERR, "Failed to find!");
			find_count++;
		}

		elapsed = time(NULL) - start;
	} while(elapsed < TEST_TIME);

	debug("BINSTR COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);

	return NULL;
}

char *test_find_performance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	time_t start = time(NULL);

	do
	{
		for(i = 0; i < 1000; i++)
		{
			found_at = String_find(&IN_STR, &ALPHA);
			find_count++;
		}
		elapsed = time(NULL) - start;
	} while(elapsed < TEST_TIME);

	debug("FIND COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);

	return NULL;
}

char *test_scan_performance()
{
	int i = 0;
	int found_at = 0;
	unsigned long find_count = 0;
	time_t elapsed = 0;
	StringScanner *scan = StringScanner_create(&IN_STR);
	time_t start = time(NULL);

	do
	{
		for(i = 0; i < 1000; i++)
		{
			found_at = 0;

			do
			{
				found_at = StringScanner_scan(scan, &ALPHA);
				find_count++;
			} while(found_at != -1);
		}
		elapsed = time(NULL) - start;
	} while(elapsed < TEST_TIME);

	debug("SCAN COUNT: %lu, END TIME: %d, OPS: %f",
			find_count, (int)elapsed, (double)find_count / elapsed);

	return NULL;
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_find_and_scan);
#if 1
	mu_run_test(test_binstr_performance);
	mu_run_test(test_find_performance);
	mu_run_test(test_scan_performance);
#endif

	return NULL;
}

RUN_TESTS(all_tests);
