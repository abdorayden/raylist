#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>

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


#if 1
int main(void){
	// we can put pointer directly ,but we can't put value 
	List* l = NULL ;
	Class_List my_list = list(&l,
				3,
				STR , "foo",
				BOOL , false,
				CHR , "A"
			);
	//my_list.List_Reverse();
	my_list.List_Append(&l ,STR , "RayDen");
	my_list.List_Append(&l ,STR , "abdo");
	void* gett = my_list.List_Get(l , 0);
	if(gett != NULL){
		printf("gett : %s\n" , gett);
	}
	int idx;
	bool _test = true;
	if(my_list.List_Search(l , &idx , BOOL , (void*)&_test)){
		printf("Found in index : %d\n" , idx);
	}else	printf("Not Found \n");
	my_list.List_Print(l);
	my_list.List_Clear(&l);
	my_list.List_Print(l);

	return 0;
}
#else

int main(void){
	List* l;
	Class_List my_list = list(
			&l,
			2,
			INTFUNC  ,_add,
			INTFUNC  ,sum
			);

	my_list.List_Append(&l , VOIDFUNC ,baz);
	my_list.List_Reverse(&l);
	int value = my_list.List_Exec_Func(l , -1);
	printf("value : %d" , value);
	return 0;
}

#endif

int _add(){
	return 3+3;
}

int sum(){
	return 5+5;
}

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
