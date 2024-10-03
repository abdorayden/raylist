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

typedef union {
	int    retint;
	char   retchar;
	float  retfloat;
	bool   retbool;
	string retstring;
	void*  retnull;
}RetType;

// Global list variable 
//List* __list__ = NULL;

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
			List** __list__,
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
			List** __list__,
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
			List** __list__,
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
			List** __list__,
			int idx
	);

	/*
	 *	List_Reverse function will reverse the list 
	 *
	 * */
	void (*List_Reverse)(
			List** __list__
	);
	/*
	 *	List_Print function will print all our data in standerd output 
	 * */
	void (*List_Print)(
			List* __list__
	);

	// TODO : Impelemnt list_exec_func 
	void* (*List_Exec_Func)(
			List* __list__,
			int
	);

	// clear list
	void (*List_Clear)(
			List* __list__
	);
}Class_List;

// global count for index
int global_count = 0;

void add(List** __list__ , void* val , Type t, int idx){
	List* temp = malloc(sizeof(List));
	temp->data = val;               
	temp->type = t;                 
	temp->index = idx;              
	temp->next = *__list__;                
	*__list__ = temp;                      
}

Class_List list(
		List** __list__,
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);

void l_clear(List** __list__);

RetType exec( 
		List* __list__ ,
		int // index 
);

void l_append(
		List** __list__ ,
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
		List** __list__ ,
		int idx
);
RetType l_get(
		List* __list__ ,
		int idx
);
bool l_search(
		List* __list__ ,
		int*,
		Type,
		void*
);
void l_reverse(List** __list__);
void l_print(List* __list__);
#endif

#ifdef LIST_C

Class_List list(List** __list__ , int count , ...)
{
	*__list__ = NULL; 
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
				add(__list__,temp, CHR , c);
			}break;
			case INT:{
				 // TODO: handle integer
				void* temp= va_arg(args , void*);
				add(__list__,temp, INT , c);
			}break;
			case STR:{
				void* temp = va_arg(args , char*);
				add(__list__,temp , STR , c);
			}break;
			case BOOL:{
				int t = va_arg(args , int);
				void* temp = t ? "true" : "false";
				add(__list__,temp, BOOL , c);
			}break;
			case FLT:{
				// we need to include stdint.h lib
				//float f = va_arg(args , float);
				//void *temp = (void *)(uintptr_t)*(uint32_t *)&f;
				//float f2 = *(float *)&temp;
				void* temp = va_arg(args , void*);
				add(__list__,temp , FLT , c);
			}break;
			// TODO: handle other types
			case VOIDPTR :{
				void* temp = va_arg(args , void*);
				add(__list__,temp , VOIDPTR , c);
			}
			case  VOIDFUNC:{
				VOIDFUNCTION temp = va_arg(args ,VOIDFUNCTION);
				add(__list__,(void*)temp , VOIDFUNC , c);
			}break; 
			case  INTFUNC :{
				INTEGERFUNCTION temp = va_arg(args ,INTEGERFUNCTION);
				add(__list__,(void*)temp , INTFUNC , c);
			}break;
			case  CHARFUNC:{
				CHARACTERFUNCTION temp = va_arg(args ,CHARACTERFUNCTION);
				add(__list__,(void*)temp , CHARFUNC , c);
			}break;
			case  STRFUNC :{
				STRINGFUNCTION temp = va_arg(args ,STRINGFUNCTION);
				add(__list__,(void*)temp , STRFUNC , c);
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
		List** __list__ ,
		Type type,
		void* data
)
{
	add(__list__ , data, type , global_count++);
}
void l_clear(List** __list__){
	*__list__ = NULL;
}
RetType exec(List* __list__ , int idx)
{
	RetType t;
	if(idx > global_count){	
		t.retnull = NULL;
		return t;
	}
	if(idx < 0){
		while(__list__ != NULL){
			switch((__list__)->type){
				case  INTFUNC :{
					int(*call)() = (int(*)())(__list__)->data;
					t.retint = call();
				}break;
				case  CHARFUNC:
				case  STRFUNC : 
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
					case  VOIDFUNC:{
						void (*call)() = (void(*)())(__list__)->data;
						call();
						t.retnull = NULL;
					}break; 
					case  INTFUNC :{
						int(*call)() = (int(*)())(__list__)->data;
						t.retint = call();
					}break;
					case  CHARFUNC:
					case  STRFUNC :{
						string (*call)() = (string(*)())(__list__)->data;
						t.retstring = call();
					}break; 
				}
			}
			__list__ = (__list__)->next;
		}
	}
	return t;
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
		List** __list__ ,
		int idx
		)
{
	List* ___temp = *__list__;
	if((___temp == NULL) || idx < 0 || idx > global_count)	return;
	if((___temp)->index == idx){
		List* tmp = ___temp;
		___temp = ___temp->next;
		free(tmp);
		return;
	}
	___temp = ___temp->next;
	l_popidx(__list__ , idx);
}
RetType l_get(List* __list__ , int idx)
{
	RetType t;
	if(idx > global_count){
		t.retnull = NULL;
		return t;
	}
	while(__list__ != NULL || idx < 0 || idx > global_count){
		if((__list__)->index == idx){	
			switch((__list__)->type){
				case STR :
				case CHR :{
					t.retstring = (__list__)->data;
				}break;
				case INT:{
					t.retint = *(int*)(__list__)->data;
				}break;
				case BOOL:{
					t.retbool = *(int*)(__list__)->data;
				}break;
			}
		
			break;
		}
		__list__ = (__list__)->next;
	}
	return t;
}
bool l_search(
		List* __list__ ,
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
				if(data == ___temp->data){
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
void l_reverse(List** __list__)
{
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
void l_print(List* __list__)
{
	if(__list__ == NULL){	
		printf("list: NULL");
		return ;
	}
	l_reverse(&__list__);
	printf("[\n");
	while(__list__ != NULL){
		switch(__list__->type){
			case CHR:{
				printf("\n\t'%s'" ,(__list__->data) );
			}break;
			case INT:{
				printf("\n\t%d" , *(int*)(__list__->data));
			}break;
			case STR:{
				printf("\n\t\"%s\"" , __list__->data);
			}break;
			case BOOL:{
				printf("\n\t%s" , (__list__->data == "true") ? "true" : "false");
			}break;
			case FLT :{
				printf("\n\t%f" , *((float*)__list__->data));
			}break;
			case VOIDPTR :{
				// printing address of the void ptr
				printf("\n\t%p" , __list__->data);
			}break;
			case  INTFUNC:{
				printf("\n\t<int function : %p><index : %d>" ,__list__->data,__list__->index);
			}break; 
			case  CHARFUNC:{
				printf("\n\t<character function : %p><index : %d>", __list__->data,__list__->index);
			}break; 
			case  STRFUNC :{
				printf("\n\t<string function : %p><index : %d>", __list__->data,__list__->index);
			}break; 
			case  VOIDFUNC:{
				printf("\n\t<void function : %p><index : %d>" ,__list__->data,__list__->index);
			}break;

		}
		__list__ = __list__->next;
		if(__list__ != NULL)	printf(",");
	}
	printf("\n]\n");
}
#endif
