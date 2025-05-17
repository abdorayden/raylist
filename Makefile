# TODO: fix dynamic and static lib
# make all will build all libs .a .so .dll
all : libraylist.a libraylist.dll libraylist.so

libraylist.so : libraylist.o
	gcc libs/libraylist.o -shared -Wall -Wextra -o libs/libraylist.so

# this for dll lib in windows 
libraylist.dll : libraylist.o
	x86_64-linux-gnu-gcc libs/libraylist.o -shared  -Wall -Wextra -o libs/libraylist.dll

libraylist.a : libraylist.o raylist.h
	ar rcs libs/libraylist.a libs/libraylist.o

libraylist.o : raylist.h
	gcc -x c -c -fPIC -DLIST_C raylist.h -o libs/libraylist.o

clean : 
	rm libs/libraylist.o
