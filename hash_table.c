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

	ht->hash_table = (h_node_t*) malloc(sizeof(h_node_t) * ht->size);
	if(!ht->hash_table) return NULL;
	int i = 0;while( i < ht->size )
		ht->hash_table[i++] = (h_node_t){NULL, NULL};
	return ht;
}

char h_free_table(h_table_t* ht){
	free(ht->hash_table);
	free(ht);
	return 0;
}

static u_int hash(char* k, u_int limit){
	u_int h = 0;
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

char h_insert(h_table_t* ht, char* k, void* v){
	u_int hk = hash(k, ht->increm);
	int p = ht->increm, offset = 0;
	while( ht->hash_table[hk].key ){
		offset = p;
		p += ht->increm;
		if(p >= ht->size)
			resize(ht, ht->increm);
		hk = hash(k, ht->increm) + offset;
	}
	ht->hash_table[hk] = (h_node_t){ k, v };
	ht->num_of_elements++;
	return 0;
}

static void display(h_table_t* ht){
	size_t i = 0;while( i < ht->size ){
		void* temp = ht->hash_table[i++].value;
		printf("%d ", (temp)?*((int*)temp):0);
	}
	printf("\n");
}

void* h_lookup(h_table_t* ht, char* k){
	int offset = 0;
	u_int hk = hash(k, ht->increm);
	while( ht->hash_table[hk].key &&
			strcmp(ht->hash_table[hk].key, k) ){
		hk = hash(k, ht->increm) + offset;
		offset += ht->increm;
	}
	return ht->hash_table[hk].value;
}

__inline__ u_int h_size(h_table_t* ht){
	return ht->size;
}

#ifdef STATIC
int main(void){
	u_int value = 19, unique = 1911;
	char* keys[] = {"kick", "ness", "S1", "S2"};
	h_table_t* ht = h_create_table();
	h_insert(ht, *keys, &value);
	h_insert(ht, keys[1], &value);
	h_insert(ht, keys[2], &value);
	h_insert(ht, keys[3], &unique);
	display(ht);
	printf("Probing %s: %d\n",keys[3], *((u_int*)h_lookup(ht, keys[3])));
	h_free_table(ht);
	return 0;
}
#endif
