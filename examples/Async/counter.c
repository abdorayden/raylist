/*
 *	implements asynchronous run functions in single thread using queue in raylist library 
 *	RayList v2.3.0
 * */
#include <stdio.h>
#include <stdlib.h>

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
	if(dc->first < dc->last) {
		dc->first++;
		return notyet((void*)dc);
	}
	return done(NULL);
}

HandlFuture task_poll(void* dta) {
	CounterData* data = (CounterData*)dta;
	if (data->count < data->max_count) {
		data->count++;
		return notyet((void*)data);
	}
	return done(NULL);
}

int main(void) {
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

	Future* task[4];
	task[0] = FutureNewTask(task_poll, data1, Low);
	task[1] = FutureNewTask(task_poll, data2, Low);
	task[2] = FutureNewTask(task_poll_char, data3, Low);
	task[3] = FutureNewTask(task_poll_char, data4, Low);

	if (!task[0] || !task[1] || !task[2] || !task[3]) {
		fprintf(stderr, "Failed to create tasks\n");
		exit(1);
	}

	FutureAddTasksGroup(task, 4);

	FutureLoop(
		OnData({
			if(futuretask == 0 || futuretask == 1) {
				CounterData* data = (CounterData*)futuredata;
				printf("Debug :: %d\n", data->count);
			} else {
				CounterChar* data = (CounterChar*)futuredata;
				printf("DEBUG :: char : %c\n", data->first);
			}
		}), OnErr({
			fprintf(stderr, "ERROR : task (%d) : %s\n", futuretask, (char*)futurerror);
			fprintf(stderr, "killing task %d\n", futuretask);
			return KillTask;
		})
	);

	free(data1);
	free(data2);
	free(data3);
	free(data4);
	return 0;
}
