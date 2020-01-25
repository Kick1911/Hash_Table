include Makefile.in

INCLUDE=include
LIB=lib

hash_table.out: hash_table.h hash_table.c
	gcc -ansi $(DEBUG) -D STATIC -o hash_table.out hash_table.c -I.

test: library hash_test.c
	gcc -ansi $(DEBUG) -o hash_table.test hash_test.c $(LIB)/libhash.so.1 -I.

mod_test: mod_test.c library
	gcc -ansi $(DEBUG) -o mod.test mod_test.c -I. -lhash

dlinked_list.o: dlinked_list.c 
	gcc -ansi $(DEBUG) -fPIC -c -o dlinked_list.o dlinked_list.c -I.

library: hash_table.h hash_table.c dlinked_list.o
	gcc -ansi $(DEBUG) -fPIC -c -o hash_table.o hash_table.c -I.
	gcc -ansi -shared -o $(LIB)/libhash.so.1 hash_table.o dlinked_list.o
	rm -f hash_table.o

install: library
	if ! [ -d $(PREFIX)/$(INCLUDE) ]; then mkdir -p $(PREFIX)/$(INCLUDE); fi
	cp hash_table.h $(PREFIX)/$(INCLUDE)
	if ! [ -d $(PREFIX)/$(LIB) ]; then mkdir -p $(PREFIX)/$(LIB); fi
	cp $(LIB)/libhash.so.1 $(PREFIX)/$(LIB)
	ln -sf $(PREFIX)/$(LIB)/libhash.so.1 $(PREFIX)/$(LIB)/libhash.so

uninstall:
	rm -f $(PREFIX)/$(LIB)/hash_table.h
	rm -f $(PREFIX)/$(LIB)/libhash.so.1 $(PREFIX)/$(LIB)/libhash.so

clean:
	rm -f *.out *.test $(LIB)/*.so.*
