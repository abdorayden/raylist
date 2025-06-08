/*
 *	future lib is under raylist licence by abdorayden (RayDen)
 *	this lib is under raylist v2.3.0 LICENSE 
 */

#ifndef FUTURE_H
#define FUTURE_H

#ifndef interface
#define interface struct
#endif

#include <stdbool.h>
#include <stdlib.h>

typedef int Task;
typedef int GroupCount;

// introduce chaining then/catch
typedef struct FutureChain FutureChain;

// this structure used to handle return data
// it contains bool isfinished to check if the task finish
// and bool is error to handle error
// return_data to work with
typedef struct {
	bool isfinished;
	bool iserror;
	void* return_data;
	void* error_data;
} HandlFuture;

// define function type for target function
typedef HandlFuture (*Poll)(void*);

// this callback function called in FutureLoop to work with return_data
// void* for the data
// int for spesific task , the goal is to differentiate the work between tasks
typedef void (*FutureOnData)(void*, Task);

// this callback is used to chain thens
typedef void (*ThenCallback)(void*, void**);

// StatusFuture
typedef enum {
	Ignore,
	KillTask,
	KillProgram
}StatusFuture;

// Priority enumeration 
// every task has own Priority 
// this task can executed 2 times if his priorety was hight except Low
//
//	Example :
//
// 	task1	<- Hight	|	this task will executed 2 times in round
// 	task2	<- Mediem	| 	this will execute 1 time in round
// 	task3	<- Low		|	this task it can be dispensed with except task1 and will executed 1 time in 2 rounds
typedef enum {
	Low	= 1	,
	Mediem		,
	Hight		,
	Critical
}Priority;

// log error callback function accept return value to check the error , and task id
// return StatusFuture to Determine the result after an error occurs
typedef StatusFuture 	(*FutureOnErr)(void* ,Task);

// .catch(CatchCallback) ...
typedef void (*CatchCallback)(void*);

typedef interface Future Future;

typedef struct FutureChain {
	Future* future;
	ThenCallback then_cb;
	CatchCallback catch_cb;
	struct FutureChain* next;
} FutureChain;

// OnData anonymouse callback function macro to handle data directly
#define OnData(body) \
	RLAmbda(void, (void* futuredata, Task futuretask), body)

// OnErr anonymouse callback function macro to handle error directly
#define OnErr(body) \
	RLAmbda(StatusFuture, (void* futurerror, Task futuretask), body)

// unit task
typedef interface Future {
	// Abstract method , should be implemented
	Poll poll;
	// the data
	void* data;
	// id of the task
	Task task_id;
	// this is priorety of the task
	Priority priority;
	// future groupe id
	GroupCount group_id ;
	// chain support for Future
	FutureChain* chain;
}Future;

static int task_count = 0;
static GroupCount group_count = 0;

#define FUTURE_LAYER LIST_MAX
RLLOCAL RLCollections FutureQueue;

Future* FutureNewTask(Poll poll, void* data, Priority priority);
void FuturePushTaskDirectrly(Future*);
void FutureAddTasksGroup(Future**, size_t);
void FutureLoop(FutureOnData, FutureOnErr);

// abstract chainig functions
// TODO: create macros for simplify the then/catch api
Future* FutureThen(Future* future, ThenCallback callback);
Future* FutureCatch(Future* future, CatchCallback callback);
void FutureExecuteChain(Future* future, HandlFuture result);

#define then(future , callback)  FutureThen(future, callback)
#define catch(future , callback)  FutureCatch(future, callback)

#define done(data) \
	(HandlFuture) { \
		.isfinished = true, \
		.iserror = false, \
		.return_data = (data), \
		.error_data = NULL \
	}

#define notyet(data) \
	(HandlFuture) { \
		.isfinished = false, \
		.iserror = false, \
		.return_data = (data), \
		.error_data = NULL \
	}

#define error(err) \
	(HandlFuture) { \
		.isfinished = false, \
		.iserror = true, \
		.return_data = NULL, \
		.error_data = (err) \
	}

#define reject(err) \
	(HandlFuture) { \
		.isfinished = true, \
		.iserror = true, \
		.return_data = NULL, \
		.error_data = (err) \
	}

#endif // FUTURE_H

#ifdef FUTURE_C

Future* FutureNewTask(Poll poll, void* data, Priority priority) {
	Future* future = malloc(sizeof(Future));
	if (!future) return NULL;

	future->poll = poll;
	future->data = data;
	future->task_id = task_count++;
	future->priority = priority;
	future->group_id = group_count;
	future->chain = NULL;
	return future;
}

Future* FutureThen(Future* future, ThenCallback callback) {
	if (!future) return NULL;

	FutureChain* new_link = malloc(sizeof(FutureChain));
	if (!new_link) return future;

	new_link->then_cb = callback;
	new_link->catch_cb = NULL;
	new_link->next = NULL;

	if (!future->chain) {
		future->chain = new_link;
	} else {
		FutureChain* current = future->chain;
		while (current->next) {
			current = current->next;
		}
		current->next = new_link;
	}

	return future;
}

Future* FutureCatch(Future* future, CatchCallback callback) {
	if (!future) return NULL;

	FutureChain* new_link = malloc(sizeof(FutureChain));
	if (!new_link) return future;

	new_link->then_cb = NULL;
	new_link->catch_cb = callback;
	new_link->next = NULL;

	if (!future->chain) {
		future->chain = new_link;
	} else {
		FutureChain* current = future->chain;
		while (current->next) {
			current = current->next;
		}
		current->next = new_link;
	}

	return future;
}

void FutureExecuteChain(Future* future, HandlFuture result) {
	if (!future || !future->chain) return;

	FutureChain* current = future->chain;
	void* current_data = result.return_data;

	while (current) {
		if (result.iserror) {
			if (current->catch_cb) {
				current->catch_cb(result.error_data);
				break;  // After catch, stop the chain
			}
		} else {
			if (current->then_cb) {
				void* new_data = NULL;
				current->then_cb(current_data, &new_data);
				current_data = new_data;
			}
		}
		current = current->next;
	}
}

// initializing the queue for the future loop
void FutureInitQueue() {
	static bool initialized = false;
	if (!initialized) {
		RLSetObject(FUTURE_LAYER);
		FutureQueue = Queue(Buf_Disable);
		RLCopyObject(RLSIZEOF(Future));
		RLDisableCopyObject();
		initialized = true;
	}
}

// NOTE: this future library take her own layer from raylist
// if you gonna create multi lists (more layers) and you use future library 
// you can't access last layer of the lists
//
// 	|   last layer | <- last layer is taken by future library
// 	|   layer3     |
// 	|   layer2     |
// 	|   layer1     | 
// 	 --------------
void FuturePushTaskDirectrly(Future* future) {
	if (!future) return;
	FutureInitQueue();

	// Push task according to its priority
	for (int i = 0; i < future->priority; i++) {
		FutureQueue.Push(RL_VOIDPTR, future);
	}
}

// this function will accept futures as one group it manage priority between them 
// and can share data between them
void FutureAddTasksGroup(Future** futures, size_t n) {
	if (!futures || n == 0) return;
	FutureInitQueue();

	for (size_t i = 0; i < n; i++) {
		if (futures[i]) {
			FutureQueue.Push(RL_VOIDPTR, futures[i]);
		}
	}
	group_count++;
}

static void FreeFutureChain(FutureChain* chain) {
	while (chain) {
		FutureChain* next = chain->next;
		free(chain);
		chain = next;
	}
}

static void FreeFuture(Future* future) {
	if (!future) return;
	if (future->data != NULL) {
		// NOTE: got double free detected , even i checked if the data does not equal to NULL
		// NOTE: i think the data is freed but it's not pointing to NULL addr
		/* free(future->data); */
		future->data = NULL;
	}
	FreeFutureChain(future->chain);
	free(future);
}

// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|
void FutureLoop(FutureOnData work, FutureOnErr logerr) {
	FutureInitQueue();
	RLDefer(FutureQueue.Clear);

	Future* curr;
	StatusFuture status = Ignore;

	while (!FutureQueue.Is_Empty()) {
		curr = (Future*)FutureQueue.Pop().GetData();
		if (!curr) continue;

		HandlFuture handle = curr->poll(curr->data);

		if (curr->chain && (handle.isfinished || handle.iserror)) {
			FutureExecuteChain(curr, handle);
		}

		if (handle.iserror) {
			if (handle.isfinished) {
				if (logerr) {
					status = logerr(handle.error_data, curr->task_id);
				} else {
					status = KillTask;
				}
			} else {
				if (logerr) {
					status = logerr(handle.error_data, curr->task_id);
				}
				if (status == Ignore) {
					FutureQueue.Push(RL_VOIDPTR, curr);
					continue;
				}
			}
		} else if (handle.isfinished) {
			if (work && handle.return_data) {
				work(handle.return_data, curr->task_id);
			}
			FreeFuture(curr);
			continue;
		} else {
			if (work && handle.return_data) {
				work(handle.return_data, curr->task_id);
			}
			FutureQueue.Push(RL_VOIDPTR, curr);
			continue;
		}

		if (status == KillTask) {
			status = Ignore;
			FreeFuture(curr);
		} else if (status == KillProgram) {
			FreeFuture(curr);
			exit(1);
		}
	}
}

#endif // FUTURE_C
