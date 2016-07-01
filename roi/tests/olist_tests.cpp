#include "minunit.h"
#include <olist.h>
#include <assert.h>
#include <set>

using std::set;

static OList *list = NULL;
const char *test1 = "test1 data";
const char *test2 = "test2 data";
const char *test3 = "test3 data";
const char *test4 = "test4 data";
const char *test5 = "test5 data";
OListNode *node1 = NULL;
OListNode *node2 = NULL;
OListNode *node3 = NULL;
OListNode *node4 = NULL;
OListNode *node5 = NULL;

const char *test_create()
{
	list = OList_create();
	mu_assert(list != NULL, "Failed to create list.");

	log_info("Passed test create.");
	return NULL;
}

const char *test_destroy()
{
	OList_clear_destroy(list);

	log_info("Passed test destroy.");
	return NULL;
}

const char *test_insert()
{
	node1 = (OListNode *)malloc(sizeof(OListNode));
	node1->pos[COORD_X] = 0.0;
	node1->pos[COORD_Y] = 0.0;
	node1->value = strdup(test1);
	node1->next[COORD_X] = node1->prev[COORD_X] = node1->next[COORD_Y] = node1->prev[COORD_Y] = NULL;

	node2 = (OListNode *)malloc(sizeof(OListNode));
	node2->pos[COORD_X] = 1.0;
	node2->pos[COORD_Y] = 1.0;
	node2->value = strdup(test2);
	node2->next[COORD_X] = node2->prev[COORD_X] = node2->next[COORD_Y] = node2->prev[COORD_Y] = NULL;
	
	node3 = (OListNode *)malloc(sizeof(OListNode));
	node3->pos[COORD_X] = -1.0;
	node3->pos[COORD_Y] = 2.0;
	node3->value = strdup(test3);
	node3->next[COORD_X] = node3->prev[COORD_X] = node3->next[COORD_Y] = node3->prev[COORD_Y] = NULL;
	
	node4 = (OListNode *)malloc(sizeof(OListNode));
	node4->pos[COORD_X] = 3.0;
	node4->pos[COORD_Y] = -1.0;
	node4->value = strdup(test4);
	node4->next[COORD_X] = node4->prev[COORD_X] = node4->next[COORD_Y] = node4->prev[COORD_Y] = NULL;
	
	node5 = (OListNode *)malloc(sizeof(OListNode));
	node5->pos[COORD_X] = -5.0;
	node5->pos[COORD_Y] = -5.0;
	node5->value = strdup(test5);
	node5->next[COORD_X] = node5->prev[COORD_X] = node5->next[COORD_Y] = node5->prev[COORD_Y] = NULL;

	OList_insert(list, node1);
	mu_assert(OList_count(list) == 1, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node1, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node1, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node1, "Failed on ylast.");
	
	OList_insert(list, node2);
	mu_assert(OList_count(list) == 2, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node2, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node1, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node2, "Failed on ylast.");
	
	OList_insert(list, node3);
	mu_assert(OList_count(list) == 3, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node3, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node2, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node1, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");
	
	OList_insert(list, node4);
	mu_assert(OList_count(list) == 4, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node3, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");
	
	OList_insert(list, node5);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node5, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node5, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");

	log_info("Passed test insert.");

	return NULL;
}

const char *test_remove()
{
	OList_remove(list, node1);
	node1 = NULL;
	mu_assert(OList_count(list) == 4, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node5, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node5, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");

	OList_remove(list, node5);
	node5 = NULL;
	mu_assert(OList_count(list) == 3, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node3, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");
	
	OList_remove(list, node3);
	node3 = NULL;
	mu_assert(OList_count(list) == 2, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node2, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node2, "Failed on ylast.");
	
	OList_remove(list, node2);
	node2 = NULL;
	mu_assert(OList_count(list) == 1, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node4, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node4, "Failed on ylast.");
	
	OList_remove(list, node4);
	node4 = NULL;
	mu_assert(OList_count(list) == 0, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == NULL, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == NULL, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == NULL, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == NULL, "Failed on ylast.");

	log_info("Passed test remove.");
	return NULL;
}

const char *test_roi()
{
	test_insert();
	std::set<OListNode*> roi1;
	OList_roi(list, node1, 1.5, 1.5, roi1);
	mu_assert(OList_has_add_to_roi(roi1, node1), "node1 should be added to the roi of node1");
	mu_assert(OList_has_add_to_roi(roi1, node2), "node2 should be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi1, node3), "node3 should not be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi1, node4), "node4 should not be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi1, node5), "node5 should not be added to the roi of node1");
	
	std::set<OListNode*> roi2;
	OList_roi(list, node5, 7, 6, roi2);
	mu_assert(OList_has_add_to_roi(roi2, node1), "node1 should be added to the roi of node5");
	mu_assert(OList_has_add_to_roi(roi2, node2), "node2 should be added to the roi of node5");
	mu_assert(!OList_has_add_to_roi(roi2, node3), "node3 should not be added to the roi of node5");
	mu_assert(!OList_has_add_to_roi(roi2, node4), "node4 should not be added to the roi of node5");
	mu_assert(OList_has_add_to_roi(roi2, node5), "node5 should be added to the roi of node5");
	
	log_info("Passed test roi.");
	return NULL;
}

const char *test_move()
{
	log_info("Move node1.");
	OList_move(list, node1, -6.0, 0.0);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node5, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");

	log_info("Move node5.");
	OList_move(list, node5, 0.0, 5.0);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");

	log_info("Move node2.");
	OList_move(list, node2, -1.0, -1.0);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");

	log_info("Move node3.");
	OList_move(list, node3, -4.5, 1.0);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node4, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node4, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node3, "Failed on ylast.");
	
	log_info("Move node4.");
	OList_move(list, node4, -4.5, 10.0);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_first(list, COORD_X) == node1, "Failed on xfirst.");
	mu_assert(OList_last(list, COORD_X) == node2, "Failed on xlast.");
	mu_assert(OList_first(list, COORD_Y) == node2, "Failed on yfirst.");
	mu_assert(OList_last(list, COORD_Y) == node4, "Failed on ylast.");

	return NULL;
}

const char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_insert);
	mu_run_test(test_remove);
	mu_run_test(test_roi);
	mu_run_test(test_move);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
