// improve library
//
//12. Random Number Generator and Summation
//
//    Tasks:
//        Generate random numbers continuously and store them in a list.
//        Calculate the sum of numbers in the list every second.
//    Purpose: Test task synchronization with shared resources (e.g., a list).
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define LIST_C
#include "../../raylist.h"

#define FUTURE_C
#include "future.h"

// init how many operation can do
#define END	10
int i = 0;
#define ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))

typedef struct 
{
	int* appendlist;
	int idx;
} SumRandom;

// implement poll abstract method
HandlFuture random_poll(void* data)
{
	SumRandom* datadd = (SumRandom*)data;
	if(i < END){
		i++;
		datadd->appendlist[datadd->idx++] = rand() % 100;
		// DEBUGGING
		//for(int x = 0; x < ARRAY_SIZE(datadd->appendlist) ; x++)
		//	printf("datadd : %d\n" , datadd->appendlist[x]);

		//puts("-----------------");
		return (HandlFuture){
			.isfinished = false,
			.iserror = NULL,
			.return_data = datadd
		};
	}
	return (HandlFuture){
		.isfinished = true,
		.iserror = "in random function task equal to NULL",
		.return_data = NULL
	};
}

// implement poll abstract method
HandlFuture sum_poll(void* data)
{
	SumRandom* datadd = (SumRandom*)data;
	size_t size = ARRAY_SIZE(datadd->appendlist); 
	if(size > 1 && i < END){
		int res = 0;
		for(int i = 0 ; i < size ; i++){
			res += *(datadd->appendlist + i);
		}
		//printf("res : %d\n" , res);
		//puts("-------------------");
		datadd->idx = 0;
		datadd->appendlist[datadd->idx++] = res;
		return (HandlFuture){
			.isfinished = false,
			.iserror = NULL,
			.return_data = datadd
		};
	}
	return (HandlFuture){
		.isfinished = true,
		.iserror = "in sum function data equal to NULL",
		.return_data = NULL
	};
}

void* logg(void* data , int task)
{
	if(task == 0){
		printf("LOG : i = %d , random number = %d\n" , i , *(int*)data);
	}
	return data;
}

int main(){
	srand(time(NULL));

	SumRandom* target = malloc(sizeof(SumRandom));
	target->appendlist = malloc(sizeof(int)*5); 
	target->idx = 0;

	target->appendlist[target->idx++] = rand() % 100;

	Future* tasks[2] = 
	{
		FutureNewTask(random_poll , target, Critical),
		FutureNewTask(sum_poll    , target, Low)
	};

	FutureAddTasks(tasks , 2);

	FutureLoop(OnData(
	{
			if(futuretask == 1)
				printf("OnData : %d\n" , *((SumRandom*)futuredata)->appendlist);
	}
	) , OnErr({
		fprintf(stderr , "ERROR : task %d :: %s" , futuretask , futurerror);
		return KillProgram;
	}));

	printf("\nfinal result : %d , %d | with size : %zu" , *target->appendlist, *(target->appendlist + 1) , ARRAY_SIZE(target->appendlist));
	// you need to manage allocated data of structure fields
	free(target->appendlist);
	return 0;
}
