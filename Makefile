include Makefile.in

INCLUDE=include
LIB=lib

hash_table.out: hash_table.h hash_table.c
	gcc -ansi $(DEBUG) -D STATIC -o hash_table.out hash_table.c -I.
test: library hash_test.c
	gcc -ansi $(DEBUG) -o hash_table.test hash_test.c $(LIB)/libhash.so.1 -I.
mod_test: mod_test.c library
	gcc -ansi $(DEBUG) -o mod.test mod_test.c -I. -lhash
library: hash_table.h hash_table.c
	gcc -ansi $(DEBUG) -fPIC -c -o hash_table.o hash_table.c -I.
	gcc -ansi -shared -o $(LIB)/libhash.so.1 hash_table.o
	rm -f hash_table.o
install: library
	cp hash_table.h $(PREFIX)/$(INCLUDE)
	cp $(LIB)/libhash.so.1 $(PREFIX)/$(LIB)
	ln -sf $(PREFIX)/$(LIB)/libhash.so.1 $(PREFIX)/$(LIB)/libhash.so
uninstall:
	rm -f $(PREFIX)/$(LIB)/hash_table.h
	rm -f $(PREFIX)/$(LIB)/libhash.so.1 $(PREFIX)/$(LIB)/libhash.so
clean:
	rm -f *.out *.test $(LIB)/*.so.*
