#include<stdio.h>

typedef void(*Call)(void);

void func(void){
	printf("Call");
}

void ff(void* fn){
	printf("from fn i call");
	//(Call)fn();
	int (*ret)() = (int(*)())fn;
	ret();
}

int main(void){
	ff(func);
	return 0;
}
