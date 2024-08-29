/************************************************************************************************
*			Copyright (c) 2023 Ray Den						*
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


#ifndef LIST_H
#define LIST_H

// TODO: handle call back functions type

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
// we don't need stdbool lib
typedef enum{
	false,
	true = !(false)
}bool;

// Type enum contained bunch of types helps functions any variables type will work for
// STR for strings BOOL for boolean ... 
typedef enum{
	CHR = 0, 	// char type
	STR    , 	// string type
	BOOL   ,	// bool type
	INT    , 	// int type
	FLT    , 	// float type
	VOIDPTR,	// void pointer type

	VOIDFUNC,
	INTFUNC ,
	CHARFUNC,
	STRFUNC
}Type;

typedef char* string;

/*
 *	Linked List struct
 *		index : index of single list
 *		type  : type of the data memory saved in void* data pointer
 *		data  : the place we store the address of our variables 
 *		next  : pointing to next list
 * */

typedef void 	(*VOIDFUNCTION		)(void);
typedef int  	(*INTEGERFUNCTION  	)(void);
typedef bool 	(*BOOLEANFUNCTION  	)(void);
typedef char 	(*CHARACTERFUNCTION	)(void);
typedef string 	(*STRINGFUNCTION 	)(void);

typedef struct list {
	int 		index;
	Type 		type;
	void* 		data;
	struct list* 	next;
}List;

// Global list variable 
List* __list__ = NULL;

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
	bool (*List_Search)(
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
	void (*List_Reverse)(void);
	/*
	 *	List_Print function will print all our data in standerd output 
	 * */
	void (*List_Print)(void);

	// TODO : Impelemnt list_exec_func 
	void* (*List_Exec_Func)(
			int
	);

	// clear list
	void (*List_Clear)(void);
}Class_List;

// global count for index
int global_count = 0;

void add(void* val , Type t, int idx){
	List* temp = malloc(sizeof(List));
	temp->data = val;               
	temp->type = t;                 
	temp->index = idx;              
	temp->next = __list__;                
	__list__ = temp;                      
}

Class_List list(
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);

void l_clear(void);

void* exec( 
		int // index 
);

void l_append(
		Type type,
		void* data
);
//void l_insert();
//void l_popval(
//		List** _list,
//		void* data,
//		Type type
//		);
void l_popidx(
		int idx
);
void* l_get(
		int idx
);
bool l_search(
		int*,
		Type,
		void*
);
void l_reverse(void);
void l_print(void);
#endif

#ifdef LIST_C

Class_List list(int count , ...)
{
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
				add(temp, CHR , c);
			}break;
			case INT:{
				 // TODO: handle integer
				void* temp= va_arg(args , void*);
				add(temp, INT , c);
			}break;
			case STR:{
				void* temp = va_arg(args , char*);
				add(temp , STR , c);
			}break;
			case BOOL:{
				int t = va_arg(args , int);
				void* temp = t ? "true" : "false";
				add(temp, BOOL , c);
			}break;
			case FLT:{
				// we need to include stdint.h lib
				//float f = va_arg(args , float);
				//void *temp = (void *)(uintptr_t)*(uint32_t *)&f;
				//float f2 = *(float *)&temp;
				void* temp = va_arg(args , void*);
				add(temp , FLT , c);
			}break;
			// TODO: handle other types
			case VOIDPTR :{
				void* temp = va_arg(args , void*);
				add(temp , VOIDPTR , c);
			}
			case  VOIDFUNC:{
				VOIDFUNCTION temp = va_arg(args ,VOIDFUNCTION);
				add((void*)temp , VOIDFUNC , c);
			}break; 
			case  INTFUNC :{
				INTEGERFUNCTION* temp = va_arg(args ,INTEGERFUNCTION);
				add((void*)temp , INTFUNC , c);
			}break;
			case  CHARFUNC:{
				CHARACTERFUNCTION* temp = va_arg(args ,CHARACTERFUNCTION);
				add((void*)temp , CHARFUNC , c);
			}break;
			case  STRFUNC :{
				STRINGFUNCTION* temp = va_arg(args ,STRINGFUNCTION);
				add((void*)temp , STRFUNC , c);
			}break; 
		}
		c++;
	}
	va_end(args);
	cl.List_Append = l_append;
	cl.List_Del_Index = l_popidx;
	cl.List_Get = l_get;
	cl.List_Search = l_search;
	cl.List_Reverse = l_reverse;
	cl.List_Print = l_print;
	cl.List_Exec_Func = exec;
	cl.List_Clear = l_clear;
	return cl;
}
void l_append(
		Type type,
		void* data
)
{
	add(data, type , global_count++);
}
void l_clear(void){
	__list__ = NULL;
}
void* exec(int idx)
{
	if(idx > global_count)	return NULL;
	if(idx < 0){
		while(__list__ != NULL){
			switch(__list__->type){
				//case  INTFUNC :
				//case  CHARFUNC:
				//case  STRFUNC : 
				case  VOIDFUNC:{
					void (*call)() = (void(*)())__list__->data;
					call();
				}break; 
			}
			__list__ = __list__->next;
		}
	}else{
		while(__list__ != NULL){
			if(__list__->index == idx){
				switch(__list__->type){
					case  VOIDFUNC:{
						void (*call)() = (void(*)())__list__->data;
						call();
						return NULL;
					}break; 
					case  CHARFUNC:
					case  INTFUNC :{
						int* (*call)() = (int*(*)()) __list__->data();
						return call();
					}break;
					case  STRFUNC :{
						char* (*call)() = (char*(*)()) __list__->data();
						return call();
					}break; 
				}
			}
			__list__ = __list__->next;
		}
	}
}
//void l_insert(void);
void l_popval(
		List** _list,
		void* data,
		Type type
		)
{
	if(*_list == NULL)
		return ;
	switch(type){
		case BOOL:
		case INT:
		case CHR:
		case STR :{
			if(strcmp((*_list)->data , data) == 0){
				List* tmp = *_list;
				*_list = (*_list)->next;
				free(tmp);
			}
			l_popval(&(*_list)->next , data , type);
		}break;
		default : {
			return ;
		}
	}
}
void l_popidx(
		int idx
		)
{
	List* ___temp = __list__;
	if((___temp == NULL) || idx < 0)	return;
	if((___temp)->index == idx){
		List* tmp = ___temp;
		___temp = ___temp->next;
		free(tmp);
		return;
	}
	___temp = ___temp->next;
	l_popidx(idx);
}
void* l_get(int idx)
{
	if(idx > global_count)	return NULL;
	while(__list__ != NULL){
		if(__list__->index == idx)	return __list__->data;
		__list__ = __list__->next;
	}
}
bool l_search(
		int* idx ,
		Type type ,
		void* data
)
{
	List* ___temp = __list__;
	if(___temp == NULL)
		return false;
	while(___temp != NULL){
		switch(type){
			case STR :
			case CHR :{
				if(data == __list__->data  ){
					*idx = ___temp->index ;
					return true;
				}
			}break;
			case INT:{
				if(*((int*)data) == (*(int*)___temp->data)){
					*idx = ___temp->index ;
				 	return true;
				}
			}break;
			case BOOL:{
				if(*(bool*)data){
					*idx = ___temp->index ;
				 	return true;
				}
			}break;
		}
		___temp = ___temp->next;
	}
}
void l_reverse()
{
    List* prev = NULL; 
    List* current = __list__; 
    List* next = NULL; 
    while (current != NULL) { 
        next = current->next; 
        current->next = prev; 
        prev = current; 
        current = next; 
    } 
    __list__ = prev; 
}
void l_print()
{
	List* ___temp = __list__;
	if(___temp == NULL)	printf("list: NULL");
	l_reverse();
	printf("[\n");
	while(___temp != NULL){
		switch(___temp->type){
			case CHR:{
				printf("\n\t'%s'" ,(___temp->data) );
			}break;
			case INT:{
				printf("\n\t%d" , *(int*)(___temp->data));
			}break;
			case STR:{
				printf("\n\t\"%s\"" , ___temp->data);
			}break;
			case BOOL:{
				printf("\n\t%s" , (___temp->data == "true") ? "true" : "false");
			}break;
			case FLT :{
				printf("\n\t%f" , *((float*)___temp->data));
			}break;
			case VOIDPTR :{
				// printing address of the void ptr
				printf("\n\t%p" , ___temp->data);
			}break;
			case  INTFUNC:{
				printf("\n\t<int function : %p><index : %d>" ,___temp->data,___temp->index);
			}break; 
			case  CHARFUNC:{
				printf("\n\t<character function : %p><index : %d>", ___temp->data,___temp->index);
			}break; 
			case  STRFUNC :{
				printf("\n\t<string function : %p><index : %d>", ___temp->data,___temp->index);
			}break; 
			case  VOIDFUNC:{
				printf("\n\t<void function : %p><index : %d>" ,___temp->data,___temp->index);
			}break;

		}
		___temp = ___temp->next;
		if(___temp != NULL)	printf(",");
	}
	printf("\n]");
}
#endif
