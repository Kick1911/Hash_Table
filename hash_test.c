#include <unitest.h>
#include <hash_table.h>
#include <string.h>
#include <stdio.h>

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
		printf("%s\n", *ptr);
		h_insert(ht, *ptr, *ptr);
		T_ASSERT_STRING((char*)h_lookup(ht, *ptr), *ptr);
		ptr++;
	}
}

void test_resize(){
	char str[] = "Kick";
	h_table_t* ht = h_create_table();
	unsigned int size = h_size(ht);
	h_insert(ht, str, (char*)1);
	h_insert(ht, str, (char*)2);
	h_insert(ht, str, (char*)3);
	h_insert(ht, str, (char*)4);
	h_insert(ht, str, (char*)5);

	/* I am not sure why it's size*6 and not size*5 */
	T_ASSERT_NUM(h_size(ht), size * 6);
	h_free_table(ht);
}

void insert_one(void* ht){
	char str[] = "Kickness";
	h_insert(ht, str, (char*)5);
	T_ASSERT_NUM((char*)h_lookup(ht, str), 5);
}

int main(void){
	h_table_t* ht = h_create_table();

	TEST(Insert one string, insert_one(ht));

	TEST(Insert String into hash, insert_strings(ht));
	TEST(Resizing, test_resize());
	

	/* TEST(Insert more than table size, stress_size(&ht)); */
	

	h_free_table(ht);
	T_CONCLUDE();
	return 0;
}
