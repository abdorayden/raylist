# Dynamic List with Diffrent data type in C

<p align="center">
  <img src="./assets/logo.jpeg" width="350" height="350"/>
</p>

## This is a C code that implements a linked list data structure with a variety of functions to manipulate and interact with the list. Here's a breakdown of the code:

### methods available

#### list(int count, ...): 
a variadic function that creates a new linked list with count elements, taking a variable number of arguments of different types.
#### List_Append(Type type, void* data): 
appends a new node to the end of the list with the given type and data.
#### List_Pop_Index(List** _list, int idx): 
removes the node at the specified idx from the list.
#### List_Get(int idx): 
returns the data at the specified idx from the list.
#### List_Search(int* idx, void* data): 
searches for the specified data in the list and returns the index if found.
#### List_Reverse(void): 
reverses the order of the nodes in the list.
#### List_Print(void): 
prints the contents of the list.

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
```

