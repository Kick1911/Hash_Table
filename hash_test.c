#include <unitest.h>
#include <hash_table.h>
#include <dlinked_list.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

/* void stress_size(void* ht){
	size_t s = h_size(ht) * 2;
	while(s--){
		h_insert(ht, str, (char*)5);
	}
} */

void insert_strings(void* ht){
	char *str[] = {"Kick", "vev", "btbtrt", "ntsrb", "btsnt", NULL};
	char** ptr = str;
	while(*ptr){
		h_insert(ht, *ptr, *ptr);
		T_ASSERT_STRING((char*)h_lookup(ht, *ptr), *ptr);
		ptr++;
	}
}

void insert_one(void* ht){
	int a = 19;
	char str[] = "Kickness";
	h_insert(ht, str, &a);
	T_ASSERT_NUM(*((int*)h_lookup(ht, str)), a);
}

int main(void){
	h_table_t* ht = h_create_table();

	TEST(Lookup no exist,
		T_ASSERT(!h_lookup(ht, "Does not exists"))
	);

	TEST(Insert one string, insert_one(ht));

	TEST(Insert String into hash, insert_strings(ht));

	/* TEST(Insert more than table size, stress_size(&ht)); */

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

	h_free_table(ht, NULL);
	T_CONCLUDE();
	return 0;
}
