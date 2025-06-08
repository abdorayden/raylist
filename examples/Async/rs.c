// improve library
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LIST_C
#include "../../raylist.h"

#define FUTURE_C
#include "future.h"

#define END 10
#define BUFFER_SIZE 5

typedef struct {
	int appendlist[BUFFER_SIZE];
	int idx;
} SumRandom;

HandlFuture random_poll(void* data) {
	SumRandom* datadd = (SumRandom*)data;
	static int i = 0;

	if(i < END) {
		i++;
		if(datadd->idx < BUFFER_SIZE) {
			datadd->appendlist[datadd->idx++] = rand() % 100;
			return notyet(datadd);
		} else {
			return error("Buffer full");
		}
	}
	return done(NULL);
}

HandlFuture sum_poll(void* data) {
	SumRandom* datadd = (SumRandom*)data;

	if(datadd->idx > 1) {
		int res = 0;
		for(int i = 0; i < datadd->idx; i++) {
			res += datadd->appendlist[i];
		}

		datadd->appendlist[0] = res;
		datadd->idx = 1;

		return notyet(datadd);
	}
	return done(NULL);
}

int main() {
	srand(time(NULL));

	SumRandom* target = malloc(sizeof(SumRandom));
	if (!target) {
		fprintf(stderr, "Memory allocation failed\n");
		return 1;
	}

	target->idx = 0;
	target->appendlist[target->idx++] = rand() % 100;

	Future* tasks[2] = {
		FutureNewTask(random_poll, target, Critical),
		FutureNewTask(sum_poll, target, Low)
	};

	if (!tasks[0] || !tasks[1]) {
		fprintf(stderr, "Failed to create tasks\n");
		free(target);
		return 1;
	}

	FutureAddTasksGroup(tasks, 2);

	FutureLoop(
		OnData({
			if(futuretask == 1) {
				SumRandom* data = (SumRandom*)futuredata;
				printf("Sum result: %d\n", data->appendlist[0]);
			}
		}), 
		NULL
	);

	printf("\nFinal result: %d (buffer size: %d)\n", 
			target->appendlist[0], target->idx);

	free(target);
	return 0;
}
