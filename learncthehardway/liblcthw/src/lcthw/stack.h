#ifndef lcthw_Stack_h
#define lcthw_Stack_h
#include <lcthw/list.h>

typedef struct Stack
{
	List *list;
} Stack;

Stack *Stack_create()
{
	Stack *stack = calloc(1, sizeof(Stack));
	stack->list = List_create();
	return stack;
}

void Stack_destroy(Stack *stack)
{
	List_destroy(stack->list);
	free(stack);
}

void Stack_push(Stack *stack, void *value)
{
	List_push(stack->list, value);
}

void *Stack_peek(Stack *stack)
{
	return List_last(stack->list);
}

void *Stack_pop(Stack *stack)
{
	return List_pop(stack->list);
}

#define Stack_count(A) (List_count(A->list))

#define STACK_FOREACH(S, C) LIST_FOREACH((S)->list, first, next, C)

#endif
