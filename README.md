# Dynamic List with Diffrent data type in C

<p align="center">
  <img src="./assets/logo.jpeg" width="350" height="350"/>
</p>

## This is a C code that implements a linked list data structure with a variety of functions to manipulate and interact with the list. Here's a breakdown of the code:

### methods available

## list(int count, ...): 
```c
Class_List list(
		int count , 	// number of data you
		... 		// data : <TYPE> , <VALUE>
);
```
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
Class_List my_list = list(
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
Class_List my_list = list(0); // init our list
// can add value directly
for(int i = 0 ; i < 10 ; i++){
    LAppend(INT , i);
}
/// also

Class_List my_list = list(3,
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
	Class_List my_list = list(
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

