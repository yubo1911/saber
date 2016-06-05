#include "minunit.h"
#include <olist.h>
#include <assert.h>

static OList *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";
char *test4 = "test4 data";
char *test5 = "test5 data";
OListNode *node1 = NULL;
OListNode *node2 = NULL;
OListNode *node3 = NULL;
OListNode *node4 = NULL;
OListNode *node5 = NULL;

char *test_create()
{
	list = OList_create();
	mu_assert(list != NULL, "Failed to create list.");

	log_info("Passed test create.");
	return NULL;
}

char *test_destroy()
{
	OList_clear_destroy(list);

	log_info("Passed test destroy.");
	return NULL;
}

char *test_insert()
{
	node1 = malloc(sizeof(OListNode));
	node1->x = 0.0;
	node1->y = 0.0;
	node1->value = strdup(test1);
	node1->xnext = node1->xprev = node1->ynext = node1->yprev = NULL;

	node2 = malloc(sizeof(OListNode));
	node2->x = 1.0;
	node2->y = 1.0;
	node2->value = strdup(test2);
	node2->xnext = node2->xprev = node2->ynext = node2->yprev = NULL;
	
	node3 = malloc(sizeof(OListNode));
	node3->x = -1.0;
	node3->y = 2.0;
	node3->value = strdup(test3);
	node3->xnext = node3->xprev = node3->ynext = node3->yprev = NULL;
	
	node4 = malloc(sizeof(OListNode));
	node4->x = 3.0;
	node4->y = -1.0;
	node4->value = strdup(test4);
	node4->xnext = node4->xprev = node4->ynext = node4->yprev = NULL;
	
	node5 = malloc(sizeof(OListNode));
	node5->x = -5.0;
	node5->y = -5.0;
	node5->value = strdup(test5);
	node5->xnext = node5->xprev = node5->ynext = node5->yprev = NULL;

	OList_insert(list, node1);
	mu_assert(OList_count(list) == 1, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node1, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node1, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node1, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node1, "Failed on ylast.");
	
	OList_insert(list, node2);
	mu_assert(OList_count(list) == 2, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node1, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node2, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node1, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node2, "Failed on ylast.");
	
	OList_insert(list, node3);
	mu_assert(OList_count(list) == 3, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node3, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node2, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node1, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node3, "Failed on ylast.");
	
	OList_insert(list, node4);
	mu_assert(OList_count(list) == 4, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node3, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node4, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node3, "Failed on ylast.");
	
	OList_insert(list, node5);
	mu_assert(OList_count(list) == 5, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node5, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node5, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node3, "Failed on ylast.");

	log_info("Passed test insert.");

	return NULL;
}

char *test_remove()
{
	OList_remove(list, node1);
	node1 = NULL;
	mu_assert(OList_count(list) == 4, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node5, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node5, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node3, "Failed on ylast.");

	OList_remove(list, node5);
	node5 = NULL;
	mu_assert(OList_count(list) == 3, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node3, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node4, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node3, "Failed on ylast.");
	
	OList_remove(list, node3);
	node3 = NULL;
	mu_assert(OList_count(list) == 2, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node2, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node4, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node2, "Failed on ylast.");
	
	OList_remove(list, node2);
	node2 = NULL;
	mu_assert(OList_count(list) == 1, "Failed on list count.");
	mu_assert(OList_xfirst(list) == node4, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == node4, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == node4, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == node4, "Failed on ylast.");
	
	OList_remove(list, node4);
	node4 = NULL;
	mu_assert(OList_count(list) == 0, "Failed on list count.");
	mu_assert(OList_xfirst(list) == NULL, "Failed on xfirst.");
	mu_assert(OList_xlast(list) == NULL, "Failed on xlast.");
	mu_assert(OList_yfirst(list) == NULL, "Failed on yfirst.");
	mu_assert(OList_ylast(list) == NULL, "Failed on ylast.");

	log_info("Passed test remove.");
	return NULL;
}

char *test_roi()
{
	test_insert();
	OListNode *roi[MAX_ROI_NUM] = {0};
	OList_roi(list, node1, 1.5, 1.5, roi);
	mu_assert(OList_has_add_to_roi(roi, MAX_ROI_NUM, node1), "node1 should be added to the roi of node1");
	mu_assert(OList_has_add_to_roi(roi, MAX_ROI_NUM, node2), "node2 should be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi, MAX_ROI_NUM, node3), "node3 should not be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi, MAX_ROI_NUM, node4), "node4 should not be added to the roi of node1");
	mu_assert(!OList_has_add_to_roi(roi, MAX_ROI_NUM, node5), "node5 should not be added to the roi of node1");
	
	OListNode *roi2[MAX_ROI_NUM] = {0};
	OList_roi(list, node5, 7, 6, roi2);
	mu_assert(OList_has_add_to_roi(roi2, MAX_ROI_NUM, node1), "node1 should be added to the roi of node5");
	mu_assert(OList_has_add_to_roi(roi2, MAX_ROI_NUM, node2), "node2 should be added to the roi of node5");
	mu_assert(!OList_has_add_to_roi(roi2, MAX_ROI_NUM, node3), "node3 should not be added to the roi of node5");
	mu_assert(!OList_has_add_to_roi(roi2, MAX_ROI_NUM, node4), "node4 should not be added to the roi of node5");
	mu_assert(OList_has_add_to_roi(roi2, MAX_ROI_NUM, node5), "node5 should be added to the roi of node5");
	
	log_info("Passed test roi.");
	return NULL;
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_create);
	mu_run_test(test_insert);
	mu_run_test(test_remove);
	mu_run_test(test_roi);
	mu_run_test(test_destroy);

	return NULL;
}

RUN_TESTS(all_tests);
