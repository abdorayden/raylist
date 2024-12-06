# Dynamic List with Diffrent data type in C

<p align="center">
  <img src="./assets/logo.jpeg" width="350" height="350"/>
</p>

## This is a C code that implements a linked list data structure with a variety of functions to manipulate and interact with the list and stack and queue all these data structures implements IfaceList interface. Here's a breakdown of the code:

### methods available in stack
## Stack_Is_Empty();
```c
	/*
	 *	return true if stack is empty else false
	 * */
	LBOOL (*Stack_Is_Empty)(
			void
	);
```
## Stack_Get_Error();
```c
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*Stack_Get_Error)(
			void
	);
```
## Stack_Push(Type type , void* data);
```c
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
```
## Stack_Peek();
```c
	/*
	 *	Peek function return last value of the Stack (Stack)
	 *	Example : 
	 *		void* value = my_list.Stack_Peek();
	 * */
	void* (*Stack_Peek)(void);
```
## Stack_Pop();
```c
	/*
	 *	Stack_Pop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.Stack_Pop();
	 * */
	void* (*Stack_Pop)(void);
```
## Stack_Clear();
```c
	// clear Stack
	void (*Stack_Clear)(
			void
	);
```
### methods available in queue
## Queue_Is_Empty();
```c
	/*
	 *	return true if queue is empty else false
	 * */
	LBOOL (*Queue_Is_Empty)(
			void
	);
```
## Queue_Get_Error();
```c
	/*
	 *  return String if the error is set true (status > 0)
	 * */ 
	string (*Queue_Get_Error)(
			void
	);
```
## Queue_Push(Type type , void* data);
```c
	/*
	 *	Queue_Push function will take the data and stored to the __list__ global variable 
	 *
	 * 	Queue_Push(
	 *		Type : the data type
	 *		void*: data pointer point address of stored data
	 * 	)
	 *	
	 *	NOTE : the append method works like push in queue algorithm
	 *
	 *	Example :
	 *		my_list.Queue_Push(STR , "Hello World");
	 *
	 * */
	void (*Queue_Push)(
			Type type,
			void* data
	);
```
## Queue_Peek();
```c
	/*
	 *	Peek function return last value of the Queue (Queue)
	 *	Example : 
	 *		void* value = my_list.Queue_Peek();
	 * */
	void* (*Queue_Peek)(void);
```
## Queue_Pop();
```c
	/*
	 *	Queue_Pop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.Queue_Pop();
	 * */
	void* (*Queue_Pop)(void);
```
## Queue_Clear();
```c
	// clear Queue
	void (*Queue_Clear)(
			void
	);
```
### methods available in list

## list(int count, ...): 
```c
IfaceList list(
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);
```
<<<<<<< HEAD

=======
## List_Pop(): 
```c
	/*
	 *	List_Pop function will pop the last value from the list
	 *
	 *	Example : 
	 *		void* value = my_list.List_Pop();
	 * */
	void* (*List_Pop)(void);
```
## List_Peek(): 
```c
	/*
	 *	Peek function return last value of the List (Stack)
	 *	Example : 
	 *		void* value = my_list.List_Peek();
	 * */
	void* (*List_Peek)(void);
```
>>>>>>> origin/master
## List_Insert(int idx , Type type , void* data)
```c
	/*
	 *	List_Insert function will take the data and insert at the idx variable it in __list__ global linked list variable
	 *
	 *	List_Insert(
	 *		int idx		: 		index where we insert the data
	 *		Type type	: 		data type
	 *		void* data the  :		data
	 *	)
	 *	Exemple :
	 *		my_list.List_Insert(1 , BOOL , true);
	 * */
	void (*List_Insert)(
			int idx,
			Type type,
			void* data
	);
```

## List_Filter(FILTERCALLBACK callback , Type type)
```c
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
```

## List_Append(Type type, void* data): 
```c
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
```
## List_Del_Index(List** _list, int idx): 
```c
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
```
## List_Get(int idx): 
```c
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
```
## List_Exec_Func(int idx , void* data);
```c
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
			void* data
	);
```
## List_Search(int* idx,Type type, void* data): 
```c
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
```
## List_Reverse(void): 
```c
	/*
	 *	List_Reverse function will reverse the list 
	 *
	 * */
	void (*List_Reverse)(
			void
	);
```
## List_Print(void): 
```c
	/*
	 *	List_Print function will print all our data in standerd output 
	 * */
	void (*List_Print)(
			void
	);
```

Overall, this code provides a basic implementation of a linked list data structure with various functions to manipulate and interact with the list.

## Example
```c
// check example.c File
int v = 7;
float p = 3.14;
void* ptr = &v;
IfaceList my_list = list(
            7,
            STR , "Hello",
            VCHR , "r",
            BOOL ,true,
            VINT , (void*)&v,
            VFLT , (void*)&p,
            VOIDPTR , ptr,
            STR , "World"
        );
my_list.List_Reverse();
my_list.List_Append(STR , "RayDen");
my_list.List_Append(BOOL , false);
my_list.List_Print();
```
## Out Put 
```console
 [
 
 	"World",
 	0x7ffd5c982d1c,
 	3.140000,
 	7
 ]
```
## Use Value Directly
```c
IfaceList my_list = list(0); // init our list
// can add value directly
for(int i = 0 ; i < 10 ; i++){
    LAppend(INT , i);
}
/// also

IfaceList my_list = list(3,
            INT , 12,
            CHR , 'r',
            FLT ,12.23,
);
```
## Filter Method

```c
LBOOL callback(void* handle){
	if((*(int*)handle)%2 == 0)	return true;
	else				return false;
}
int main(void){
	int check = 5;
	int check2 = 6;
	int check3 = 8;
	IfaceList my_list = list(
				4,
				INT , (void*)&check,
				STR , "foo",
				INT , (void*)&check2,
				INT , (void*)&check3
			);
	// test filter function
    // the filter function will filter all the value using the callback function that you wrote
	my_list.Filter(callback , INT);
}
```

