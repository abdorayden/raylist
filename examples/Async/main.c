
/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 * */

#include <stdio.h>
#include <stdarg.h>

#define LIST_C
#include "../../raylist.h"

// define function type for target function
typedef void* (*Routine)(void*);

// unit task
typedef struct UniT {
	Routine routine;
	void* data;
} UniT;

// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|

void exec_loop(IfaceList queue) {
	// while queue is not empty
	while (!queue.Queue_Is_Empty()) {
		// pop the unit task
		UniT* curr = (UniT*)queue.Queue_Pop();
		// call function and check if function finished or not
		if (!curr->routine((void*)curr)) {
			// function is not finished 
			// we pushed back to the queue
			queue.Queue_Push(RL_VOIDPTR , curr);
		} else {
			// function is done 
			// free the allocated data curr and skip
			free(curr);
		}
	}
}

//////////////////////////
/// Example stoled from copilot
/////////////////////////

typedef struct {
    int count;
    int max_count;
} CounterData;

void* counter_task_execute(void* task) {
	// getting the data from task
	CounterData* data = (CounterData*)((UniT*)task)->data;

	// check if count is less than count_max
	if (data->count < data->max_count) {
		// do work (count++)
        	data->count++;
		// debugging 
		printf("DEBUG :: %d\n" , data->count);
		// return NULL (function is not finished yet)
		return NULL;
    	}
	// free allocated data memory
	free(data);
	// return task (task is not NULL so function is done)
	return task;
}

int main(void)
{
	/// init our object this queue has no limit buffer 
	IfaceList queue = Queue(Buf_Disable);

	// allocate data1 and data2
	CounterData* data1 = malloc(sizeof(CounterData));
	data1->count = 0;
	data1->max_count = 100;

	CounterData* data2 = malloc(sizeof(CounterData));
	data2->count = 200;
	data2->max_count = 400;

	// allocate tasks 1 and 2
	UniT* task1 = malloc(sizeof(UniT));
	task1->routine = counter_task_execute;
	task1->data = data1;

	UniT* task2 = malloc(sizeof(UniT));
	task2->routine = counter_task_execute;
	task2->data = data2;

	// push tasks to the queue
	queue.Queue_Push(RL_VOIDPTR , task1);
	queue.Queue_Push(RL_VOIDPTR , task2);

	// execute tasks
	exec_loop(queue);
}
