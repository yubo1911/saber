#include "minunit.h"
#include <lcthw/ringbuffer.h>
#include <assert.h>

static RingBuffer *buffer = NULL;

char *tests[] = {"test1 data", "test2 data", "test3 data"};

#define BUFFER_LENGTH 100
#define NUM_TESTS 3

char *test_create()
{
	buffer = RingBuffer_create(BUFFER_LENGTH);
	mu_assert(buffer != NULL, "Failed to create ringbuffer.");

	return NULL;
}

char *test_read_write()
{
	int i = 0;
	int wr_data_cnt = 0;
	for(i = 0; i < NUM_TESTS; i++)
	{
		int res = RingBuffer_write(buffer, tests[i], strlen(tests[i]));
		mu_assert(res >= 0, "Failed to write.");
		wr_data_cnt += strlen(tests[i]);
		mu_assert(!RingBuffer_empty(buffer), "Should not be empty.");
		mu_assert(RingBuffer_available_data(buffer) == wr_data_cnt, "Write size wrong.");
	}

	for(i = 0; i < NUM_TESTS; i++)
	{
		mu_assert(!RingBuffer_empty(buffer), "Should not be empty.");
		int cur_size = strlen(tests[i]);
		char *read_target = calloc(1, cur_size);
		int res = RingBuffer_read(buffer, read_target, cur_size);
		mu_assert(res >= 0, "Failed to read.");
		int j = 0;
		for(j = 0; j < cur_size; j++)
		{
			mu_assert(tests[i][j] == read_target[j], "Wrong read result.");
		}
	}
	
	mu_assert(RingBuffer_empty(buffer), "Should be empty.");

	return NULL;
}

char *test_destroy()
{
	mu_assert(buffer != NULL, "Failed to make ringbuffer #2.");
	RingBuffer_destroy(buffer);

	return NULL;
}


char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_read_write);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
