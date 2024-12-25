
/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 *	RayList v2.2.0
 * */

#include <stdio.h>
#include <stdarg.h>

#define LIST_C
#include "../../raylist.h"

// this structure used to handle return data
// it contains bool isfinished to check if the task finish
// and bool is error to handle error
// return_data to work with
typedef struct {
	bool 	isfinished;
	bool 	iserror;
	void* 	return_data;
}HandlPromise;

// define function type for target function
typedef HandlPromise 	(*Poll)(void*);
// this callback function called in PromiseLoop to work with return_data
typedef void 		(*DoWorkInPromiseLoop)(void*);

// unit task
typedef struct Promise {
	// Abstract method , should be implemented
	Poll poll;
	// the data
	void* data;
}Promise;


// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|

Promise* PromiseNewTask(Poll poll , void* data){
	Promise* promise = malloc(sizeof(Promise));
	promise->poll = poll;
	promise->data = data;
	return promise;
}

void PromiseLoop(RLCollections queue , DoWorkInPromiseLoop callback) {
	// while queue is not empty
	while (!queue.Is_Empty()) {
		// pop the unit task
		Promise* curr = (Promise*)queue.Pop();

		HandlPromise handle = curr->poll((void*)curr);
		// call function and check if function finished or not
		if (!handle.isfinished) {
			// function is not finished 
			// we pushed back to the queue
			if(callback != NULL){
				// handle the data with callback function
				callback(handle.return_data);
			}
			if(handle.iserror){
				// check errno or log error
			}
			queue.Push(RL_VOIDPTR , curr);
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

// implementation of abstract method poll
// NOTE : poll function should return HandlPromise structure
HandlPromise counter_task_execute(void* task) {
	// getting the data from task
	CounterData* data = (CounterData*)((Promise*)task)->data;

	// check if count is less than count_max
	if (data->count < data->max_count) {
		// do work (count++)
        	data->count++;
		// return NULL (function is not finished yet)
		return (HandlPromise){
			.isfinished = false,
			.iserror = false,
			.return_data = (void*)&data->count
		};
    	}
	// free allocated data memory
	free(data);
	// return task (task is not NULL so function is done)
	return (HandlPromise){
		.isfinished = true,
		.iserror = false,
		.return_data = NULL
	};
}

// for handle the return data
void printvalue(void* data){
	printf("Debug :: %d\n" , *(int*)data);
}

int main(void)
{
	/// init our object this queue has no limit buffer 
	RLCollections queue = Queue(Buf_Disable);

	// allocate data1 and data2
	CounterData* data1 = malloc(sizeof(CounterData));
	data1->count = 0;
	data1->max_count = 100;

	CounterData* data2 = malloc(sizeof(CounterData));
	data2->count = 200;
	data2->max_count = 400;

	// allocate tasks 1 and 2
	Promise* task1 = PromiseNewTask(counter_task_execute,data1);
	//task1->poll = counter_task_execute;
	//task1->data = data1;

	Promise* task2 = PromiseNewTask(counter_task_execute,data2);
	//task2->poll = counter_task_execute;
	//task2->data = data2;

	// push tasks to the queue
	queue.Push(RL_VOIDPTR , task1);
	queue.Push(RL_VOIDPTR , task2);

	// execute tasks
	PromiseLoop(queue , printvalue);
}
