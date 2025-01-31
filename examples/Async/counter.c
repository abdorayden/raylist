/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 *	RayList v2.3.0
 * */

#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define LIST_C
#include "../../raylist.h"

#define FUTURE_C
#include "future.h"

typedef struct {
    int count;
    int max_count;
} CounterData;

typedef struct {
    char first;
    char last;
} CounterChar;

HandlFuture task_poll_char(void* dta) {
	CounterChar* dc = (CounterChar*)dta;
	if(dc->first < dc->last)
	{
        	dc->first++;
		return notyet((void*)&dc->first);
	}
	return done(NULL);
}

// implementation of abstract method poll
// NOTE : poll function should return HandlFuture structure
HandlFuture task_poll(void* dta) {
	// getting the data from task
	CounterData* data = (CounterData*)dta;

	if (data->count < data->max_count) {
        	data->count++;
		return notyet((void*)&data->count);
    	}
	return done(NULL);
}

int main(void)
{
	// allocate the data
	CounterData* data1 = malloc(sizeof(CounterData));
	data1->count = 0;
	data1->max_count = 100;

	CounterData* data2 = malloc(sizeof(CounterData));
	data2->count = 150;
	data2->max_count = 200;

	CounterChar* data3 = malloc(sizeof(CounterChar));
	data3->first = 'a';
	data3->last = 'm';

	CounterChar* data4 = malloc(sizeof(CounterChar));
	data4->first = 'm';
	data4->last = 'z';

	// allocate 2 tasks
	Future* task[4];
	task[0] = FutureNewTask(task_poll,data1,Low);
	task[1] = FutureNewTask(task_poll,data2,Low);
	task[2] = FutureNewTask(task_poll_char,data3,Low);
	task[3] = FutureNewTask(task_poll_char,data4,Low);

	FutureAddTasksGroup(task,4);

	// logging callback 
	// 
	FutureLoop(OnData({
		// if the function executed in task 3
		if(futuretask == 0 || futuretask == 1)
			printf("Debug :: %d\n" , *(int*)futuredata);
		else
			printf("DEBUG :: char : %c\n",*(char*)futuredata);
	}) , OnErr({
		fprintf(stderr , "ERROR : task (%d) : %s\n" , futuretask , futurerror);
		fprintf(stderr , "killing task %d\n" , futuretask);
		return KillTask;
	}));
}
