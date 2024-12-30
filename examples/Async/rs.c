// improve library
//
//12. Random Number Generator and Summation
//
//    Tasks:
//        Generate random numbers continuously and store them in a list.
//        Calculate the sum of numbers in the list every second.
//    Purpose: Test task synchronization with shared resources (e.g., a list).

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define LIST_C
#include "../../raylist.h"

#define FUTURE_C
#include "future.h"

#define END	10
int i = 0;
int idx = 0; 
#define ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))

HandlFuture random_poll(void* task){
	int* datadd = (int*)((Future*)task)->data;
	if(i < END){
		i++;
		datadd[idx++] = rand() % 100;
		// DEBUGGING
		for(int x = 0; x < ARRAY_SIZE(datadd) ; x++)
			printf("datadd : %d\n" , datadd[x]);

		puts("-----------------");
		return (HandlFuture){
			.isfinished = false,
			.iserror = false,
			.return_data = datadd
		};
	}
	return (HandlFuture){
		.isfinished = true,
		.iserror = false,
		.return_data = NULL
	};
}

HandlFuture sum_poll(void* task){
	int* datadd = (int*)((Future*)task)->data;
	size_t size = ARRAY_SIZE(datadd); 
	if(size > 1 && i < END){
		int res = 0;
		for(int i = 0 ; i < size ; i++){
			res += *(datadd + i);
		}
		printf("res : %d\n" , res);
		puts("-------------------");
		idx = 0;
		datadd[idx++] = res;
		return (HandlFuture){
			.isfinished = false,
			.iserror = false,
			.return_data = datadd
		};
	}
	return (HandlFuture){
		.isfinished = true,
		.iserror = false,
		.return_data = NULL
	};
}

void* logg(void* data , int task){
	if(task == 0){
		printf("LOG : i = %d , random number = %d\n" , i , *(int*)data);
	}
	return data;
}

int main(){
	srand(time(NULL));
	// we need array of 3 integer 

	int* appendlist = malloc(sizeof(int)*3);
	appendlist[idx++] = rand() % 100;
	RLCollections queue = Queue(Buf_Disable);

	Future* add_task = FutureNewTask(random_poll , appendlist);
	Future* sum_task = FutureNewTask(sum_poll    , appendlist);

	queue.Push(RL_VOIDPTR , add_task);
	queue.Push(RL_VOIDPTR , sum_task);

	FutureLoop(queue , NULL);

	printf("\nfinal result : %d" , *appendlist);
	return 0;
}
