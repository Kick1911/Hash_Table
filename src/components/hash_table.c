#include <hash_table.h>
#include <malloc.h>
#include <string.h>
#include <utils/hash.h>
#include <dlinked_list.h>
#include <errno.h>

#define START_SIZE (37)

static char
resize(h_table_t* ht, size_t more){
    size_t old_size = ht->capacity;
    size_t new_size = old_size + more;
    ht->hash_table = realloc(ht->hash_table, sizeof(*ht->hash_table) * new_size);

    if(!ht->hash_table) return 2;

    while(old_size < new_size)
        ht->hash_table[old_size++] = NULL;
    ht->capacity += more;
    return 0;
}

int
h_init_table(h_table_t* ht){
    if(!ht) return 1;

    ht->capacity = START_SIZE;
    ht->increm = ht->capacity;
    ht->size = 0;
    ht->elements = dl_create();
    ht->free = NULL;

    ht->hash_table = calloc(sizeof(*ht->hash_table), ht->capacity);
    if(!ht->hash_table) return 2;
    return 0;
}

void
h_free_table(h_table_t* ht){
    h_node_t* n;
    while( (n = dl_pop(ht->elements)) ){
        free(n->key);
        if(ht->free)
            ht->free(n->value);
        free(n);
    }
    dl_free(ht->elements);
    free(ht->hash_table);
}

char
h_insert(h_table_t* ht, const char* k, void* v){
    char* key;
    h_node_t* n;
    int64_t hk, p;

    hk = hash(k, ht->increm);
    if(hk < 0) return 1;

    p = ht->increm;
    while( ht->hash_table[hk] && strcmp(HN_KEY(ht->hash_table[hk]), k) ){
        if(p > ht->capacity)
            resize(ht, ht->increm);
        hk = hash(HN_KEY(ht->hash_table[hk]), ht->increm) + (p - ht->increm);
        p += ht->increm;
    }

    if(!ht->hash_table[hk]){
        n = malloc(sizeof(*n));
        if(!n) goto fail;
        key = malloc(sizeof(char) * strlen(k) + 1);
        if(!key) goto free_n_and_fail;
        strcpy(key, k);
        n->key = key;
        ht->hash_table[hk] = dl_push(ht->elements, n);
        ht->size++;
    }else
        if(ht->free) ht->free(HN_VALUE(ht->hash_table[hk]));

    HN_VALUE(ht->hash_table[hk]) = v;

    return 0;

    free_n_and_fail:
    free(n);

    fail:
    return 1;
}

void*
h_delete(h_table_t* ht, const char* k){
    void* d = NULL;
    int64_t hk;

    hk = get_hash(ht, k);
    if(hk < 0 || !ht->hash_table[hk]){
        errno = ENOENT;
        return NULL;
    }

    d = HN_VALUE(ht->hash_table[hk]);
    free(HN_KEY(ht->hash_table[hk]));
    /* Delete from dlinked_list and free h_node_t */
    free(dl_unlink(ht->elements, ht->hash_table[hk]));
    ht->hash_table[hk] = NULL;

    /* Move duplicate keys one increment back */
    while( (hk += ht->increm) < ht->capacity && ht->hash_table[hk] )
        ht->hash_table[hk - ht->increm] = ht->hash_table[hk];

    ht->size--;

    return d;
}

h_iter_t*
h_iter(h_table_t* ht){
    h_iter_t* hi = malloc(sizeof(h_iter_t));
    if(!hi) return NULL;
    hi->node = DL_HEAD(ht->elements);
    return hi;
}

static int
h_next_node(h_iter_t* hi, h_node_t** hn){
    dl_node_t* n = hi->node;
    if(!n) return 1;

    *hn = n->data;
    hi->node = n->next;
    return 0;
}

int
h_next(h_iter_t* hi, char** k, void** v){
    h_node_t* hn = NULL;
    if(h_next_node(hi, &hn)){
        free(hi);
        return 1;
    }
    if(!hn) return 1;

    *k = hn->key;
    *v = hn->value;
    return 0;
}

/* static void display(h_table_t* ht){
    size_t i = 0;while( i < ht->capacity ){
        if(ht->hash_table[i]){
            void* temp = ht->hash_table[i]->value;
            printf("%d ", (temp)?*((int*)temp):0);
        }
        i++;
    }
    printf("\n");
} */

void*
h_lookup(h_table_t* ht, const char* k){
    h_node_t* n = get_entry(ht, k);
    if(!n){
        errno = ENOENT;
        return NULL;
    }
    return n->value;
}

