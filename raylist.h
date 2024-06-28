
// Header Only Liberary in C

/************************************
 *	EXAMPLE:
 *		list(3,
 *	     INT,23,
 *	     STR , "Hello",
 *	     STR , "world"
 *		);           
 *	   list(5, 
 *	     CHR, 35, 
 *	     STR, "Hello",
 *	     STR, "world",
 *	     BOOL , false    
 *	     	    );  
 *
 ************************************/ 

#ifndef LIST_H
#define LIST_H

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

typedef enum{
	false,
	true = !(false)
}bool;

typedef enum{
	CHR = 0, 	// char type
	STR    , 	// string type
	BOOL   ,	// bool type

	INT    , 	// int type
	INTPTR ,	// int pointer type

	FLT    , 	// float type
	FLTPTR , 	// float pointer type

	DBL    ,	// double type
	DBLPTR ,	// double pointer type

	VOIDPTR,	// void pointer type
}Type;

typedef struct list {
	int index;
	Type type;
	void* data;
	struct list* next;
}List;

// Global list variable 
List* __list__ = NULL;

typedef struct {
	void (*List_Append)(
			Type type,
			void* data	// the data
	);
	//void (*List_Insert)(void);
	//void (*List_Pop_Val)(
	//		List** _list,
	//		void* data,
	//		Type type
	//		);
	void (*List_Pop_Index)(
			int idx // index value to delete
	);
	void (*List_Get)(
			int idx
	);
	void (*List_Reverse)(void);
	void* (*List_Index)(
		void* data, // if data in NULL function will return the data at the parameter index else will ignore idx
		int idx  
	);
	void (*List_Print)(void);
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
		List** _list,
		int idx
);
void l_get(
		int idx
);
void l_reverse(
		List** _list
);
void* l_index(
		List* _list,
		void* data, // if data in NULL function will return the data at the parameter index else will ignore idx
		Type* type, // return type pointer 
		int idx  
);
void l_print(
		List* _list
);
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
				char t = (char)va_arg(args , char);
				void* temp = &t;
				add(temp, CHR , c);
			}break;
			case INT:{
				int t = (int)va_arg(args , int);
				void* temp = &t;
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
				float f = va_arg(args , float);
				void *temp = (void *)(uintptr_t)*(uint32_t *)&f;
				//float f2 = *(float *)&p;
				add(temp , FLT , c);
			}break;
			case FLTPTR :{
				void* temp = va_arg(args , float*);
				add(temp , FLTPTR , c);
			}break;
			case INTPTR :{
				void* temp = va_arg(args , int*);
				add(temp , INTPTR , c);
			}break;
			case DBLPTR :{
				void* temp = va_arg(args , double*);
				add(temp , DBLPTR , c);
			}break;
			case DBL :{
				assert(false && "No Implemented yet");
				//void* temp = va_arg(args , double*);
				//add(temp , DBLPTR , c);
			}break;
			case VOIDPTR :{
				void* temp = va_arg(args , void*);
				add(temp , DBLPTR , c);
			}
		}
		c++;
	}
	va_end(args);
	cl.List_Append = l_append;
	//cl.List_Insert = l_insert;
	//cl.List_Pop_Val = l_popval;
	cl.List_Pop_Index = l_popidx;
	cl.List_Get = l_get;
	cl.List_Reverse = l_reverse;
	cl.List_Index = l_index;
	cl.List_Print = l_print;
	return cl;
}
void l_append(
		Type type,
		void* data
)
{
	add(data, type , global_count++);
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
		List** _list,
		int idx
		)
{
	if((*_list == NULL) || idx < 0)	return;
	if((*_list)->index == idx){
		List* tmp = *_list;
		*_list = (*_list)->next;
		free(tmp);
	}
	l_popidx(&(*_list)->next , idx);
}
void l_get(int idx){}
void l_reverse(
		List** _list
)
{
    List* prev = NULL; 
    List* current = *_list; 
    List* next = NULL; 
    while (current != NULL) { 
        next = current->next; 
        current->next = prev; 
        prev = current; 
        current = next; 
    } 
    *_list = prev; 
}
void* l_index(
		List* _list,
		void* data, // if data in NULL function will return the data at the parameter index else will ignore idx
		Type* type, // return type pointer 
		int idx  
)
{
	if(_list == NULL)	return NULL;
	if((idx >= 0) && (data == NULL)){
		while(_list != NULL){
			if(_list->index == idx){
				*type = _list->type;
				return _list->data;
			}
		}
	}
	if(data != NULL){
		while(_list != NULL){
			if(_list->data == data){
				*type = _list->type;
				return &_list->index;
			}
		}
	}
	return NULL;
}
//void pr(List* l)
//{
//	while(l != NULL){
//		if(strlen(l->data) == 1)
//			printf("%d,%d\n" , *((int*)l->data), l->index);
//		else
//			printf("%s,%d\n" , l->data, l->index);
//		l = l->next;
//	}
//}
void l_print(
		List* _list
)
{
	printf("printing");
	if(_list == NULL)	printf("list: NULL");
	l_reverse(&_list);
	printf("[");
	while(_list != NULL){
		switch(_list->type){
			case CHR:{
				printf("'%c'" , *((int*)_list->data));
			}break;
			case INT:{
				printf("%d" , *((int*)_list->data));
			}break;
			case STR:{
				printf("\"%s\"" , _list->data);
			}break;
			case BOOL:{
				printf("%s" , (_list->data == "true") ? "true" : "false");
			}break;
		}
		_list = _list->next;
		if(_list != NULL)	printf(",");
	}
	printf("]");
}
#endif
