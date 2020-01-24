#include <hash_table.h>
#include <malloc.h>
#include <string.h>

static unsigned long mod(long a, long b){ 
	long d = a/b;
	long r = a - b * d;
	if( d < 1 ){
		a = b + r;
		return a - b * (a / b);
	}
	return r;
}

h_table_t* h_create_table(){
	h_table_t* ht = malloc(sizeof(h_table_t));
	if(!ht) return NULL;
	ht->size = START_SIZE;
	ht->increm = ht->size;
	ht->num_of_elements = 0;

	ht->hash_table = (h_node_t*) calloc(sizeof(h_node_t), ht->size);
	if(!ht->hash_table) return NULL;
	return ht;
}

char h_free_table(h_table_t* ht){
	free(ht->hash_table);
	free(ht);
	return 0;
}

static _HT_U_INT hash(const char* k, _HT_U_INT limit){
	_HT_U_INT h = 0;
	size_t len = strlen(k) - 1;
	while(len--)
		h += k[len] - k[len+1];
	return mod(h, limit);
}

static char resize(h_table_t* ht, size_t more){
	size_t old_size = ht->size;
	size_t new_size = old_size + more;
	ht->hash_table = realloc(ht->hash_table, sizeof(h_node_t) * new_size);
	if(!ht->hash_table) return 2;
	while(old_size < new_size)
		ht->hash_table[old_size++] = (h_node_t){NULL, NULL};
	ht->size += more;
	return 0;
}

static h_node_t* get_entry(h_table_t* ht, const char* k){
	int offset = 0;
	_HT_U_INT hk = hash(k, ht->increm);
	while( ht->hash_table[hk].key &&
			strcmp(ht->hash_table[hk].key, k) ){
		hk = hash(k, ht->increm) + offset;
		offset += ht->increm;
	}
	return ht->hash_table + hk;
}

char h_insert(h_table_t* ht, const char* k, void* v){
	char* key;
	_HT_U_INT hk = hash(k, ht->increm);
	int p = ht->increm, offset = 0;
	while( (key = ht->hash_table[hk].key) && strcmp(key, k) ){
		if(p > ht->size)
			resize(ht, ht->increm);
		hk = hash(key, ht->increm) + (p - ht->increm);
		p += ht->increm;
	}
	if(!key){
		key = malloc(sizeof(char) * strlen(k) + 1);
        strcpy(key, k);
	}
	ht->hash_table[hk].key = key;
	ht->hash_table[hk].value = v;
	ht->num_of_elements++;
	return 0;
}

void h_free_key(h_table_t* ht, char* k){
	h_node_t* n = get_entry(ht, k);
	free(n->key);
	n->key = NULL;
}

void display(h_table_t* ht){
	size_t i = 0;while( i < ht->size ){
		void* temp = ht->hash_table[i++].value;
		printf("%ld ", (temp)?*((int*)temp):0);
	}
	printf("\n");
}

void* h_lookup(h_table_t* ht, const char* k){
	return get_entry(ht, k)->value;
}

__inline__ _HT_U_INT h_size(h_table_t* ht){
	return ht->size;
}

#ifdef STATIC
int main(void){
	char** ptr;
	char str[] = "Kickness";
	char* keys[] = {"Kick", "vev", "btbtrt", "ntsrb", "btsnt", NULL};
	h_table_t* ht = h_create_table();
	ptr = keys;
	h_insert(ht, str, str);
	while(*ptr){
		h_insert(ht, *ptr, *ptr);
        printf("Probing %s: %s\n", *ptr, h_lookup(ht,  *ptr));
		ptr++;
	}
	display(ht);
	h_free_table(ht);
	return 0;
}
#endif
