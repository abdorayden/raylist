/*
 *	future lib is under raylist licence by abdorayden (RayDen)
 * */

#ifndef FUTURE_H
#define FUTURE_H

#ifndef interface
#define interface	struct
#endif

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
typedef void* 		(*DoWorkInFutureLoop)(void* , int);

// unit task
typedef interface Future {
	// Abstract method , should be implemented
	Poll poll;
	// the data
	void* data;
	// id of the task
	int task_id;
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

// executable function
//  |	   	|							      |	          |
//  |      	|                                                             |	          |
//  |  		| => exec func1 (count ++ ,                                   |           |
//  | 	        |			exit ,                                |           |
//  |  func2    |			function not finish , pushed back)    |  func1    |
//  |  func1    |                                                             |  func2    |
//  |___________|                                                      	      |___________|

void FutureLoop(RLCollections queue , DoWorkInFutureLoop callback) {
	// while queue is not empty
	while (!queue.Is_Empty()) {
		// pop the unit task
		Future* curr = (Future*)queue.Pop();

		HandlFuture handle = curr->poll((void*)curr);
		// call function and check if function finished or not
		if (!handle.isfinished) {
			// function is not finished 
			// we pushed back to the queue
			if(callback != NULL){
				// handle the data with callback function
				handle.return_data = callback(handle.return_data , curr->task_id);
			}
			if(handle.iserror){
				// check errno or log error
			}
			queue.Push(RL_VOIDPTR , curr);
		} else {
			// function is done 
			// free the allocated data curr and skip
			free(curr);
		}
	}
}
#endif
