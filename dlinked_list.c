#include <dlinked_list.h>
#include <malloc.h>

static dl_node_t* dl_create_node(void* data){
    dl_node_t* n = (dl_node_t*)malloc(sizeof(dl_node_t));
    if(!n) return NULL;
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

dl_list_t* dl_create(){
    dl_list_t* l = (dl_list_t*)malloc(sizeof(dl_list_t));
    if(!l) return NULL;
    l->head = NULL;
    l->end = l->head;
    return l;
}

dl_node_t* dl_push(dl_list_t* l, void* data){
    dl_node_t* n = dl_create_node(data);
    if(!n) return NULL;
    n->data = data;
    n->next = NULL;
    n->prev = l->end;
    if (l->end != NULL)
        l->end->next = n;
    l->end = n;
    return n;
}

void* dl_pop(dl_list_t* l){
    void* d;
    dl_node_t* n = l->end;
    if(!n) return NULL;

    d = n->data;
    l->end = n->prev;
    if(l->end)
        l->end->next = NULL;
    free(n);
    return d;
}

dl_node_t* dl_peek(dl_list_t* l){
    return l->head;
}

void* dl_pop_head(dl_list_t* l){
    void* d;
    dl_node_t* n = l->head;
    if(!n) return NULL;

    d = n->data;
    l->head = n->next;
    if(l->head)
        l->head->prev = NULL;
    free(n);
    return d;
}

void* dl_unlink(dl_list_t* l, dl_node_t* n){
    void* d;
    if(l->end == n)
        return dl_pop(l);
    if(l->head == n)
        return dl_pop_head(l);
    d = n->data;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    free(n);
    return d;
}

size_t dl_size(dl_list_t* l){
    return 0;
}

void dl_free(dl_list_t* l){
    free(l->head);
    free(l);
}
