#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

#define LIST_C
#include "raylist.h"

int main(void){
	Class_List my_list = list(
				2,
				STR , "Hello",
				STR , "World"
			);
	my_list.List_Print();
	return 0;
}
