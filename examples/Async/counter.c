/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 *	RayList v2.3.0
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
HandlFuture task_poll(void* dta) {
	// getting the data from task
	CounterData* data = (CounterData*)dta;

	// check if count is less than count_max
	if (data->count < data->max_count) {
		// do work (count++)
        	data->count++;
		// return NULL (function is not finished yet)
		return (HandlFuture){
			.isfinished = false,
			.iserror = NULL,
			.return_data = (void*)&data->count
		};
    	}
	// free allocated data memory
	//free(data);
	// return task (task is not NULL so function is done)
	return (HandlFuture){
		.isfinished = true,
		.iserror = NULL,
		.return_data = NULL
	};
}

int main(void)
{
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
	Future* task[5];
	task[0] = FutureNewTask(task_poll,data1,Low);
	task[1] = FutureNewTask(task_poll,data2,Low);
	task[2] = FutureNewTask(task_poll,data3,Low);
	task[3] = FutureNewTask(task_poll,data4,Low);
	task[4] = FutureNewTask(task_poll,data5,Low);

	FutureAddTasks(task,5);

	// logging callback 
	FutureLoop(OnData({
		// if the function executed in task 3
		if(futuretask == 2){
			if(*(int*)futuredata >= 1000)
				*(int*)futuredata += 5;
		}else if(futuretask == 3){
			*(int*)futuredata += 5;
		}
		printf("Debug :: %d\n" , *(int*)futuredata);
	}) , NULL);
}
