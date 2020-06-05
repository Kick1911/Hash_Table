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
	h_node_t** hash_table;
	void* elements; /* dl_link_t */
	void (*free)(void*);
}h_table_t;

typedef struct{
	void* node; /* dl_node_t */
} h_iter_t;

h_table_t* h_create_table(void (*free_cb)(void*));
char h_free_table(h_table_t*);
h_iter_t* h_iter(h_table_t*);
int h_next(h_iter_t*, char**, void**);
int h_next_node(h_iter_t*, h_node_t**);
char h_insert(h_table_t*, const char*, void*);
void* h_lookup(h_table_t*, const char*);
void h_free_key(h_table_t*, char*);
char check(h_table_t*, char*);
_HT_U_INT h_size(h_table_t*);

#endif
