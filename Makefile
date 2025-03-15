# make all will build all libs .a .so .dll
all : libs libraylist.so libraylist.a libraylist.dll libraylist.o

libraylist.so : libs
	gcc -shared -fPIC -Wall -Wextra -DLIST_C raylist.h -o libs/libraylist.so

# this for dll lib in windows 
libraylist.dll : libs
	x86_64-linux-gnu-gcc -shared  -Wall -Wextra -DLIST_C raylist.h -o libs/libraylist.dll

libraylist.a : libs libraylist.o
	ar rcs libs/libraylist.a libs/libraylist.o

libraylist.o : libs
	gcc -c -DLIST_C raylist.h -o libs/libraylist.o

libs : 
	mkdir libs
