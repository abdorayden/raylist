/*
 *	future lib is under raylist licence by abdorayden (RayDen)
 *	this lib is under raylist v2.3.0 LICENSE 
 * */

#ifndef FUTURE_H
#define FUTURE_H

#ifndef interface
#define interface	struct
#endif

typedef int Task;

// this structure used to handle return data
// it contains bool isfinished to check if the task finish
// and bool is error to handle error
// return_data to work with
typedef struct {
	bool 	isfinished;
	bool 	iserror;
	void* 	return_data;
}HandlFuture;

// define function type for target function
typedef HandlFuture 	(*Poll)(void*);
// this callback function called in FutureLoop to work with return_data
// void* for the data
// int for spesific task , the goal is to differentiate the work between tasks
typedef void* 		(*FutureOnData)(void* , Task);

// OnData anonymouse callback function macro to handle data directly
#define OnData(body)	\
	RLAmbda(void* , (void* futuredata , Task futuretask) , body)

// StatusFuture
typedef enum {
	Ignore,
	KillTask,
	KillProgram
}StatusFuture;

// log error callback function accept return value to check the error , and task id
// return StatusFuture to Determine the result after an error occurs
typedef StatusFuture 	(*FutureOnErr)(Task);

// OnErr anonymouse callback function macro to handle error directly
#define OnErr(body)	\
	RLAmbda(StatusFuture , (Task futuretask) , body)

// unit task
typedef interface Future {
	// Abstract method , should be implemented
	Poll poll;
	// the data
	void* data;
	// id of the task
	Task task_id;
}Future;

static int task_count = 0;

#endif //FUTURE_H

#ifdef FUTURE_C

Future* FutureNewTask(Poll poll , void* data){
	Future* future = malloc(sizeof(Future));
	future->poll = poll;
	future->data = data;
	future->task_id = task_count++;
	return future;
}

// NOTE: this future library take her own layer from raylist
// if you gonna create multi lists (more layers) and you use future library 
// you can't access first layer of the lists
// 	|	 |
// 	| layer3 |
// 	| layer2 |
// 	| layer1 | <- first layer is taken by future library
// 	 --------
#define FUTURE_LAYER	1
RLLOCAL RLCollections FutureQueue;

void FutureAddTasks(Future** futures , size_t n){
	// init queue
	FutureQueue = Queue(Buf_Disable);
	RLSetObject(FUTURE_LAYER);

	// push array of tasks to queue
	for(size_t start = 0 ; start < n ; start++){
		FutureQueue.Push(RL_VOIDPTR , futures[start]);
	}
}

// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|

void FutureLoop(FutureOnData work , FutureOnErr logerr) {
	RLSetObject(FUTURE_LAYER);
	RLDefer(FutureQueue.Clear);
	StatusFuture status = Ignore;
	// while FutureQueue is not empty
	while (!FutureQueue.Is_Empty()) {
		// pop the unit task
		Future* curr = (Future*)FutureQueue.Pop();

		HandlFuture handle = curr->poll((void*)curr);
		// call function and check if function finished or not
		if (!handle.isfinished) {
			if(handle.iserror){
				if(logerr != NULL){
					status = logerr(curr->task_id);
					if(status != Ignore){
						break;
					}
				}
			}
			// function is not finished 
			// we pushed back to the FutureQueue
			if(work != NULL){
				// handle the data with work function
				handle.return_data = work(handle.return_data , curr->task_id);
			}
			FutureQueue.Push(RL_VOIDPTR , curr);
		} else {
			// function is done 
			// free the allocated data curr and skip
			RLFREE(curr);
		}
		if(status == KillTask){
			RLFREE(curr);
		} else if(status == KillProgram){
			RLFREE(curr);
			return;
		}
	}
}
#endif // FUTURE_C
