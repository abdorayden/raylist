/*
 *	RayList v2.3.0 Example file
 *
 *	a simple Dictionary using raylist
 *
 * */

#include <stdio.h>
#include <stdarg.h>

#define LIST_C
#define USING_LIST
#include "../../raylist.h"


// here you need to initialize your keyy and value type 
typedef struct {
	int key;
	char* value;
}Dictionary;

int main(void){
	// initialize our list
	RLList list_dict = List(0);

	// enable copy object
	// the append function will create copy of the object and added to the list
	RLCopyObject(sizeof(Dictionary));
	// fill Dictionary struct
	Dictionary dict = {
		0,
		"rayden"
	};
	// append variable struct to a list
	list_dict.Append(RL_VOIDPTR , (void*)&dict);
	dict = (Dictionary){
		1,
		"foo"
	};
	list_dict.Append(RL_VOIDPTR , (void*)&dict);
	dict = (Dictionary){
		2,
		"bar"
	};
	list_dict.Append(RL_VOIDPTR , (void*)&dict);
	dict = (Dictionary){
		3,
		"baz"
	};
	list_dict.Append(RL_VOIDPTR , (void*)&dict);
	RLDisableCopyObject();

	// print Dictionary
	for(int i = 0 ; i < list_dict.Len() ; i++){
		Dictionary t = *(Dictionary*)list_dict.Get(i);
		printf("%d => %s\n" , t.key , t.value);
	}
	// clear memory
	list_dict.Clear();
	return 0;
}
