#include <olist.h>
#include <dbg.h>

static inline void OList_insert_helper(OList *list, OListNode *node, int coord);
static inline void OList_remove_helper(OList *list, OListNode *node, int coord);
static inline void OList_traverse_helper(OList *list, int coord, int reverse);
static inline void OList_move_helper(OList *list, OListNode *node, OListNode *prevnode[COORD_NUM], int coord);
static inline void OList_remove_without_free_helper(OList *list, OListNode *node, int coord);

OList *OList_create()
{
	OList *list = calloc(1, sizeof(OList));
	list->first[COORD_X] = list->last[COORD_X] = NULL;
	list->first[COORD_Y] = list->last[COORD_Y] = NULL;
	list->count = 0;
	return list;
}

void OList_destroy(OList *list)
{
	// Just need go through x direction
	LIST_FOREACH(list, first[COORD_X], next[COORD_X], cur)
	{
		if (cur->prev[COORD_X])
		{
			free(cur->prev[COORD_X]);
		}
	}

	free(list->last[COORD_X]);
	free(list);
}

void OList_clear(OList *list)
{
	// Just need go through x direction
	LIST_FOREACH(list, first[COORD_X], next[COORD_X], cur)
	{
		free(cur->value);
	}
}

void OList_clear_destroy(OList *list)
{
	OList_clear(list);
	OList_destroy(list);
}

void OList_insert(OList *list, OListNode *node)
{
	check(node, "node can't be NULL");
	check(list, "list can't be NULL");

	OList_insert_helper(list, node, COORD_X);
	OList_insert_helper(list, node, COORD_Y);
	list->count++;

error:
	return;

}

static inline void OList_insert_helper(OList *list, OListNode *node, int coord)
{
	if (list->first[coord] == NULL) // empty list
	{
		list->first[coord] = list->last[coord] = node;
		node->prev[coord] = node->next[coord] = NULL;
	}
	else if (node->pos[coord] < list->first[coord]->pos[coord]) // insert first
	{
		OListNode *oldfirst = list->first[coord];
		list->first[coord] = node;
		node->next[coord] = oldfirst;
		node->prev[coord] = NULL;
		oldfirst->prev[coord] = node;
	}
	else
	{
		LIST_FOREACH(list, first[coord], next[coord], cur)
		{
			if (cur->next[coord] == NULL) // insert last
			{
				cur->next[coord] = node;
				node->prev[coord] = cur;
				node->next[coord] = NULL;
				list->last[coord] = node;
				break;
			}
			else if (node->pos[coord] < cur->next[coord]->pos[coord]) // insert normally
			{
				OListNode *oldnext = cur->next[coord];
				cur->next[coord] = node;
				node->next[coord] = oldnext;
				oldnext->prev[coord] = node;
				node->prev[coord] = cur;
				break;
			}
		}
	}
	
}

void *OList_remove(OList *list, OListNode *node)
{
	void *result = NULL;

	check(list->first[COORD_X] && list->last[COORD_X], "List is empty.");
	check(node, "node can't be NULL");

	OList_remove_helper(list, node, COORD_X);
	OList_remove_helper(list, node, COORD_Y);
	
	list->count--;
	result = node->value;
	free(node);
error:
	return result;
}

static inline void OList_remove_helper(OList *list, OListNode *node, int coord)
{
	if (node == list->first[coord] && node == list->last[coord])
	{
		list->first[coord] = NULL;
		list->last[coord] = NULL;
	}
	else if (node == list->first[coord])
	{
		list->first[coord] = node->next[coord];
		check(list->first[coord] != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first[coord]->prev[coord] = NULL;
	}
	else if (node == list->last[coord])
	{
		list->last[coord] = node->prev[coord];
		check(list->last[coord] != NULL, "Invalid list, somehow got a last that is NULL.");
		list->last[coord]->next[coord] = NULL;
	}
	else
	{
		OListNode *after = node->next[coord];
		OListNode *before = node->prev[coord];
		after->prev[coord] = before;
		before->next[coord] = after;
	}

error:
	return;
}

void OList_roi(OList *list, OListNode *node, double rangex, double rangey, OListNode *roi[])
{
	check(node, "node can't be NULL");
	check(list, "list can't be NULL");
	check(rangex > 0 && rangey > 0, "range should > 0");
	check(roi, "result list can't be NULL");
	
	int index = 0;
	OListNode *left = node, *right = node, *up = node, *down = node;
	int outleft, outright, outup, outdown, stopleft, stopright, stopup, stopdown;
	outleft = outright = outup = outdown = 0;
	stopleft = stopright = stopup = stopdown = 0;
	while(index < MAX_ROI_NUM)
	{
		if(left == NULL && right == NULL && up == NULL && down == NULL) break;
		if(stopleft == 0 && left != NULL) outleft = OList_out_range(node, left, rangex, rangey, &stopleft, NULL);
		if(stopright == 0&& right != NULL) outright = OList_out_range(node, right, rangex, rangey, &stopright, NULL);
		if(stopup == 0 && up != NULL) outup = OList_out_range(node, up, rangex, rangey, NULL, &stopup);
		if(stopdown == 0 && down != NULL) outdown = OList_out_range(node, down, rangex, rangey, NULL, &stopdown);
		
		if(stopleft && stopright && stopup && stopdown) break;

		if(!outleft && !OList_has_add_to_roi(roi, index, left)) roi[index++] = left;
		if(index >= MAX_ROI_NUM) break;

		if(!outright && !OList_has_add_to_roi(roi, index, right)) roi[index++] = right;
		if(index >= MAX_ROI_NUM) break;

		if(!outup && !OList_has_add_to_roi(roi, index, up)) roi[index++] = up;
		if(index >= MAX_ROI_NUM) break;

		if(!outdown && !OList_has_add_to_roi(roi, index, down)) roi[index++] = down;
		if(index >= MAX_ROI_NUM) break;
		
		if(left) left = left->prev[COORD_X];
		if(right) right = right->next[COORD_X];
		if(up) up = up->prev[COORD_Y];
		if(down) down = down->next[COORD_Y];
	}

error:
	return;
}

int OList_out_range(OListNode *from, OListNode *to, double rangex, double rangey, int *stopx, int *stopy)
{
	if(!from || !to)
	{
		if(stopx) *stopx = 1;
		if(stopy) *stopy = 1;
		return 1;
	}
	
	int outx = abs(from->pos[COORD_X] - to->pos[COORD_X]) > rangex;
	int outy = abs(from->pos[COORD_Y] - to->pos[COORD_Y]) > rangey;
	if(stopx) *stopx = outx;
	if(stopy) *stopy = outy;
	return outx || outy;
}

int OList_has_add_to_roi(OListNode *roi[], int index, OListNode* node)
{
	int i = 0;
	for(i = 0; i < index; i++)
	{
		if(roi[i] == node) return 1;
	}
	return 0;
}

static inline void OList_traverse_helper(OList *list, int coord, int reverse)
{
	if(!reverse)
	{
		LIST_FOREACH(list, first[coord], next[coord], cur)
		{
			if (cur)
			{
				log_info("%s", (const char *)cur->value);
			}
		}
	}
	else
	{
		LIST_FOREACH(list, last[coord], prev[coord], cur)
		{
			if (cur)
			{
				log_info("%s", (const char *)cur->value);
			}
		}
	}
}

void OList_tranvers(OList *list)
{
	log_info("Tranverse olist in x:");
	OList_traverse_helper(list, COORD_X, 0);
	
	log_info("Tranverse olist in x reverse:");
	OList_traverse_helper(list, COORD_X, 1);
	
	log_info("Tranverse olist in y:");
	OList_traverse_helper(list, COORD_Y, 0);

	log_info("Tranverse olist in y reverse:");
	OList_traverse_helper(list, COORD_Y, 1);
}

void OList_move(OList *list, OListNode *node, double deltax, double deltay)
{
	check(node, "node can't be NULL");
	check(list, "list can't be NULL");
	
	node->pos[COORD_X] += deltax;
	node->pos[COORD_Y] += deltay;
	OListNode *prevnode[COORD_NUM] = {0};
	prevnode[COORD_X] = OList_find_place(list, node, COORD_X, deltax);
	prevnode[COORD_Y] = OList_find_place(list, node, COORD_Y, deltay);
	OList_move_helper(list, node, prevnode, COORD_X);
	OList_move_helper(list, node, prevnode, COORD_Y);
error:
	return;
}

static inline void OList_move_helper(OList *list, OListNode *node, OListNode *prevnode[COORD_NUM], int coord)
{
	if(prevnode[coord] != NULL)
	{
		OList_remove_without_free(list, node, coord==COORD_X, coord==COORD_Y);
		if(prevnode[coord] == list->last[coord])
		{
			OListNode *oldlast = list->last[coord];
			oldlast->next[coord] = node;
			node->prev[coord] = oldlast;
			node->next[coord] = NULL;
			list->last[coord] = node;
		}
		else
		{
			OListNode *nextnode = prevnode[coord]->next[coord];
			prevnode[coord]->next[coord] = node;
			node->prev[coord] = prevnode[coord];
			node->next[coord] = nextnode;
			nextnode->prev[coord] = node;
		}
	}
	else if(node->pos[coord] < list->first[coord]->pos[coord])
	{
		OList_remove_without_free(list, node, coord==COORD_X, coord==COORD_Y);
		OListNode *oldfirst = list->first[coord];
		list->first[coord] = node;
		node->next[coord] = oldfirst;
		node->prev[coord] = NULL;
		oldfirst->prev[coord] = node;
	}
}

OListNode *OList_find_place(OList *list, OListNode *node, int coord, double delta)
{
	check(coord == COORD_X || coord == COORD_Y, "Search direction must be x(0) or y(1).");

	OListNode *cur = node;
	if(delta > 0)
	{
		for(cur = node; cur != NULL; cur = cur->next[coord])
		{
			if(cur->pos[coord] > node->pos[coord])
			{
				return cur->prev[coord];
			}
		}
		if(node == list->last[coord]) return node->prev[coord];
		return list->last[coord];
	}
	else if (delta < 0)
	{
		for(cur = node; cur != NULL; cur = cur->prev[coord])
		{
			if(cur->pos[coord] < node->pos[coord])
			{
				return cur;
			}
		}
		return NULL;
	}
	else
	{
		return node->prev[coord];
	}

error:
	exit(-1);

}

void OList_remove_without_free(OList *list, OListNode *node, int removex, int removey)
{
	check(node, "node can't be NULL");
	if(removex)
	{
		OList_remove_without_free_helper(list, node, COORD_X);
	}

	if(removey)
	{
		OList_remove_without_free_helper(list, node, COORD_Y);
	}
error:
	return;
}

static inline void OList_remove_without_free_helper(OList *list, OListNode *node, int coord)
{
	check(list->first[coord] && list->last[coord], "List is empty.");
	
	if (node == list->first[coord] && node == list->last[coord])
	{
		list->first[coord] = NULL;
		list->last[coord] = NULL;
	}
	else if (node == list->first[coord])
	{
		list->first[coord] = node->next[coord];
		check(list->first[coord] != NULL, "Invalid list, somehow got a first that is NULL.");
		list->first[coord]->prev[coord] = NULL;
	}
	else if (node == list->last[coord])
	{
		list->last[coord] = node->prev[coord];
		check(list->last[coord] != NULL, "Invalid list, somehow got a last that is NULL.");
		list->last[coord]->next[coord] = NULL;
	}
	else
	{
		OListNode *after = node->next[coord];
		OListNode *before = node->prev[coord];
		after->prev[coord] = before;
		before->next[coord] = after;
	}

error:
	return;
}
