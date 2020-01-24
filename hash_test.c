#include <unitest.h>
#include <hash_table.h>
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
		h_free_key(ht, *ptr);
		ptr++;
	}
}

void insert_one(void* ht){
	int a = 19;
	char str[] = "Kickness";
	h_insert(ht, str, &a);
	T_ASSERT_NUM(*((int*)h_lookup(ht, str)), a);
    display(ht);
    h_free_key(ht, str);
}

int main(void){
	h_table_t* ht = h_create_table();

	TEST(Lookup no exist,
		T_ASSERT(!h_lookup(ht, "Does not exists"))
	);

	TEST(Insert one string, insert_one(ht));

	TEST(Insert String into hash, insert_strings(ht));

	/* TEST(Insert more than table size, stress_size(&ht)); */
	

	h_free_table(ht);
	T_CONCLUDE();
	return 0;
}
