#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <stdint.h>

typedef struct h_node{
	char* key;
	void* value;
}h_node_t;

typedef struct {
	uint32_t capacity;
	uint32_t increm;
	uint32_t size;
	void** hash_table; /* dl_link_t** */
	void* elements; /* dl_link_t* */
	void (*free)(void*);
}h_table_t;

typedef struct{
	void* node; /* dl_node_t */
} h_iter_t;

#define H_SIZE(ht) ((h_table_t*)(ht))->size

h_table_t* h_create_table(void (*free_cb)(void*));
void h_free_table(h_table_t*);
h_iter_t* h_iter(h_table_t*);
int h_next(h_iter_t*, char**, void**);
char h_insert(h_table_t*, const char*, void*);
void* h_delete(h_table_t*, const char*);
void* h_lookup(h_table_t*, const char*);

#endif /* END OF _HASH_TABLE_H */
