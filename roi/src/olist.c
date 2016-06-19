#include <olist.h>
#include <dbg.h>

OList *OList_create()
{
	OList *list = calloc(1, sizeof(OList));
	list->xfirst = list->xlast = NULL;
	list->yfirst = list->ylast = NULL;
	list->count = 0;
	return list;
}

void OList_destroy(OList *list)
{
	// Just need go through x direction
	LIST_FOREACH(list, xfirst, xnext, cur)
	{
		if (cur->xprev)
		{
			free(cur->xprev);
		}
	}

	free(list->xlast);
	free(list);
}

void OList_clear(OList *list)
{
	// Just need go through x direction
	LIST_FOREACH(list, xfirst, xnext, cur)
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
	
	// x direction
	if (list->xfirst == NULL) // empty list
	{
		list->xfirst = list->xlast = node;
		node->xprev = node->xnext = NULL;
	}
	else if (node->x < list->xfirst->x) // insert first
	{
		OListNode *oldfirst = list->xfirst;
		list->xfirst = node;
		node->xnext = oldfirst;
		node->xprev = NULL;
		oldfirst->xprev = node;
	}
	else
	{
		LIST_FOREACH(list, xfirst, xnext, cur)
		{
			if (cur->xnext == NULL) // insert last
			{
				cur->xnext = node;
				node->xprev = cur;
				node->xnext = NULL;
				list->xlast = node;
				break;
			}
			else if (node->x < cur->xnext->x) // insert normally
			{
				OListNode *oldnext = cur->xnext;
				cur->xnext = node;
				node->xnext = oldnext;
				oldnext->xprev = node;
				node->xprev = cur;
				break;
			}
		}
	}
	
	// y direction
	if (list->yfirst == NULL) // empty list
	{
		list->yfirst = list->ylast = node;
		node->ynext = node->yprev = NULL;
	}
	else if (node->y < list->yfirst->y) // insert first
	{
		OListNode *oldfirst = list->yfirst;
		list->yfirst = node;
		node->ynext = oldfirst;
		node->yprev = NULL;
		oldfirst->yprev = node;
	}
	else
	{
		LIST_FOREACH(list, yfirst, ynext, cur)
		{
			if (cur->ynext == NULL) // insert last
			{
				cur->ynext = node;
				node->yprev = cur;
				node->ynext = NULL;
				list->ylast = node;
				break;
			}
			else if (node->y < cur->ynext->y) // insert normally
			{
				OListNode *oldnext = cur->ynext;
				cur->ynext = node;
				node->ynext = oldnext;
				oldnext->yprev = node;
				node->yprev = cur;
				break;
			}
		}
	}

	list->count++;

error:
	return;
}
void *OList_remove(OList *list, OListNode *node)
{
	void *result = NULL;

	check(list->xfirst && list->xlast, "List is empty.");
	check(node, "node can't be NULL");
	
	// x direction
	if (node == list->xfirst && node == list->xlast)
	{
		list->xfirst = NULL;
		list->xlast = NULL;
	}
	else if (node == list->xfirst)
	{
		list->xfirst = node->xnext;
		check(list->xfirst != NULL, "Invalid list, somehow got a first that is NULL.");
		list->xfirst->xprev = NULL;
	}
	else if (node == list->xlast)
	{
		list->xlast = node->xprev;
		check(list->xlast != NULL, "Invalid list, somehow got a last that is NULL.");
		list->xlast->xnext = NULL;
	}
	else
	{
		OListNode *after = node->xnext;
		OListNode *before = node->xprev;
		after->xprev = before;
		before->xnext = after;
	}

	// y direction
	if (node == list->yfirst && node == list->ylast)
	{
		list->yfirst = NULL;
		list->ylast = NULL;
	}
	else if (node == list->yfirst)
	{
		list->yfirst = node->ynext;
		check(list->yfirst != NULL, "Invalid list, somehow got a first that is NULL.");
		list->yfirst->yprev = NULL;
	}
	else if (node == list->ylast)
	{
		list->ylast = node->yprev;
		check(list->ylast != NULL, "Invalid list, somehow got a last that is NULL.");
		list->ylast->ynext = NULL;
	}
	else
	{
		OListNode *after = node->ynext;
		OListNode *before = node->yprev;
		after->yprev = before;
		before->ynext = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;
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
		
		if(left) left = left->xprev;
		if(right) right = right->xnext;
		if(up) up = up->yprev;
		if(down) down = down->ynext;
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
	
	int outx = abs(from->x - to->x) > rangex;
	int outy = abs(from->y - to->y) > rangey;
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

void OList_tranvers(OList *list)
{
	{
		log_info("Tranverse olist in x:");
		LIST_FOREACH(list, xfirst, xnext, curx)
		{
			if (curx)
			{
				log_info("%s", (const char *)curx->value);
			}
		}
	}
	{
		log_info("Tranverse olist in x reverse:");
		LIST_FOREACH(list, xlast, xprev, curx)
		{
			if (curx)
			{
				log_info("%s", (const char *)curx->value);
			}
		}
	}
	{
		log_info("Tranverse olist in y:");
		LIST_FOREACH(list, yfirst, ynext, cury)
		{
			if (cury)
			{
				log_info("%s", (const char *)cury->value);
			}
		}
	}
	{
		log_info("Tranverse olist in y reverse:");
		LIST_FOREACH(list, ylast, yprev, cury)
		{
			if (cury)
			{
				log_info("%s", (const char *)cury->value);
			}
		}
	}
}

void OList_move(OList *list, OListNode *node, double deltax, double deltay)
{
	check(node, "node can't be NULL");
	check(list, "list can't be NULL");
	
	node->x += deltax;
	node->y += deltay;

	OListNode *prevxnode = OList_find_place(list, node, 1, deltax);
	OListNode *prevynode = OList_find_place(list, node, 2, deltay);

	if(prevxnode != NULL)
	{
		OList_remove_without_free(list, node, 1, 0);
		if(prevxnode == list->xlast)
		{
			OListNode *oldlast = list->xlast;
			oldlast->xnext = node;
			node->xprev = oldlast;
			node->xnext = NULL;
			list->xlast = node;
		}
		else
		{
			OListNode *nextnode = prevxnode->xnext;
			prevxnode->xnext = node;
			node->xprev = prevxnode;
			node->xnext = nextnode;
			nextnode->xprev = node;
		}
	}
	else if(node->x < list->xfirst->x)
	{
		OList_remove_without_free(list, node, 1, 0);
		OListNode *oldfirst = list->xfirst;
		list->xfirst = node;
		node->xnext = oldfirst;
		node->xprev = NULL;
		oldfirst->xprev = node;
	}
	
	if(prevynode != NULL)
	{
		log_info("prevynode %s", (const char *)prevynode->value);
		log_info("ylast %s", (const char *)list->ylast->value);
		OList_remove_without_free(list, node, 0, 1);
		if(prevynode == list->ylast)
		{
			OListNode *oldlast = list->ylast;
			oldlast->ynext = node;
			node->yprev = oldlast;
			node->ynext = NULL;
			list->ylast = node;
		}
		else
		{
			OListNode *nextnode = prevynode->ynext;
			prevynode->ynext = node;
			node->yprev = prevynode;
			node->ynext = nextnode;
			nextnode->yprev = node;
		}
	}
	else if(node->y < list->yfirst->y)
	{
		OList_remove_without_free(list, node, 0, 1);
		OListNode *oldfirst = list->yfirst;
		list->yfirst = node;
		node->ynext = oldfirst;
		node->yprev = NULL;
		oldfirst->yprev = node;
	}
error:
	return;
}

OListNode *OList_find_place(OList *list, OListNode *node, int xy, double delta)
{
	check(xy == 1 || xy == 2, "Search direction must be x(1) or y(2).");
	if(xy == 1)
	{
		OListNode *cur = node;
		if(delta > 0)
		{
			for(cur = node; cur != NULL; cur = cur->xnext)
			{
				if(cur->x > node->x)
				{
					return cur->xprev;
				}
			}
			if(node == list->xlast) return node->xprev;
			return list->xlast;
		}
		else if (delta < 0)
		{
			for(cur = node; cur != NULL; cur = cur->xprev)
			{
				if(cur->x < node->x)
				{
					return cur;
				}
			}
			return NULL;
		}
		else
		{
			return node->xprev;
		}
	}
	else if(xy == 2)
	{
		OListNode *cur = node;
		if(delta > 0)
		{
			for(cur = node; cur != NULL; cur = cur->ynext)
			{
				if(cur->y > node->y)
				{
					return cur->yprev;
				}
			}
			if(node == list->ylast) return node->yprev;
			return list->ylast;
		}
		else if (delta < 0)
		{
			for(cur = node; cur != NULL; cur = cur->yprev)
			{
				if(cur->y < node->y)
				{
					return cur;
				}
			}
			return NULL;
		}
		else
		{
			return node->yprev;
		}
	}

error:
	exit(-1);

}

void OList_remove_without_free(OList *list, OListNode *node, int removex, int removey)
{
	check(node, "node can't be NULL");
	if(removex)
	{
		check(list->xfirst && list->xlast, "List is empty.");
		// x direction
		if (node == list->xfirst && node == list->xlast)
		{
			list->xfirst = NULL;
			list->xlast = NULL;
		}
		else if (node == list->xfirst)
		{
			list->xfirst = node->xnext;
			check(list->xfirst != NULL, "Invalid list, somehow got a first that is NULL.");
			list->xfirst->xprev = NULL;
		}
		else if (node == list->xlast)
		{
			list->xlast = node->xprev;
			check(list->xlast != NULL, "Invalid list, somehow got a last that is NULL.");
			list->xlast->xnext = NULL;
		}
		else
		{
			OListNode *after = node->xnext;
			OListNode *before = node->xprev;
			after->xprev = before;
			before->xnext = after;
		}
	}

	if(removey)
	{
		check(list->yfirst && list->ylast, "List is empty.");
		// y direction
		if (node == list->yfirst && node == list->ylast)
		{
			list->yfirst = NULL;
			list->ylast = NULL;
		}
		else if (node == list->yfirst)
		{
			list->yfirst = node->ynext;
			check(list->yfirst != NULL, "Invalid list, somehow got a first that is NULL.");
			list->yfirst->yprev = NULL;
		}
		else if (node == list->ylast)
		{
			list->ylast = node->yprev;
			check(list->ylast != NULL, "Invalid list, somehow got a last that is NULL.");
			list->ylast->ynext = NULL;
		}
		else
		{
			OListNode *after = node->ynext;
			OListNode *before = node->yprev;
			after->yprev = before;
			before->ynext = after;
		}
	}
error:
	return;
}
