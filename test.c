#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

#define LIST_C
#include "raylist.h"

// TODO: make List_Search function work
// TODO: make List_Pop_Idx function work

int main(void){
	int v = 7;
	float p = 3.14;
	// we can put pointer directly ,but we can't put value 
	void* ptr = &v;
	Class_List my_list = list(
				7,
				STR , "Hello",
				CHR , "r",
				BOOL ,true,
				INT , (void*)&v,
				FLT , (void*)&p,
				VOIDPTR , ptr,
				STR , "World"
			);
	my_list.List_Reverse();
	my_list.List_Append(STR , "RayDen");
	my_list.List_Append(BOOL , false);
	void* gett = my_list.List_Get(3);
	if(gett != NULL){
		printf("gett : %d" , *(int*)gett);
	}
	my_list.List_Print();
	return 0;
}
