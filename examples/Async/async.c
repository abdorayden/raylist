
/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 *	RayList v2.2.0
 * */

#include <stdio.h>
#include <stdarg.h>

#define LIST_C
#include "../../raylist.h"

#define FUTURE_C
#include "future.h"

typedef struct {
    int count;
    int max_count;
} CounterData;

// implementation of abstract method poll
// NOTE : poll function should return HandlFuture structure
HandlFuture task_poll(void* task) {
	// getting the data from task
	CounterData* data = (CounterData*)((Future*)task)->data;

	// check if count is less than count_max
	if (data->count < data->max_count) {
		// do work (count++)
        	data->count++;
		// return NULL (function is not finished yet)
		return (HandlFuture){
			.isfinished = false,
			.iserror = false,
			.return_data = (void*)&data->count
		};
    	}
	// free allocated data memory
	free(data);
	// return task (task is not NULL so function is done)
	return (HandlFuture){
		.isfinished = true,
		.iserror = false,
		.return_data = NULL
	};
}

// for handle the return data
//void* printvalue(void* data , int task){
//	// if the function executed in task 3
//	if(task == 2){
//		if(*(int*)data >= 1000)
//			*(int*)data += 5;
//	}
//	printf("Debug :: %d\n" , *(int*)data);
//	return data;
//}

int main(void)
{
	/// init our object this queue has no limit buffer 
	RLCollections queue = Queue(Buf_Disable);

	// allocate the data
	CounterData* data1 = malloc(sizeof(CounterData));
	data1->count = 0;
	data1->max_count = 100;

	CounterData* data2 = malloc(sizeof(CounterData));
	data2->count = 200;
	data2->max_count = 400;

	CounterData* data3 = malloc(sizeof(CounterData));
	data3->count = 1000;
	data3->max_count = 1200;

	CounterData* data4 = malloc(sizeof(CounterData));
	data4->count = 10000;
	data4->max_count = 12000;

	CounterData* data5 = malloc(sizeof(CounterData));
	data5->count = -50;
	data5->max_count = 50;

	// allocate 5 tasks
	Future* task1 = FutureNewTask(task_poll,data1);

	Future* task2 = FutureNewTask(task_poll,data2);

	Future* task3 = FutureNewTask(task_poll,data3);

	Future* task4 = FutureNewTask(task_poll,data4);

	Future* task5 = FutureNewTask(task_poll,data5);

	// push tasks to the queue
	queue.Push(RL_VOIDPTR , task1);
	queue.Push(RL_VOIDPTR , task2);
	queue.Push(RL_VOIDPTR , task3);
	queue.Push(RL_VOIDPTR , task4);
	queue.Push(RL_VOIDPTR , task5);

	// execute tasks
	//FutureLoop(queue , printvalue);

	// testing lambda
	FutureLoop(queue , lambda(void* , (void* data , int task) , {
		// if the function executed in task 3
		if(task == 2){
			if(*(int*)data >= 1000)
				*(int*)data += 5;
		}
		printf("Debug :: %d\n" , *(int*)data);
		return data;
	}));
}
