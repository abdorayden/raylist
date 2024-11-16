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

//  Class_List my_list = list(0);

int _add();
int sum();
char* tostr();
void test(void);
void foo(void);
void bar(void);
void baz(void);

// TODO: handle float type
// TODO: handle char type
// TODO: handle double type

LBOOL callback(void* handle){
	if((*(int*)handle)%2 != 0)	return true;
	else				return false;
}

#if 1
int main(void){
	// we can put pointer directly ,but we can't put value 
	int check = 5;
	Class_List my_list = list(
				3,
				BOOL , false,
				STR , "foo",
				INT , (void*)&check
			);
	// test filter function
	my_list.Filter(callback , INT);
	//my_list.Del_Index(0);
	my_list.Print();
	//my_list.List_Reverse();
	//my_list.Append(STR , "RayDen");
	//my_list.Append(STR , "abdo");
	//my_list.Del_Index(0);
	//float gett = *(float*)my_list.Get(0);
	//printf("gett : %f\n" , gett);

	// test get error funtion
	//Class_List ll = list(0);
	//printf("%s\n" , ll.Get_Error());
	//ll.Append(STR , "Hello  World");
	//if(ll.Get_Error() == NULL) printf("status is fine \n");

	//int idx;
	//bool _test = true;
	//if(my_list.Search(&idx , VFLT , (void*)&a)){
	//	printf("Found in index : %d\n" , idx);
	//}else	printf("Not Found \n");
	//my_list.Print();
	my_list.Clear();
	//my_list.Print();

	return 0;
}
#else

int main(void){
	// to define empty list use
	Class_List my_list = list(
			2,
			STRFUNC , tostr,
			VOIDFUNC ,test
			);

	my_list.Append(VOIDFUNC ,baz);
	my_list.Append(VOIDFUNC ,bar);
	printf("function said : %s", my_list.Exec_Func(0));
	return 0;
}

#endif

char* tostr()
{
	return "Hello World";
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
