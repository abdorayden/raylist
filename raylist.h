/************************************************************************************************
*			Copyright (c) 2023 Ray Den	raylist v1.1.0				*
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

// TODO : add filter 	function
// TODO : add map	function
// TODO : add insert	function
// TODO : add thread	function 
// TODO : add thread	type
// TODO : add sort	function

// TODO : handle return value of the function in exec function

#ifndef LIST_H
#define LIST_H

// to check if raylist is included
#define LIST_INCLUDED

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

// boolean enum
#ifndef LBOOL

typedef enum{
	false,
	true = !(false)
}bool;
#define LBOOL bool

#endif


#ifndef LALLOC
#define LALLOC	malloc
#endif

// Type enum contained bunch of types helps functions any variables type will work for
// STR for strings BOOL for boolean ... 
typedef enum{
	CHR = 0, 	// char type
	STR    , 	// string type
	BOOL   ,	// bool type
	INT    , 	// int type
	FLT    , 	// float type
	
	VCHR    , 	// variable char type
	VINT    , 	// variable int type
	VFLT    , 	// variable float type

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
	LIST_EMPTY
}ListError;

static ListError status = FINE;

typedef char* string;

typedef void 	(*VOIDFUNCTION		)(void);
typedef int  	(*INTEGERFUNCTION  	)(void);
typedef LBOOL 	(*BOOLEANFUNCTION  	)(void);
typedef char 	(*CHARACTERFUNCTION	)(void);
typedef string 	(*STRINGFUNCTION 	)(void);

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

// unused
#if 0

// TODO: local for better solution in return data type
typedef union {
	int    retint;
	char   retchar;
	float  retfloat;
	LBOOL   retbool;
	string retstring;
	void*  retnull;
}RetType;

#endif
// global count for index
static int global_count = 0;

// Global list variable 
static List* __list__ = NULL;

// filter callback function 
typedef enum{
	ALL,  	// delete all types 
	ONLY	// only type
}Filter_Flag;

typedef LBOOL (*FILTERCALLBACK)(void*);

/*
 *	Class_List struct act like class all thouse functions point to other functions 
 *
 *	Example :
 *		void foo(void){
 *			printf("bar");
 *		}
 *		Class_List test;
 *		test.List_Append = foo;
 *		
 * */
typedef struct {
#ifndef USING_LIST
	/*
	 *	List_Append function will take the data and stored to the __list__ global variable 
	 *
	 * 	List_Append(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
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
	 *	my_list.List_Filter(check_mod_of_num , INT);
	 *					after : 	[
	 *								2
	 *							]
	 * */
	void (*List_Filter)(
			FILTERCALLBACK,
			Type
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

	void* (*List_Exec_Func)(
			int idx
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
#else
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
	 *	my_list.List_Filter(check_mod_of_num , INT);
	 *					after : 	[
	 *								2
	 *							]
	 * */
	void (*Filter)(
			FILTERCALLBACK,
			Type
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

	void* (*Exec_Func)(
			int idx
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
#endif
}Class_List;

Class_List list(
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);

void	l_filter(FILTERCALLBACK callback , Type type);

void l_clear(
		void
		);

void* exec( 
		int idx	// index 
);

void l_append(
		Type type,
		void* data
);
void l_delete(
		int idx
);
string l_geterror(
		void
);
void* l_get(
		int idx
);
LBOOL l_search(
		int*,
		Type,
		void*
);
void l_reverse(
		void
		);
void l_print(
		void
		);
#endif

#ifdef LIST_C

static void add_voidptr(
		List** __list__ , 
		void* val , 
		Type t, 
		int idx
){
	List* temp = LALLOC(sizeof(List));
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
static void add_int(
		List** __list__ , 
		int val , 
		Type t, 
		int idx
){
	List* temp = LALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = LALLOC(sizeof(int));
	*(int*)temp->data = val;
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

static void add_char(
		List** __list__ , 
		char val , 
		Type t, 
		int idx
){
	List* temp = LALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = LALLOC(sizeof(char));
	*(char*)temp->data = val;
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

static void add_float(
		List** __list__ , 
		float val , 
		Type t, 
		int idx
){
	List* temp = LALLOC(sizeof(List));
	if(temp == NULL)
	{
		status = LIST_MEMALLOC;
		return;
	}
	temp->data = LALLOC(sizeof(float));
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

static void init(List** l){
	*l = NULL; 
}
Class_List list(int count , ...)
{
	init(&__list__);
	va_list args;
	va_start(args , count);

	Class_List cl;
	global_count = count;
	size_t c = 0;
	while(c < count){
		Type t = va_arg(args , Type);
		switch(t){
			case CHR:{
				void* temp = va_arg(args , char*);
				add(&__list__,temp, CHR , c);
			}break;
			case INT:{
				void* temp= va_arg(args , void*);
				add(&__list__,temp, INT , c);
			}break;
			case STR:{
				void* temp = va_arg(args , char*);
				add(&__list__,temp , STR , c);
			}break;
			case BOOL:{
				int t = va_arg(args , int);
				void* temp = t ? "true" : "false";
				add(&__list__,temp, BOOL , c);
			}break;
			case FLT:{
				void* temp = va_arg(args , void*);
				float* ttemp = (float*)temp;
				add(&__list__,ttemp , FLT , c);
			}break;
			case VOIDPTR :{
				void* temp = va_arg(args , void*);
				add(&__list__,temp , VOIDPTR , c);
			}
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
#else
	cl.Append = l_append;
	cl.Del_Index = l_delete;
	cl.Get = l_get;
	cl.Search = l_search;
	cl.Reverse = l_reverse;
	cl.Print = l_print;
	cl.Exec_Func = exec;
	cl.Clear = l_clear;
	cl.Get_Error = l_geterror;
	cl.Filter = l_filter;
#endif
	return cl;
}

#define LAppend(t , d) _Generic((d) , 		\
			int : add_int,		\
			char : add_char,	\
			float : add_float	\
		)(&__list__ , d , t  , global_count++);

void l_append(
		Type type,
		void* data
)
{
	if(status != FINE) status = FINE;
	add(&__list__ , data, type , global_count++);
}
void l_clear(){
	List* current = __list__;
	List* temp;
	while (current != NULL) {
		temp = current->next; 
		free(current);        
		current = temp;       
	}
	init(&__list__);
}
static void* local_exec(List* __list__ ,int idx)
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
	if(idx < 0){
		while(__list__ != NULL){
			switch((__list__)->type){
				case  VOIDFUNC:{
					void (*call)() = (void(*)())(__list__)->data;
					call();
				}break; 
			}
			__list__ = (__list__)->next;
		}
	}else{
		while(__list__ != NULL){
			if((__list__)->index == idx){
				switch((__list__)->type){
					case STRFUNC : {
						string (*call)() = ((string(*)())(__list__)->data);
						return (char*)call();
					}break;
					case  VOIDFUNC:{
						void (*call)() = (void(*)())(__list__)->data;
						call();
						return NULL;
					}break; 
				}
			}
			__list__ = (__list__)->next;
		}
	}
	return NULL;
}

void* exec(int idx){
	return local_exec(__list__ , idx);
}
static void local_l_popidx(
		List** __list__,
		int idx
		)
{
	List* ___temp = *__list__;
	if(___temp == NULL){
		status = LIST_EMPTY;
		return;
	}
	if(idx > global_count || idx < 0)
	{
		status = LIST_INDEX_OUT_OF_RANGE;
		return;
	}
	status = FINE;
	while(___temp != NULL){
		if((___temp)->index == idx){
			List* tmp = ___temp;
			if(___temp->next != NULL)
				___temp = ___temp->next;
			free(tmp);
			global_count--;
		}
		else if ((___temp)->index > idx)	(___temp)->index--;
		___temp = ___temp->next;
	}
}
void l_delete(int idx)
{
	local_l_popidx(&__list__ , idx);
}

void l_filter(FILTERCALLBACK callback , Type type){
	List* ___temp = __list__;
	while(___temp != NULL){
		if(___temp->type == type){
			if(!callback(___temp->data)){
				l_delete(___temp->index);
			}
		}else{
			l_delete(___temp->index);
		}
		___temp = ___temp->next;
	}
}

string l_geterror()
{
	switch(status){
		case LIST_INDEX_OUT_OF_RANGE : return "[ERROR] list index out of range";
		case LIST_EMPTY : return "[ERROR] list empty";
		case LIST_MEMALLOC : return "[ERROR] list allocating memory";
		default : return NULL;
	}
}

static void* local_l_get(List* __list__ , int idx)
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
}
void* l_get(int idx){
	return local_l_get(__list__ , idx);
}
static LBOOL local_l_search(
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
					*idx = ___temp->index ;
					return true;
				}
			}break;
			case CHR :
			case INT:{
				if(*((int*)data) == (*(int*)___temp->data)){
					*idx = ___temp->index ;
				 	return true;
				}
			}break;
			case FLT: {
				if(*(float*)data == *(float*)___temp->data){
					*idx = ___temp->index ;
				 	return true;
				}
			}break;
			case BOOL:{
				if(*(LBOOL*)data){
					*idx = ___temp->index ;
				 	return true;
				}
			}break;
		}
		___temp = ___temp->next;
	}
}
LBOOL l_search(
		int* idx ,
		Type type ,
		void* data
	     )
{
	return local_l_search(__list__ , idx , type , data);
}
static void local_l_reverse(List** __list__)
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
	local_l_reverse(&local_list);
	printf("[\n");
	while(local_list != NULL){
		switch((local_list)->type){
			case CHR:{
				printf("\n\t[%d] '%c'",(local_list)->index  ,(*(char*)(local_list)->data) );
			}break;
			case INT:{
				printf("\n\t[%d] %d",(local_list)->index  ,(*(int*)(local_list)->data) );
			}break;
			case FLT:{
				printf("\n\t[%d] %f",(local_list)->index  ,(*(float*)(local_list)->data) );
			}break;

			case VCHR:{
				printf("\n\t[%d] '%c'",(local_list)->index  ,*(char*)((local_list)->data) );
			}break;
			case VINT:{
				printf("\n\t[%d] %d",(local_list)->index  , *(int*)((local_list)->data));
			}break;
			case STR:{
				printf("\n\t[%d] \"%s\"",(local_list)->index  , (local_list)->data);
			}break;
			case BOOL:{
				printf("\n\t[%d] %s",(local_list)->index  , ((local_list)->data == "true") ? "true" : "false");
			}break;
			case VFLT :{
				printf("\n\t[%d] %f",(local_list)->index  , *((float*)(local_list)->data));
			}break;
			//case VOIDPTR :{
			//	// printing address of the void ptr
			//	printf("\n\t[%d] %p",(local_list)->index , (local_list)->data);
			//}break;
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
#endif
