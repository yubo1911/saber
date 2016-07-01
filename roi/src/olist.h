#ifndef olist_h
#define olist_h

#include <stdlib.h>

struct OListNode;
#define MAX_ROI_NUM 30

#define COORD_NUM 2
#define COORD_X 0
#define COORD_Y 1

typedef struct OListNode 
{
	struct OListNode *next[COORD_NUM];
	struct OListNode *prev[COORD_NUM];
	double pos[COORD_NUM];
	void *value;
} OListNode;

typedef struct OList 
{
	int count;
	OListNode *first[COORD_NUM];
	OListNode *last[COORD_NUM];
} OList;

OList *OList_create();
void OList_destroy(OList *list);
void OList_clear(OList *list);
void OList_clear_destroy(OList *list);

#define OList_count(A) ((A)->count)
#define OList_first(A, N) ((A)->first[N] != NULL ? (A)->first[N] : NULL)
#define OList_last(A, N) ((A)->last[N] != NULL ? (A)->last[N] : NULL)

void OList_insert(OList *list, OListNode *node);
void *OList_remove(OList *list, OListNode *node);
void OList_remove_without_free(OList *list, OListNode *node, int removex, int removey);
void OList_roi(OList *list, OListNode *node, double rangex, double rangey, OListNode *roi[]);
int OList_out_range(OListNode *from, OListNode *to, double rangex, double rangey, int *stopx, int *stopy);
int OList_has_add_to_roi(OListNode *roi[], int index, OListNode* node);
void OList_tranvers(OList *list);
void OList_move(OList *list, OListNode *node, double deltax, double deltay);
OListNode *OList_find_place(OList *list, OListNode *node, int xy, double delta);

//void OList_push(List *list, void *value);
//void *OList_pop(List *list);

//void List_unshift(List *list, void *value);
//void *List_shift(List *list);

#define LIST_FOREACH(L, S, M, V) OListNode *_node = NULL;\
	OListNode *V = NULL;\
	for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
