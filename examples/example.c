#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

// using list name space 
#define USING_LIST

#define LIST_C
#include "../raylist.h"

// TODO: make List_Search function work
// TODO: make List_Pop_Idx function work

int _add();
int sum();
void test(void);
void foo(void);
void bar(void);
void baz(void);

// TODO: handle float type
// TODO: handle char type
// TODO: handle double type

#if 1
int main(void){
	// we can put pointer directly ,but we can't put value 
	float a = 4.01f;
	char b = 'A';
	Class_List my_list = list(
				4,
				STR , "foo",
				FLT , (void*)&a,
				BOOL , false,
				CHR , (void*)&b
			);
	//my_list.List_Reverse();
	my_list.Append(STR , "RayDen");
	my_list.Append(STR , "abdo");
	char gett = *(char*)my_list.Get(3);
	printf("gett : %c\n" , gett);

	int idx;
	bool _test = true;
	if(my_list.Search(&idx , FLT , (void*)&a)){
		printf("Found in index : %d\n" , idx);
	}else	printf("Not Found \n");
	my_list.Print();
	my_list.Clear();
	my_list.Print();

	return 0;
}
#else

int main(void){
	// to define empty list use
	//  Class_List my_list = list(0);
	Class_List my_list = list(
			1,
			VOIDFUNC ,test
			);

	my_list.Append(VOIDFUNC ,baz);
	my_list.Exec_Func(1);
	return 0;
}

#endif

void test(void){
	for(int i = 0 ; i < 100 ; i++){
		printf("test\n");
	}
}
void foo(void){
	printf("foo\n");
}
void bar(void){
	printf("bar\n");
}
void baz(void){
	printf("baz\n");
}
