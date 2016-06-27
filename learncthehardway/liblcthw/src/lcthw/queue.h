#ifndef lcthw_Queue_h
#define lcthw_Queue_h
#include <lcthw/list.h>

typedef struct Queue
{
	List *list;
} Queue;

Queue *Queue_create()
{
	Queue *queue = calloc(1, sizeof(Queue));
	queue->list = List_create();
	return queue;
}

void Queue_destroy(Queue *queue)
{
	List_destroy(queue->list);
	free(queue);
}

void Queue_send(Queue *queue, void *value)
{
	List_push(queue->list, value);
}

void *Queue_peek(Queue *queue)
{
	return List_last(queue->list);
}

void *Queue_recv(Queue *queue)
{
	return List_shift(queue->list);
}

#define Queue_count(A) (List_count(A->list))

#define QUEUE_FOREACH(Q, C) LIST_FOREACH((Q)->list, first, next, C)

#endif
