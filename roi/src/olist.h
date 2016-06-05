#ifndef olist_h
#define olist_h

#include <stdlib.h>

struct OListNode;
#define MAX_ROI_NUM 30

typedef struct OListNode 
{
	struct OListNode *xnext;
	struct OListNode *xprev;
	struct OListNode *ynext;
	struct OListNode *yprev;
	double x, y;
	void *value;
} OListNode;

typedef struct OList 
{
	int count;
	OListNode *xfirst;
	OListNode *xlast;
	OListNode *yfirst;
	OListNode *ylast;
} OList;

OList *OList_create();
void OList_destroy(OList *list);
void OList_clear(OList *list);
void OList_clear_destroy(OList *list);

#define OList_count(A) ((A)->count)
#define OList_xfirst(A) ((A)->xfirst != NULL ? (A)->xfirst : NULL)
#define OList_xlast(A) ((A)->xlast != NULL ? (A)->xlast : NULL)
#define OList_yfirst(A) ((A)->yfirst != NULL ? (A)->yfirst : NULL)
#define OList_ylast(A) ((A)->ylast != NULL ? (A)->ylast : NULL)

void OList_insert(OList *list, OListNode *node);
void *OList_remove(OList *list, OListNode *node);
void OList_roi(OList *list, OListNode *node, double rangex, double rangey, OListNode *roi[]);
int OList_out_range(OListNode *from, OListNode *to, double rangex, double rangey, int *stopx, int *stopy);
int OList_has_add_to_roi(OListNode *roi[], int index, OListNode* node);
void OList_tranvers(OList *list);

//void OList_push(List *list, void *value);
//void *OList_pop(List *list);

//void List_unshift(List *list, void *value);
//void *List_shift(List *list);

#define LIST_FOREACH(L, S, M, V) OListNode *_node = NULL;\
	OListNode *V = NULL;\
	for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
