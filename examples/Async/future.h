/*
 *	future lib is under raylist licence by abdorayden (RayDen)
 *	this lib is under raylist v2.3.0 LICENSE 
 * */

// TODO : separit tasks by groups every group chain between them and share data between them
// TODO : and add timeout (just found better way)
// TODO : create system managment task 

#ifndef FUTURE_H
#define FUTURE_H

#ifndef interface
#define interface	struct
#endif

typedef int Task;
typedef int GroupCount;

// this structure used to handle return data
// it contains bool isfinished to check if the task finish
// and bool is error to handle error
// return_data to work with

typedef struct {
	bool 	isfinished;
	void* 	iserror;
	void* 	return_data;
}HandlFuture;

// define function type for target function
typedef HandlFuture 	(*Poll)(void*);
// this callback function called in FutureLoop to work with return_data
// void* for the data
// int for spesific task , the goal is to differentiate the work between tasks
typedef void 		(*FutureOnData)(void* , Task);

// OnData anonymouse callback function macro to handle data directly
#define OnData(body)	\
	RLAmbda(void , (void* futuredata , Task futuretask) , body)

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

// OnErr anonymouse callback function macro to handle error directly
#define OnErr(body)	\
	RLAmbda(StatusFuture , (void* futurerror , Task futuretask) , body)

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
	GroupCount count;
}Future;

static int task_count = 0;

#define FUTURE_LAYER	LIST_MAX
RLLOCAL RLCollections FutureQueue;
GroupCount count_group = 0;

Future* FutureNewTask(Poll , void*, Priority);
void FuturePushTaskDirectrly(Future*);
void FutureAddTasksGroup(Future**, size_t);
void FutureLoop(FutureOnData, FutureOnErr);

#define done(data)			\
	(HandlFuture) {			\
		.isfinished = true,	\
		.iserror = NULL,	\
		.return_data = (data)	\
	}

#define notyet(data)			\
	(HandlFuture) {			\
		.isfinished = false,	\
		.iserror = NULL,	\
		.return_data = (data)	\
	}

#define error(err)				\
	(HandlFuture) {				\
		.isfinished = false,		\
		.iserror = (err),		\
		.return_data = NULL		\
	}

#define reject(err)				\
	(HandlFuture) {				\
		.isfinished = true,		\
		.iserror = (err),		\
		.return_data = NULL		\
	}

#endif //FUTURE_H

#ifdef FUTURE_C

Future* FutureNewTask(Poll poll , void* data , Priority priority)
{
	Future* future = malloc(sizeof(Future));
	future->poll = poll;
	future->data = data;
	future->task_id = task_count++;
	future->priority = priority;
	//future->count = count_group++;
	return future;
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


// push single Task to the queue
void FuturePushTaskDirectrly(Future* future){
	// init queue
	RLSetObject(FUTURE_LAYER);
	FutureQueue = Queue(Buf_Disable);
	RLCopyObject(RLSIZEOF(Future));
	FutureQueue.Push(RL_VOIDPTR , future);
	RLDisableCopyObject();
}
// this function will accept futures as one group it manage priority between them 
// and can share data between them
void FutureAddTasksGroup(Future** futures , size_t n)
{
	// init queue
	RLSetObject(FUTURE_LAYER);
	FutureQueue = Queue(Buf_Disable);

	RLCopyObject(RLSIZEOF(Future));
	// push array of tasks to queue
	for(size_t start = 0 ; start < n ; start++){
		for(int i = 0 ; i < futures[start]->priority ; i++)
			FutureQueue.Push(RL_VOIDPTR , futures[start]);
	}
	RLDisableCopyObject();
}

// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|

void FutureLoop(FutureOnData work , FutureOnErr logerr) 
{
	RLSetObject(FUTURE_LAYER);
	RLDefer(FutureQueue.Clear);
	Future* curr;
	// initial start from first group
	// because they are so lucky *-*
	GroupCount ctemp = 0;
	// TODO : handle id of other groups using other queue to manage data between groupes in safety
	// TODO : found better solution for alarm signal timeout
	// TODO : handle memory managment in both libs future.h and raylist.h
	// TODO : handle priority in FutureAddTasksGroup (critical pritiority must work after all priority tasks)
	// Example : 
	// 	three tasks : 1)- hight , 2)- Mediem , 3)- Low
	// 	Hight -> Mediem -> Hight -> Low -> Hight -> Mediem
	StatusFuture status = Ignore;
	// while FutureQueue is not empty
	while (!FutureQueue.Is_Empty()) {
		// pop the unit task
		curr = (Future*)FutureQueue.Pop();
		HandlFuture handle = curr->poll((void*)curr->data);
		// call function and check if function finished or not
		// TODO: done
		if(handle.iserror != NULL && handle.isfinished){
			if(logerr != NULL) 	status = logerr(handle.iserror , curr->task_id);
				// if logerr function was NULL task will be killed
			else 			status = KillTask;

			if(work != NULL)
			{
				if(handle.return_data != NULL)
					work(handle.return_data , curr->task_id);
			}

		// error
		} else if (handle.iserror && !handle.isfinished) {
			// function is not finished 
			// we pushed back to the FutureQueue
			if(logerr != NULL){
				// handle the data with work function
				status = logerr(handle.iserror , curr->task_id);
			}
			if(status == Ignore){
				FutureQueue.Push(RL_VOIDPTR , curr);
			}
		// notyet
		}else if(!handle.isfinished && handle.return_data && !handle.iserror){
			if(work != NULL){
				// handle the data with work function
				work(handle.return_data , curr->task_id);
			}
			FutureQueue.Push(RL_VOIDPTR , curr);
		}else if(handle.isfinished && handle.iserror) {
			if(logerr != NULL)
			{
				status = logerr(handle.iserror , curr->task_id);
			}else {
				status = KillTask;
			}
		}

		if(status == KillTask){
			status = Ignore;
			if(curr != NULL)
				RLFREE(curr);
		} else if(status == KillProgram){
			if(curr != NULL)
				RLFREE(curr);
			exit(1);
		}
	}
}

#endif // FUTURE_C
