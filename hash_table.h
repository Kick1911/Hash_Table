#ifndef HASH_H
#define HASH_H
#include <stddef.h>
#define START_SIZE (17)

typedef unsigned int u_int;

typedef struct h_node{
	char* key;
	void* value;
}h_node_t;

typedef struct h_table{
	u_int size;
	u_int increm;
	u_int num_of_elements;
	h_node_t* hash_table;
}h_table_t;

char h_alloc_table(h_table_t*);
char h_free_table(h_table_t*);

char h_insert(h_table_t*, char*, void*);
void* h_lookup(h_table_t*, char*);
char check(h_table_t*, char*);
static u_int hash(char*, u_int);
__inline__ u_int h_size(h_table_t*);

#endif
