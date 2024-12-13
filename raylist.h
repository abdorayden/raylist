/************************************************************************************************
*			Copyright (c) 2024 Ray Den	raylist v2.0.0				*
*												*
*	Permission is hereby granted, free of charge, to any person obtaining a copy		*
*	of this software and associated documentation files (the "Software"), to deal		*
*	in the Software without restriction, including without limitation the rights		*
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell		*
*	copies of the Software, and to permit persons to whom the Software is			*
*	furnished to do so, subject to the following conditions:				*
*												*
*	The above copyright notice and this permission notice shall be included in		*
*	all copies or substantial portions of the Software.					*
*												*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		*
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,		*	
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		*
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER			*
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,		*	
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN		*
*	THE SOFTWARE.										*
*												*
 ************************************************************************************************/

// TODO : add thread	function 
// TODO : add thread	type

#if !defined(LIST_H) || !defined(LIST_INCLUDED)
#define LIST_H
#define LIST_INCLUDED 
// to check if raylist is included

// 	typeof pragma returns string contained type of variable
// 	Example :
// 		int var = 4;
// 		printf("typeof(test) : %s" , typeof(var));
// 	Output :
// 		typeof(test) : int

#define typeof(x) _Generic((x),   \
    int		: "int"		, \
    int   *	: "int*"	, \
    float *	: "float*"	, \
    float	: "float"	, \
    double	: "double"	, \
    double*	: "double*"	, \
    char	: "char"	, \
    char  *	: "char*"	, \
    void  *	: "void*"	, \
    short 	: "short"	, \
    short * 	: "short*"	, \
    default	: "unknown"	  \
    )

/*
 *	local type
 * */
#define RLLOCAL	static

/*
 *	if not defined LBOOL the developer not using other boolean style
 *	so we used the defualt 
 * */
#ifndef LBOOL

typedef enum{
	/* by default false take 0 value and that means false 		*/
	false,
	/* not zero that means true 					*/
	true = !(false)
}bool;
#define LBOOL bool

#endif

RLLOCAL LBOOL revesed = false;

/*
 *	if not defined RLALLOC the developer not using other allocation memory style
 *	so we used the defualt 
 * */
#ifndef RLALLOC
/*
 *	check if stdlib is included
 * */
#if defined(_STDLIB_H) && _STDLIB_H == 1
#include <stdlib.h>
#endif

#define RLALLOC	malloc

#endif

#ifndef RLFREE
/*
 *	check if stdlib is included
 * */
#if defined(_STDLIB_H) && _STDLIB_H == 1
#include <stdlib.h>
#endif

#define RLFREE	free

#endif

#ifndef RLUNUSED
#define RLUNUSED(x) (void)x
#endif

/*
 *	API
 *	this api is used to create thread for multiple operating systems
 *	this api used for running Asynchronous functions
 * */
#ifdef _WIN32

#include <windows.h>
#define RLAPIThread HANDLE
#define RLAPICallBackFunction	LPTHREAD_START_ROUTINE
#define RLAPIParam		LPVOID

#define RLTestCancel()

#else

#ifndef _PTHREAD_H
#include <pthread.h>
#endif

#define RLAPIThread pthread_t
#define RLAPICallBackFunction	void*
#define RLAPIParam		void*

#define RLTestCancel		pthread_testcancel

#endif



// Type enum contained bunch of types helps functions any variables type will work for
// STR for strings BOOLL for boolean ... 
typedef enum{
	CHR = 0, 	// char type
	STR    , 	// string type
	BOOLL   ,	// bool type
	INTT    , 	// int type
	FLT    , 	// float type
	VOIDPTR,	// void pointer type

	VOIDFUNC,
	INTFUNC ,
	CHARFUNC,
	STRFUNC
}Type;

typedef enum{
	FINE = 0,
	LIST_INDEX_OUT_OF_RANGE,
	LIST_MEMALLOC,
	NULL_VALUE,
	ERROR_WAIT_THREAD,
	ERROR_KILL_THREAD,
	ERROR_CREATE_THREAD,
	LIST_EMPTY
}ListError;

// global status to handle raylist errors
RLLOCAL ListError status = FINE;

typedef char* string;

/*
 *	the call back function type in list
 * */

typedef void 	(*VOIDFUNCTION		)(void*);
typedef int  	(*INTEGERFUNCTION  	)(void*);
typedef LBOOL 	(*BOOLEANFUNCTION  	)(void*);
typedef char 	(*CHARACTERFUNCTION	)(void*);
typedef string 	(*STRINGFUNCTION 	)(void*);

/*
 *	Linked List struct
 *		index : index of single list
 *		type  : type of the data memory saved in void* data pointer
 *		data  : the place we store the address of our variables 
 *		next  : pointing to next list
 * */
struct list {
	int 		index;
	Type 		type ;
	void* 		data ;
	struct list* 	next ;
};

typedef struct list List;

// global count for index
RLLOCAL int global_count = 0;

// Global list variable 
RLLOCAL List* __list__ = NULL;

// filter callback function 
typedef enum{
	ALL = 0xFF,  	// delete all types 
	ONLY		// only type
}Filter_Flag;

typedef LBOOL (*FILTERCALLBACK)(void*);

// map call back function
typedef void* (*MAPCALLBACK)(void*);

// Exec function Flag
typedef enum {
	/* return value from Exec function
	 * OUTT flag is not enabled if index parameter in Exec_Func was out of range
	 * */ 
	OUTT, 
	/* the return value will replace function callback index ,
	* the function Exec will run all function 
	* the void function is not replaced 
	* */
	INPLACE,  
	/* the function return value will added to the list */
	ADDED
}Exec_Flag;

/*
 *	interface for manipulate thread handle 
 * */
typedef struct {
	// the thread in it self
	RLAPIThread thread;
	/*
	 *	Wait function 
	 *	function waits for the thread specified by thread to terminate
	 * */
	void (*Wait)(RLAPIThread thread);
	/*
	 *	Kill function
	 *	for killing the thread
	 * */
	void (*Kill)(RLAPIThread thread);
}IfaceThread;

/*
 *	IfaceList struct act like class all thouse functions point to other functions 
 *
 *	Example :
 *		void foo(void){
 *			printf("bar");
 *		}
 *		IfaceList test;
 *		test.List_Append = foo;
 *		
 * */
typedef struct {
#ifndef USING_LIST
	/*
	 *	return true if stack is empty else false
	 * */
	LBOOL (*Queue_Is_Empty)(
			void
	);
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*Queue_Get_Error)(
			void
	);
	/*
	 *	Queue_Push function will take the data and stored to the __list__ global variable 
	 *
	 * 	Queue_Push(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *	
	 *	NOTE : the append method works like push in stack algorithm
	 *
	 *	Example :
	 *		my_list.Queue_Push(STR , "Hello World");
	 *
	 * */
	void (*Queue_Push)(
			Type type,
			void* data
	);
	/*
	 *	Peek function return last value of the Queue (Queue)
	 *	Example : 
	 *		void* value = my_list.Queue_Peek();
	 * */
	void* (*Queue_Peek)(void);
	/*
	 *	Queue_Pop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.Queue_Pop();
	 * */
	void* (*Queue_Pop)(void);
	// clear Queue
	void (*Queue_Clear)(
			void
	);

	///////////
	/*
	 *	return true if list is empty else false
	 * */
	LBOOL (*List_Is_Empty)(
			void
	);
	/*
	 *	return true if stack is empty else false
	 * */
	LBOOL (*Stack_Is_Empty)(
			void
	);
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*Stack_Get_Error)(
			void
	);
	/*
	 *	Stack_Push function will take the data and stored to the __list__ global variable 
	 *
	 * 	Stack_Push(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *	
	 *	NOTE : the append method works like push in stack algorithm
	 *
	 *	Example :
	 *		my_list.Stack_Push(STR , "Hello World");
	 *
	 * */
	void (*Stack_Push)(
			Type type,
			void* data
	);
	/*
	 *	Peek function return last value of the Stack (Stack)
	 *	Example : 
	 *		void* value = my_list.Stack_Peek();
	 * */
	void* (*Stack_Peek)(void);
	/*
	 *	Stack_Pop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.Stack_Pop();
	 * */
	void* (*Stack_Pop)(void);
	// clear Stack
	void (*Stack_Clear)(
			void
	);
	/*
	 *	List_Insert function will take the data and insert at the idx variable it in __list__ global linked list variable
	 *
	 *	List_Insert(
	 *		int idx		: 		index where we insert the data
	 *		Type type	: 		data type
	 *		void* data the  :		data
	 *	)
	 *	Exemple :
	 *		my_list.List_Insert(1 , BOOLL , true);
	 * */
	void (*List_Insert)(
			int idx,
			Type type,
			void* data
	);

	/*
	 *	List_Append function will take the data and stored to the __list__ global variable 
	 *
	 * 	List_Append(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *	
	 *	NOTE : the append method works like push in stack algorithm
	 *
	 *	Example :
	 *		my_list.List_Append(STR , "Hello World");
	 *
	 * */
	void (*List_Append)(
			Type type,
			void* data
	);
	/*
	 *	List_Filter function takes function callback that returns boolean and filter all elements from the list
	 *
	 *	Example : 
	 *	int check_mod_of_num(void* tch){
	 *		return (*(int*)tch % 2);
	 *	}
	 *	
	 *					before : 	[
	 *								3,2,7,5,1
	 *							]
	 *	my_list.List_Filter(check_mod_of_num , INTT);
	 *					after : 	[
	 *								2
	 *							]
	 * */
	void (*List_Filter)(
			FILTERCALLBACK,
			Type , 
			Filter_Flag flag
	);
	/*
	 * 	List_Search function return true if the data has been found and will store index of the data in int* parameter else will return false
	 *
	 *	List_Search(
	 *		int* : integer pointer variable to store index of the data we found
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 *	)
	 *
	 *	Example :
	 *		int var;
	 *		if(my_list.List_Search(&var, STR , "Hello World"))	// handle (var contained the index of the data)
	 *		else 							// handle
	 *
	 * */
	LBOOL (*List_Search)(
			int* ,
			Type ,
			void* 
	);
	/*
	 *	List_Del_Index function takes index and delete it from the linked list
	 *
	 *	List_Del_Index(
	 *		int : the index that we want to delete
	 *	)
	 *
	 *	Example :
	 *		my_list.List_Del_Index(3);
	 *
	 * */
	void (*List_Del_Index)(
			int idx 
	);
	/*
	 *	List_Get function takes index parameter and will return void* data  
	 *
	 *	List_Del_Index(
	 *		int : the index of the data we want to get
	 *	)
	 *
	 *	Example :
	 *		void* d = my_list.List_Get(2);
	 *		the data pointer in d variable now 
	 *
	 * */
	void* (*List_Get)(
			int idx
	);

	/*
	 *	List_Reverse function will reverse the list 
	 *
	 * */
	void (*List_Reverse)(
			void
	);
	/*
	 *	List_Print function will print all our data in standerd output 
	 * */
	void (*List_Print)(
			void
	);
	/*
	 * void bar{
	 * 	printf("Hello World");
	 * }
	 * IfaceList my_list = list(0);
	 * my_list.List_Append(VOIDFUNC , bar);
	 * 	the list now contains a function 
	 *  zero for execute function of index 0 , and NULL parameter mean the funtion doesm't accept a parameter
	 * my_list.List_Exec_Func(0 , NULL);
	 * */

	void* (*List_Exec_Func)(
			int idx,
			void* data,
			Exec_Flag flag
	);

	// clear list
	void (*List_Clear)(
			void
	);
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*List_Get_Error)(
			void
	);
	/*
	 *	map is a higher-order function that applies a given function to each element of a collection
	 *	Example : 
	 *
	 *		void* callback(void* d)
	 *		{
	 *			if(*(int*)d == 2)
	 *			{
	 *				*(int*)d = 10;
	 *			}
	 *			return d;
	 *		}
	 *		IfaceList my_list = list(0);
	 *		for(int i = 0 ; i < 4 ; i++){
	 *			RLAppend(i , INTT);
	 *		}
	 *		list : [0,1,2,3]
	 *		my_list.List_Map(callback , INTT);
	 *		list : [0,1,10,3]
	 * */ 
	void (*List_Map)(
			MAPCALLBACK	func,
			Type		type
	);
	/*
	 *	exec asynchronous functiona
	 *	IfaceList my_list = list(0);
	 *	my_list.Append(STRFUNC , func);
	 *	my_list.List_Exec_Async(0, NULL).Wait();
	 *
	 * */
	IfaceThread (*List_Exec_Async)(
			int idx,
			RLAPIParam data
	);
#else
	/*
	 *	return true if list is empty else false
	 * */
	LBOOL (*Is_Empty)(
			void
	);
	/*
	 *	Push function will take the data and stored to the __list__ global variable 
	 *
	 * 	Push(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *	
	 *	NOTE : the append method works like push in stack algorithm
	 *
	 *	Example :
	 *		my_list.Push(STR , "Hello World");
	 *
	 * */
	void (*Push)(
			Type type,
			void* data
	);
	/*
	 *	QPeek function return first value of the List (Queue)
	 *	Example : 
	 *		void* value = my_list.QPeek();
	 * */
	void* (*QPeek)(void);
	/*
	 *	QPop function will pop the first value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.QPop();
	 * */
	void* (*QPop)(void);
	/*
	 *	SPeek function return last value of the List (Stack)
	 *	Example : 
	 *		void* value = my_list.SPeek();
	 * */
	void* (*SPeek)(void);
	/*
	 *	SPop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.SPop();
	 * */
	void* (*SPop)(void);
	/*
	 *	Insert function will take the data and insert at the idx variable it in __list__ global linked list variable
	 *
	 *	Insert(
	 *		int idx		: 		index where we insert the data
	 *		Type type	: 		data type
	 *		void* data the  :		data
	 *	)
	 *	Exemple :
	 *		my_list.Insert(1 , BOOLL , true);
	 * */
	void (*Insert)(
			int idx,
			Type type,
			void* data
	);
	/*
	 *	Append function will take the data and stored to the __list__ global variable 
	 *
	 * 	Append(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *
	 *	Example :
	 *		my_list.Append(STR , "Hello World");
	 *
	 * */
	void (*Append)(
			Type type,
			void* data
	);
	/*
	 *	List_Filter function takes function callback that returns boolean and filter all elements from the list
	 *
	 *	Example : 
	 *	int check_mod_of_num(void* tch){
	 *		return (*(int*)tch % 2);
	 *	}
	 *	
	 *					before : 	[
	 *								3,2,7,5,1
	 *							]
	 *	my_list.List_Filter(check_mod_of_num , INTT);
	 *					after : 	[
	 *								2
	 *							]
	 * */
	void (*Filter)(
			FILTERCALLBACK,
			Type , 
			Filter_Flag flag
	);
	/*
	 * 	Search function return true if the data has been found and will store index of the data in int* parameter else will return false
	 *
	 *	Search(
	 *		int* : integer pointer variable to store index of the data we found
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 *	)
	 *
	 *	Example :
	 *		int var;
	 *		if(my_list.Search(&var, STR , "Hello World"))	// handle (var contained the index of the data)
	 *		else 							// handle
	 *
	 * */
	LBOOL (*Search)(
			int* ,
			Type ,
			void* 
	);
	/*
	 *	Del_Index function takes index and delete it from the linked list
	 *
	 *	Del_Index(
	 *		int : the index that we want to delete
	 *	)
	 *
	 *	Example :
	 *		my_list.Del_Index(3);
	 *
	 * */
	void (*Del_Index)(
			int idx 
	);
	/*
	 *	Get function takes index parameter and will return void* data  
	 *
	 *	Del_Index(
	 *		int : the index of the data we want to get
	 *	)
	 *
	 *	Example :
	 *		void* d = my_list.Get(2);
	 *		the data pointer in d variable now 
	 *
	 * */
	void* (*Get)(
			int idx
	);

	/*
	 *	Reverse function will reverse the list 
	 *
	 * */
	void (*Reverse)(
			void
	);
	/*
	 *	Print function will print all our data in standerd output 
	 * */
	void (*Print)(
			void
	);
	/*
	 * void bar{
	 * 	printf("Hello World");
	 * }
	 * IfaceList my_list = list(0);
	 * my_list.Append(VOIDFUNC , bar);
	 * 	the list now contains a function 
	 *  zero for execute function of index 0 , and NULL parameter mean the funtion doesm't accept a parameter
	 * my_list.Exec_Func(0 , NULL);
	 * */

	void* (*Exec_Func)(
			int idx,
			void* data,
			Exec_Flag flag
	);
	// clear list
	void (*Clear)(
			void
	);
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*Get_Error)(
			void
	);
	/*
	 *	map is a higher-order function that applies a given function to each element of a collection
	 *	Example : 
	 *
	 *		void* callback(void* d)
	 *		{
	 *			if(*(int*)d == 2)
	 *			{
	 *				*(int*)d = 10;
	 *			}
	 *			return d;
	 *		}
	 *		IfaceList my_list = list(0);
	 *		for(int i = 0 ; i < 4 ; i++){
	 *			RLAppend(i , INTT);
	 *		}
	 *		list : [0,1,2,3]
	 *		my_list.Map(callback , INTT);
	 *		list : [0,1,10,3]
	 * */ 
	void (*Map)(
			MAPCALLBACK	func,
			Type		type
	);
	/*
	 *	exec asynchronous functiona
	 *	IfaceList my_list = list(0);
	 *	my_list.Append(STRFUNC , func);
	 *	my_list.List_Exec_Async(0, NULL).Wait();
	 *
	 * */
	IfaceThread (*Exec_Async)(
			int idx,
			RLAPIParam data
	);
#endif
}IfaceList;


IfaceList list(
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);

IfaceList stack(void);

IfaceList queue(void);

#endif

#ifdef LIST_C

/*
 *	api function 
 * */
RLLOCAL RLAPIThread RLCreateThread(RLAPICallBackFunction func , RLAPIParam parameter)
{
#ifndef _WIN32
	RLAPIThread thread;
	if(pthread_create(&thread, NULL, func, parameter) == 0)
	{
		status = ERROR_CREATE_THREAD;
		return thread;
	}
#else
	RLAPIThread thread = CreateThread(NULL , 0 , func , parameter , 0 , NULL);
	if(thread == NULL){
		status = ERROR_CREATE_THREAD;
		return NULL ;
	}
#endif
	return thread;
}

RLLOCAL void RLWait(RLAPIThread thread)
{
#ifdef _WIN32
	if(WaitForSingleObject(thread , INFINITE) == WAIT_FAILED)
	{
		status = ERROR_WAIT_THREAD;
		return;
	}
#else
	if(pthread_join(thread , NULL) != 0)
	{
		status = ERROR_WAIT_THREAD;
		return;
	}
#endif
}

/*
 * NOTE : in Callback function don't forget to call RLTestCancel() function 
 * to enable kill thread
 * */

RLLOCAL void RLKill(RLAPIThread thread)
{
#ifdef _WIN32
	if(!TerminateThread(thread , 0))
	{
		status = ERROR_KILL_THREAD;
		return;
	}
	CloseHandle(thread);
#else
	if(pthread_cancel(thread) != 0)
	{
		status = ERROR_WAIT_THREAD;
		return;
	}
#endif
}

RLLOCAL void add_voidptr(
		List** __list__ , 
		void* val , 
		Type t, 
		int idx
){
	List* temp = RLALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = val;  
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}
RLLOCAL void add_int(
		List** __list__ , 
		int val , 
		Type t, 
		int idx
){
	List* temp = RLALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = RLALLOC(sizeof(int));
	*(int*)temp->data = val;
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

RLLOCAL void add_char(
		List** __list__ , 
		char val , 
		Type t, 
		int idx
){
	List* temp = RLALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = RLALLOC(sizeof(char));
	*(char*)temp->data = val;
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

RLLOCAL void add_float(
		List** __list__ , 
		float val , 
		Type t, 
		int idx
){
	List* temp = RLALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = RLALLOC(sizeof(float));
	*(float*)(temp->data) = val;
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

#define add(l , v , t , i) _Generic((v) ,	 	\
			int : add_int,			\
			char : add_char,		\
			float : add_float,		\
			default : add_voidptr		\
		)(l , v , t , i);

RLLOCAL void init(List** l){
	*l = NULL; 
}

RLLOCAL LBOOL l_is_empty(void)
{
	return (global_count == 0);
}

// the complexity still O(N) in worst case the index helps you to know the data u want to get later
RLLOCAL void local_l_insert(List** __list__ , int idx , Type type , void* data){
	List* local_list = *(__list__);

	if(idx < global_count && idx >= 0){
		while(local_list != NULL){
			if(local_list->index > idx){	
				local_list->index++;
			}
			if(idx == local_list->index){
				List* node = RLALLOC(sizeof(List));
				if(node == NULL)
				{
					status = LIST_MEMALLOC;
					return;
				}
				node->data = data;
				node->type = type;
				node->index = idx;

				local_list->index++;
				List* temp = local_list->next;
				local_list->next = node;
				node->next = temp;
				if(local_list->next != NULL)
					local_list = local_list->next;
			}

			local_list = local_list->next;
		}
		global_count++;
	}else{
		add(__list__ ,data , type , global_count++);
	}
}
RLLOCAL void* l_peek(void)
{
	return __list__->data;
}

RLLOCAL void* l_qpeek(void)
{
	List* temp = __list__;
	while(temp->next != NULL)	temp = temp->next;
	return temp->data;
}

RLLOCAL void* local_l_qpop(List** __list__){
	void* d;
	if(*__list__ == NULL)	return NULL;
	List* ___temp = *__list__;
	if((___temp)->next == NULL){
		d = ___temp->data;
		RLFREE(___temp);
		___temp = NULL;
		global_count--;
		return d;
	}
	while((___temp)->next->next != NULL)	___temp = (___temp)->next;
	d = ___temp->next->data;
	RLFREE(___temp->next);
	___temp->next = NULL;
	global_count--;
	return d;
}

RLLOCAL void* l_qpop(){
	return local_l_qpop(&__list__);
}

RLLOCAL void* local_l_pop(List** __list__){
	void* ret;
	List* temp = *__list__;
	if(temp == NULL)	return NULL;
	ret = temp->data;
	*__list__ = (*__list__)->next;
	RLFREE(temp);
	global_count--;
	return ret;
}

RLLOCAL void* l_pop(){
	return local_l_pop(&__list__);
}

RLLOCAL void l_insert(int idx , Type type , void* data){
	local_l_insert(&__list__ , idx , type , data);
}


#define RLAppend(t , d) _Generic((d) , 		\
			int : add_int,		\
			char : add_char,	\
			float : add_float,	\
			default : add_voidptr		\
		)(&__list__ , d , t  , global_count++);

#define RLPush(t , d) _Generic((d) , 		\
			int : add_int,		\
			char : add_char,	\
			float : add_float,	\
			default : add_voidptr		\
		)(&__list__ , d , t  , global_count++);

RLLOCAL void l_append(
		Type type,
		void* data
)
{
	if(status != FINE) status = FINE;
	add(&__list__ , data, type , global_count++);
}

RLLOCAL void l_push(
		Type type,
		void* data
)
{
	if(status != FINE) status = FINE;
	add(&__list__ , data, type , global_count++);
}

RLLOCAL void l_clear(){
	List* current = __list__;
	List* temp;
	while (current != NULL) {
		temp = current->next; 
		RLFREE(current);        
		current = temp;       
	}
	global_count = 0;
	init(&__list__);
}
RLLOCAL void* local_exec(List* __list__ ,int idx , void* data , Exec_Flag flag)
{
	if(idx > global_count){	
		status = LIST_INDEX_OUT_OF_RANGE;
		return NULL;
	
	}else if(__list__ == NULL){
		status = LIST_EMPTY;
		return NULL;
	}else{
		status = FINE;
	}
	if(idx < 0 || idx > global_count){
		while(__list__ != NULL){
			switch((__list__)->type){
				case  VOIDFUNC:{
					if(data == NULL){
						void (*call)() = (void(*)())(__list__)->data;
						call();
					}else{
						void (*call)(void*) = (void(*)(void*))(__list__)->data;
						call(data);
					}
				}break; 
				case STRFUNC : {
					if(data == NULL){
						string (*call)() = ((string(*)())(__list__)->data);
						if(flag == INPLACE) {
							__list__->type = STR;
							__list__->data = (string)call();
						}else{
							l_append(STR , (void*)call());
						}
					}else{
						string (*call)(void*) = ((string(*)(void*))(__list__)->data);
						if(flag == INPLACE) {
							__list__->type = STR;
							__list__->data = (string)call(data);
						}else{
							l_append(STR , (void*)call(data));
						}
					}
				}break;
				default :{ 	
					__list__ = (__list__)->next;
					continue;
				}
			}
			__list__ = (__list__)->next;
		}
	}else{
		while(__list__ != NULL){
			if((__list__)->index == idx){
				switch((__list__)->type){
					case STRFUNC : {
						if(data == NULL){
							string (*call)() = ((string(*)())(__list__)->data);
							if(flag == OUTT)
								return (char*)call();
							else if(flag == INPLACE) {
								__list__->type = STR;
								__list__->data = (string)call();
							}else{
								l_append(STR , (void*)call());
							}
							return NULL;
						}else{
							string (*call)(void*) = ((string(*)(void*))(__list__)->data);
							if(flag == OUTT){
								return (char*)call(data);
							}
							else if(flag == INPLACE) {
								__list__->type = STR;
								__list__->data = (char*)call(data);
							}else{
								l_append(STR , (void*)call(data));
							}
							return NULL;
						}
					}break;
					case  VOIDFUNC:{
						if(data == NULL){
							void (*call)() = (void(*)())(__list__)->data;
							call();
						}else{
							void (*call)(void*) = (void(*)(void*))(__list__)->data;
							call(data);
						}
						return NULL;
					}break; 
					default :{ 
						__list__ = (__list__)->next;
						continue;
					}
				}
			}
			__list__ = (__list__)->next;
		}
	}
	return NULL;
}

RLLOCAL void* exec(int idx , void* data , Exec_Flag flag){
	return local_exec(__list__ , idx , data , flag);
}

RLLOCAL void local_l_popidx(
		List** __list__,
		int idx
		)
{
	if(*__list__ == NULL){
		status = LIST_EMPTY;
		return;
	}
	if(idx > global_count || idx < 0)
	{
		status = LIST_INDEX_OUT_OF_RANGE;
		return;
	}
	status = FINE;
	List* ___temp = *__list__;
	List* prev = NULL;
	if (___temp != NULL && ___temp->index == idx) {
	    *__list__ = ___temp->next;
	    RLFREE(___temp);
	    ___temp = *__list__;
	    while(___temp != NULL && revesed){
	        ___temp->index--;
	        ___temp =  ___temp->next;
	    }
	    global_count--;
	    return;
	}
	while (___temp != NULL && ___temp->index != idx) {
		if(!revesed && ___temp->index > 0)
			___temp->index--;
		prev = ___temp;
		___temp = ___temp->next;
	}
	if (___temp == NULL) {
	    return;
	}
	prev->next = ___temp->next;
	RLFREE(___temp);
	___temp = prev->next;
	while(revesed && ___temp != NULL && ___temp->index > 0){
		___temp->index--;
		___temp =  ___temp->next;
	}
	global_count--;
}
RLLOCAL void l_delete(int idx)
{
	local_l_popidx(&__list__ , idx);
}

RLLOCAL void l_filter(FILTERCALLBACK callback , Type type , Filter_Flag flag){
	List* ___temp = __list__;
	while(___temp != NULL){
		if(___temp->type == type){
			if(!callback(___temp->data)){
				l_delete(___temp->index);
			}
		}else if(flag == ALL){
			l_delete(___temp->index);
		}
		___temp = ___temp->next;
	}
}

RLLOCAL void l_map(MAPCALLBACK callback , Type type){
	List* ___temp = __list__;
	while(___temp != NULL){
		if(___temp->type == type){
			___temp->data = callback(___temp->data);
		}
		___temp = ___temp->next;
	}
}

RLLOCAL void wait(RLAPIThread thread){
	RLWait(thread);
}
RLLOCAL void kill(RLAPIThread thread)
{
	RLKill(thread);
}

RLLOCAL IfaceThread exec_async(int idx , RLAPIParam data){
	List* ___temp = __list__;

	IfaceThread th = {0};

	if(___temp == NULL)
	{
		status = NULL_VALUE;
		return th;
	}

	if(idx < 0 || idx > global_count){
		status = LIST_INDEX_OUT_OF_RANGE;
		return th;
	}

	while(___temp != NULL)
	{
		if(___temp->index == idx){
			RLAPIThread thread = RLCreateThread(___temp->data , data);
			th.thread = thread;
			th.Wait = wait;
			th.Kill = kill;
			break;
		}
		___temp = ___temp->next;
	}
	return th;
}

RLLOCAL string l_geterror()
{
	switch(status){
		case LIST_INDEX_OUT_OF_RANGE : return "[ERROR] list index out of range";
		case LIST_EMPTY : return "[ERROR] list empty";
		case LIST_MEMALLOC : return "[ERROR] list allocating memory";
		case NULL_VALUE : return "[ERROR] NULL value detected";
		case ERROR_CREATE_THREAD : return "[ERROR] create thread , use GetLastError() or check errorno code ";
		case ERROR_WAIT_THREAD : return "[ERROR] wait thread , use GetLastError() or check errorno code ";
		case ERROR_KILL_THREAD : return "[ERROR] kill thread , use GetLastError() or check errorno code ";
		default : return NULL;
	}
}

RLLOCAL void* local_l_get(List* __list__ , int idx)
{
	if(__list__ == NULL){
		status = LIST_EMPTY;
		return NULL;
	}
	if(idx > global_count || idx < 0){
		status = LIST_INDEX_OUT_OF_RANGE;
		return NULL;
	}
	List* local_list = __list__;
	status = FINE;
	while(local_list != NULL){
		if((local_list)->index == idx){	
			return (local_list)->data;
			break;
		}
		local_list = (local_list)->next;
	}
	status = NULL_VALUE;
	return NULL;
}
RLLOCAL void* l_get(int idx){
	return local_l_get(__list__ , idx);
}
RLLOCAL LBOOL local_l_search(
		List* __list__,
		int* idx ,
		Type type ,
		void* data
)
{
	List* ___temp = __list__;
	if(___temp == NULL){
		status = LIST_EMPTY;
		return false;
	}
	status = FINE;
	while(___temp != NULL){
		switch(type){
			case STR :{
				if(data == ___temp->data){
					if(idx != NULL)
						*idx = ___temp->index ;
					return true;
				}
			}break;
			case CHR :
			case INTT:{
				if(*((int*)data) == (*(int*)___temp->data)){
					if(idx != NULL)
						*idx = ___temp->index ;
				 	return true;
				}
			}break;
			case FLT: {
				if(*(float*)data == *(float*)___temp->data){
					if(idx != NULL)
						*idx = ___temp->index ;
				 	return true;
				}
			}break;
			case BOOLL:{
				if(*(LBOOL*)data){
					if(idx != NULL)
						*idx = ___temp->index ;
				 	return true;
				}
			}break;
			default : {
				___temp = ___temp->next;
				continue;
			 }
		}
		___temp = ___temp->next;
	}
	return false;
}
RLLOCAL LBOOL l_search(
		int* idx ,
		Type type ,
		void* data
	     )
{
	return local_l_search(__list__ , idx , type , data);
}
RLLOCAL void local_l_reverse(List** __list__)
{
	if((*__list__) == NULL){
		status = LIST_EMPTY;
		return;
	}
	status = FINE;
	List* prev = NULL; 
	List* current = *__list__; 
	List* next = NULL; 
	while (current != NULL) { 
	    next = current->next; 
	    current->next = prev; 
	    prev = current; 
	    current = next; 
	} 
	*__list__ = prev; 
	revesed = !revesed;
}
void l_reverse(){
	local_l_reverse(&__list__);
}
void l_print()
{
	if(__list__ == NULL){	
		printf("[NULL]");
		status = LIST_EMPTY;
		return ;
	}
	List* local_list = __list__;
	status = FINE;
	//local_l_reverse(&local_list);
	printf("[\n");
	while(local_list != NULL){
		switch((local_list)->type){
			case CHR:{
				printf("\n\t[%d] '%c'",(local_list)->index  ,(*(char*)(local_list)->data) );
			}break;
			case INTT:{
				printf("\n\t[%d] %d",(local_list)->index  ,(*(int*)(local_list)->data) );
			}break;
			case FLT:{
				printf("\n\t[%d] %f",(local_list)->index  ,(*(float*)(local_list)->data) );
			}break;

			case STR:{
				printf("\n\t[%d] \"%s\"",(local_list)->index  , (char*)(local_list)->data);
			}break;
			case BOOLL:{
				printf("\n\t[%d] %s",(local_list)->index  , *(int*)(local_list)->data ? "true" : "false");
			}break;
			case VOIDPTR :{
				// printing address of the void ptr
				printf("\n\t[%d] %p",(local_list)->index , (local_list)->data);
			}break;
			case  INTFUNC:{
				printf("\n\t<int function : %p><index : %d>" ,(local_list)->data,(local_list)->index);
			}break; 
			case  CHARFUNC:{
				printf("\n\t<character function : %p><index : %d>", (local_list)->data,(local_list)->index);
			}break; 
			case  STRFUNC :{
				printf("\n\t<string function : %p><index : %d>", (local_list)->data,(local_list)->index);
			}break; 
			case  VOIDFUNC:{
				printf("\n\t<void function : %p><index : %d>" ,(local_list)->data,(local_list)->index);
			}break;

		}
		(local_list) = (local_list)->next;
		if((local_list) != NULL)	printf(",");
	}
	printf("\n]\n");
}

IfaceList list(int count , ...)
{
	init(&__list__);
	va_list args;
	va_start(args , count);

	IfaceList cl;
	global_count = count;
	int c = 0;
	while(c < count){
		Type t = va_arg(args , Type);
		switch(t){
			case CHR:{
				void* temp = va_arg(args , char*);
				add(&__list__,temp, CHR , c);
			}break;
			case INTT:{
				void* temp= va_arg(args , void*);
				add(&__list__,temp, INTT , c);
			}break;
			case STR:{
				void* temp = va_arg(args , char*);
				add(&__list__,temp , STR , c);
			}break;
			case BOOLL:{
				void* t = va_arg(args , void*);
				void* temp = *(int*)t == true ? "true" : "false";
				add(&__list__,temp, BOOLL , c);
			}break;
			case FLT:{
				void* temp = va_arg(args , void*);
				float* ttemp = (float*)temp;
				add(&__list__,ttemp , FLT , c);
			}break;
			case VOIDPTR :{
				void* temp = va_arg(args , void*);
				add(&__list__,temp , VOIDPTR , c);
			}break;
			case  VOIDFUNC:{
				VOIDFUNCTION temp = va_arg(args ,VOIDFUNCTION);
				add(&__list__,(void*)temp , VOIDFUNC , c);
			}break; 
			case  INTFUNC :{
				INTEGERFUNCTION temp = va_arg(args ,INTEGERFUNCTION);
				add(&__list__,(void*)temp , INTFUNC , c);
			}break;
			case  CHARFUNC:{
				CHARACTERFUNCTION temp = va_arg(args ,CHARACTERFUNCTION);
				add(&__list__,(void*)temp , CHARFUNC , c);
			}break;
			case  STRFUNC :{
				STRINGFUNCTION temp = va_arg(args ,STRINGFUNCTION);
				add(&__list__,(void*)temp , STRFUNC , c);
			}break; 
		}
		c++;
	}
	if(c == 0)	status = LIST_EMPTY;
	va_end(args);
#ifndef USING_LIST
	cl.Queue_Push = NULL;
	cl.Stack_Is_Empty = NULL;
	cl.Stack_Get_Error = NULL;
	cl.Stack_Push = NULL;
	cl.Stack_Peek = NULL;
	cl.Stack_Pop = NULL;
	cl.Queue_Peek = NULL;
	cl.Queue_Pop = NULL;
	cl.List_Is_Empty = l_is_empty;
	cl.List_Insert = l_insert;
	cl.List_Append = l_append;
	cl.List_Del_Index = l_delete;
	cl.List_Get = l_get;
	cl.List_Search = l_search;
	cl.List_Reverse = l_reverse;
	cl.List_Print = l_print;
	cl.List_Exec_Func = exec;
	cl.List_Clear = l_clear;
	cl.List_Get_Error = l_geterror;
	cl.List_Filter = l_filter;
	cl.List_Map = l_map;
	cl.List_Exec_Async = exec_async;
#else
	cl.Push = NULL;
	cl.SPeek = NULL;
	cl.SPop = NULL;
	cl.QPeek = NULL;
	cl.QPop = NULL;
	cl.Insert = l_insert;
	cl.Append = l_append;
	cl.Del_Index = l_delete;
	cl.Get = l_get;
	cl.Search = l_search;
	cl.Reverse = l_reverse;
	cl.Is_Empty = l_is_empty;
	cl.Print = l_print;
	cl.Exec_Func = exec;
	cl.Clear = l_clear;
	cl.Get_Error = l_geterror;
	cl.Filter = l_filter;
	cl.Map = l_map;
	cl.Exec_Async = exec_async;
#endif
	return cl;
}

IfaceList stack()
{
	init(&__list__);

	IfaceList cl;
	global_count = 0;

#ifndef USING_LIST
	cl.Queue_Push = NULL;
	cl.Stack_Is_Empty = l_is_empty;
	cl.Stack_Get_Error = l_geterror;
	cl.Stack_Push = l_push;
	cl.Stack_Peek = l_peek;
	cl.Stack_Pop = l_pop;
	cl.Stack_Clear = l_clear;
	cl.List_Exec_Async = NULL;
	cl.Queue_Peek = NULL;
	cl.Queue_Pop = NULL;
	cl.List_Insert = NULL;
	cl.List_Append = NULL;
	cl.List_Del_Index = NULL;
	cl.List_Get = NULL;
	cl.List_Search = NULL;
	cl.List_Reverse = NULL;
	cl.List_Print = NULL;
	cl.List_Exec_Func = NULL;
	cl.List_Clear = NULL;
	cl.List_Get_Error = NULL;
	cl.List_Filter = NULL;
	cl.List_Map = NULL;
#else
	cl.Is_Empty = l_is_empty;
	cl.Push = l_push;
	cl.SPeek = l_peek;
	cl.SPop = l_pop;
	cl.Clear = l_clear;
	cl.QPeek = NULL;
	cl.QPop = NULL;
	cl.Get_Error = l_geterror;
	cl.Insert = NULL;
	cl.Append = NULL;
	cl.Del_Index = NULL;
	cl.Get = NULL;
	cl.Search = NULL;
	cl.Reverse = NULL;
	cl.Print = NULL;
	cl.Exec_Func = NULL;
	cl.Filter = NULL;
	cl.Map = NULL;
	cl.Exec_Async = NULL;
#endif
	return cl;
}

IfaceList queue(void)
{
	init(&__list__);

	IfaceList cl;
	global_count = 0;

#ifndef USING_LIST
	cl.Queue_Pop = l_qpop;
	cl.Queue_Peek = l_qpeek;
	cl.Queue_Push = l_push;
	cl.Queue_Get_Error = l_geterror;
	cl.Queue_Is_Empty = l_is_empty;
	cl.Queue_Clear = l_clear;
	cl.Stack_Is_Empty = NULL;
	cl.Stack_Get_Error = NULL;
	cl.Stack_Push = NULL;
	cl.Stack_Peek = NULL;
	cl.Stack_Pop = NULL;
	cl.List_Insert = NULL;
	cl.List_Append = NULL;
	cl.List_Del_Index = NULL;
	cl.List_Get = NULL;
	cl.List_Search = NULL;
	cl.List_Reverse = NULL;
	cl.List_Print = NULL;
	cl.List_Exec_Func = NULL;
	cl.List_Clear = NULL;
	cl.List_Get_Error = NULL;
	cl.List_Filter = NULL;
	cl.List_Map = NULL;
	cl.List_Exec_Async = NULL;
#else
	cl.QPop = l_qpop;
	cl.QPeek = l_qpeek;
	cl.Is_Empty = l_is_empty;
	cl.Push = l_push;
	cl.Clear = l_clear;
	cl.SPeek = NULL;
	cl.SPop = NULL;
	cl.Get_Error = l_geterror;
	cl.Insert = NULL;
	cl.Append = NULL;
	cl.Del_Index = NULL;
	cl.Get = NULL;
	cl.Search = NULL;
	cl.Reverse = NULL;
	cl.Print = NULL;
	cl.Exec_Func = NULL;
	cl.Filter = NULL;
	cl.Map = NULL;
	cl.Exec_Async = NULL;
#endif
	return cl;
}

#endif /* LIST_C */
