#include <unitest.h>
#include <hash_table.h>
#include <dlinked_list.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

void insert_strings(){
    h_table_t* ht = h_create_table(NULL);
	char *str[] = {"Kick", "vev", "btbtrt", "ntsrb", "btsnt", NULL};
	char** ptr = str;
	while(*ptr){
		h_insert(ht, *ptr, *ptr);
		T_ASSERT_STRING((char*)h_lookup(ht, *ptr), *ptr);
		ptr++;
	}
	h_free_table(ht);
}

void insert_one(){
	int a = 19;
	char str[] = "Kickness";
	h_table_t* ht = h_create_table(NULL);
	h_insert(ht, str, &a);
	T_ASSERT_NUM(*((int*)h_lookup(ht, str)), a);
	h_free_table(ht);
}

int main(void){
	TEST(Lookup no exist,
		h_table_t* ht = h_create_table(NULL);
		T_ASSERT(!h_lookup(ht, "Does not exists"))
        h_free_table(ht);
	);

	TEST(Set empty key,
		h_table_t* ht = h_create_table(NULL);
		T_ASSERT_NUM(h_insert(ht, "", NULL), 1);
		T_ASSERT_NUM(H_SIZE(ht), 0);
        h_free_table(ht);
	);

	TEST(Delete bad keys,
		h_table_t* ht = h_create_table(NULL);
		T_ASSERT(!h_delete(ht, ""));
		T_ASSERT(!h_delete(ht, "unknown"));
		T_ASSERT_NUM(H_SIZE(ht), 0);
		h_free_table(ht);
	);

	TEST(Delete keys,
		int num = 19;
		int* ptr = &num;
		char str[] = "This is a string.";
		h_table_t* ht = h_create_table(NULL);

		T_ASSERT_NUM(h_insert(ht, "key1", ptr), 0);
		T_ASSERT_NUM(h_insert(ht, "key2", str), 0);
		T_ASSERT_NUM(H_SIZE(ht), 2);

		T_ASSERT_NUM(h_lookup(ht, "key1"), ptr);
		T_ASSERT_NUM(h_lookup(ht, "key2"), str);

		T_ASSERT_NUM(h_delete(ht, "key1"), ptr);
		T_ASSERT_NUM(h_delete(ht, "key2"), str);
		T_ASSERT_NUM(H_SIZE(ht), 0);
        h_free_table(ht);
	);

	TEST(Insert one string, insert_one());

	TEST(Insert String into hash, insert_strings());

	TEST(Re-insert,
		char test1[] = "Test string";
		char test2[] = "Another test string";
		h_table_t* ht = h_create_table(NULL);

		T_ASSERT_NUM(h_insert(ht, "kick", test1), 0);
		T_ASSERT_NUM(h_insert(ht, "kick", test2), 0);
		h_free_table(ht);
	);

	TEST(Iterator,
		void* iter;
		char* k = NULL;
		void* v = NULL;
		char test1[] = "Test string";
		char test2[] = "Another test string";
		h_table_t* ht = h_create_table(NULL);
		h_insert(ht, "a", test1);
		h_insert(ht, "b", test2);
		iter = h_iter(ht);
		T_ASSERT_NUM(h_next(iter, &k, &v), 0);
		T_ASSERT_STRING(k, "a");
		T_ASSERT_STRING((char*)v, test1);
		T_ASSERT_NUM(h_next(iter, &k, &v), 0);
		T_ASSERT_STRING(k, "b");
		T_ASSERT_STRING((char*)v, test2);
		T_ASSERT_NUM(h_next(iter, &k, &v), 1); /* Auto free iterator */
		h_free_table(ht);
	);

	TEST(Linked List Test,
		char str1[] = "Kick";
		char str2[] = "ness";
		char str3[] = "mid";
		void* n;
		void* l = dl_create();
		T_ASSERT(l);
		T_ASSERT(dl_push(l, str1));
		T_ASSERT( (n = dl_push(l, str3)) );
		T_ASSERT(dl_push(l, str2));
		T_ASSERT_STRING((char*)dl_unlink(l, n), str3);
		T_ASSERT_STRING((char*)dl_pop(l), str2);
		T_ASSERT_STRING((char*)dl_pop(l), str1);
		T_ASSERT(!dl_pop(l));
		dl_free(l);
	);

	T_CONCLUDE();
	return 0;
}
