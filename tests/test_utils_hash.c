#include <unitest.h>
#include <stdio.h>
#include <malloc.h>
#include <hash_table.h>
#include <utils/hash.h>

int main(void){
    h_table_t ht;

    h_table_init(&ht);

    TEST(Get unknown key,
        int64_t h;

        h = get_hash(&ht, "unknown");
        T_ASSERT_NUM(h, 36);
        h = get_hash(&ht, "");
        T_ASSERT_NUM(h, -1);
    );

    TEST(Get unknown entry,
        h_node_t* n;

        n = get_entry(&ht, "unknown");
        T_ASSERT(!n);
        n = get_entry(&ht, "");
        T_ASSERT(!n);
    );

    h_table_free(&ht);
    T_CONCLUDE();
    return 0;
}
