#include <dlinked_list.h>
#ifndef HASH_H
#define HASH_H

#define START_SIZE (17)
#define _HT_U_INT unsigned int

typedef struct h_node{
	char* key;
	void* value;
}h_node_t;

typedef struct {
	_HT_U_INT size;
	_HT_U_INT increm;
	_HT_U_INT num_of_elements;
	h_node_t* hash_table;
	dl_list_t* keys;
}h_table_t;

h_table_t* h_create_table();
char h_free_table(h_table_t*);

char h_insert(h_table_t*, const char*, void*);
void* h_lookup(h_table_t*, const char*);
void h_free_key(h_table_t*, char*);
char check(h_table_t*, char*);
_HT_U_INT h_size(h_table_t*);

#endif
