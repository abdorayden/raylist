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
	RLList dict = List(0);

	// fill Dictionary struct
	Dictionary dict1 = {
		0,
		"rayden"
	};
	Dictionary dict2 = {
		1,
		"foo"
	};
	Dictionary dict3 = {
		2,
		"bar"
	};
	Dictionary dict4 = {
		3,
		"baz"
	};
	// append variable struct to a list
	dict.Append(RL_VOIDPTR , (void*)&dict1);
	dict.Append(RL_VOIDPTR , (void*)&dict2);
	dict.Append(RL_VOIDPTR , (void*)&dict3);
	dict.Append(RL_VOIDPTR , (void*)&dict4);

	// print Dictionary
	for(int i = 0 ; i < dict.Len() ; i++){
		Dictionary t = *(Dictionary*)dict.Get(i);
		printf("%d => %s\n" , t.key , t.value);
	}
	// clear memory
	dict.Clear();
	return 0;
}
