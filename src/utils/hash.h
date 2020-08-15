#ifndef _HASH_H
#define _HASH_H

#include <stddef.h>
#include <string.h>
#include <hash_table.h>
#include <dlinked_list.h>
#include <stdio.h>

static uint64_t mod(int64_t a, int64_t b){ 
	int64_t d = a/b;
	int64_t r = a - b * d;
	if( d < 1 ){
		a = b + r;
		return a - b * (a / b);
	}
	return r;
}

static int64_t hash(const char* k, uint64_t limit){
	int64_t h = 0;
	size_t len = strlen(k);

    if(!len) return -1;
	while(--len)
		h += k[len] - k[len+1];
	return mod(h, limit);
}

static int64_t get_hash(h_table_t* ht, const char* k){
	int64_t hk = hash(k, ht->increm);

    if(hk < 0) return -1;
	while( ht->hash_table[hk] && strcmp(HN_KEY(ht->hash_table[hk]), k) )
		hk += ht->increm;
	return hk;
}

static h_node_t* get_entry(h_table_t* ht, const char* k){
	int64_t hk = get_hash(ht, k);
	if(hk < 0 || hk > ht->capacity) return NULL;
	return (ht->hash_table[hk])? DN_DATA(ht->hash_table[hk]) : NULL;
}

#endif

