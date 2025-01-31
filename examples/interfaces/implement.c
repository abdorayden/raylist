/*
 *	Test if RayList is flexible enough to allow developers to implement the RLCollections interface for their stack or queue. 
 */


#include <stdio.h>

#include "../../raylist.h"

#define MAX_BUFFER	20

typedef struct {
	int table[MAX_BUFFER];
	int pos;
}stack_r;

stack_r stack = {0};

void push(Type t , void* d)
{
	RLUNUSED(t);
	int value = *(int*)d;
	stack.table[stack.pos++] = value;
}

void* pop(void)
{
	if(stack.pos > 0){
		stack.pos -= 1;
	}
	return (void*)&stack.table[stack.pos];
}
void* peek(void){
	if(stack.pos > 0)
		return (void*)&stack.table[stack.pos - 1];
	else 
		return (void*)&stack.table[stack.pos];
}

RLCollections my_stack() {
	RLCollections ret = {0};
	ret.Push = RLAmbda(void , (Type t , void* d) , {
		RLUNUSED(t);
		int value = *(int*)d;
		stack.table[stack.pos++] = value;
	});
	ret.Pop = pop;
	ret.Peek = peek;
	return ret;
}

int main(void){
	int wahren = 31;
	int idk = 51;
	RLCollections ms = my_stack();
	ms.Push(0 , (void*)&wahren);
	ms.Push(0 , (void*)&idk);
	printf("Hello : %d\n" , *(int*)ms.Peek());
	printf("Hello : %d\n" , *(int*)ms.Pop());
	printf("Hello : %d\n" , *(int*)ms.Peek());
	printf("Hello : %d\n" , *(int*)ms.Pop());
	printf("Hello : %d\n" , *(int*)ms.Peek());
}
