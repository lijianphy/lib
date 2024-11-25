#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "khash.h"

// Declare test hash tables
KHASH_MAP_INIT_INT(int32, int) // int -> int hash map
KHASH_MAP_INIT_STR(str, int)   // string -> int hash map
KHASH_SET_INIT_INT(intset)     // int set

void test_int_hash_map()
{
    printf("Testing integer hash map...\n");

    // Initialize hash table
    khash_t(int32) *h = kh_init(int32);
    assert(h != NULL);
    assert(kh_size(h) == 0);

    // Test insertion
    int ret;
    khint_t k = kh_put(int32, h, 5, &ret);
    assert(ret == 1); // Should be a new insertion
    kh_value(h, k) = 10;
    assert(kh_size(h) == 1);

    // Test retrieval
    k = kh_get(int32, h, 5);
    assert(k != kh_end(h));
    assert(kh_value(h, k) == 10);

    // Test non-existent key
    k = kh_get(int32, h, 123);
    assert(k == kh_end(h));

    // Test update
    k = kh_put(int32, h, 5, &ret);
    assert(ret == 0); // Key already exists
    kh_value(h, k) = 20;
    assert(kh_value(h, k) == 20);

    // Test deletion
    k = kh_get(int32, h, 5);
    kh_del(int32, h, k);
    assert(kh_size(h) == 0);

    kh_destroy(int32, h);
    printf("Integer hash map tests passed!\n");
}

void test_string_hash_map()
{
    printf("Testing string hash map...\n");

    khash_t(str) *h = kh_init(str);
    assert(h != NULL);

    // Test string insertion
    int ret;
    khint_t k = kh_put(str, h, "hello", &ret);
    assert(ret == 1);
    kh_value(h, k) = 42;

    // Test string retrieval
    k = kh_get(str, h, "hello");
    assert(k != kh_end(h));
    assert(kh_value(h, k) == 42);

    // Test non-existent string
    k = kh_get(str, h, "world");
    assert(k == kh_end(h));

    kh_destroy(str, h);
    printf("String hash map tests passed!\n");
}

void test_int_set()
{
    printf("Testing integer set...\n");

    khash_t(intset) *h = kh_init(intset);
    assert(h != NULL);

    // Test insertion
    int ret;
    khint_t k = kh_put(intset, h, 100, &ret);
    assert(ret == 1);
    assert(kh_size(h) == 1);

    // Test membership
    k = kh_get(intset, h, 100);
    assert(k != kh_end(h));

    // Test non-membership
    k = kh_get(intset, h, 200);
    assert(k == kh_end(h));

    kh_destroy(intset, h);
    printf("Integer set tests passed!\n");
}

void test_resize()
{
    printf("Testing hash table resizing...\n");

    khash_t(int32) *h = kh_init(int32);
    int ret;

    // Insert many elements to trigger resize
    for (int i = 0; i < 1000; i++)
    {
        khint_t k = kh_put(int32, h, i, &ret);
        kh_value(h, k) = i * 10;
    }

    // Verify all elements after resize
    for (int i = 0; i < 1000; i++)
    {
        khint_t k = kh_get(int32, h, i);
        assert(k != kh_end(h));
        assert(kh_value(h, k) == i * 10);
    }

    kh_destroy(int32, h);
    printf("Resize tests passed!\n");
}

void test_iteration()
{
    printf("Testing hash table iteration...\n");

    khash_t(int32) *h = kh_init(int32);
    int ret;

    // Insert some elements
    for (int i = 0; i < 500; i++)
    {
        khint_t k = kh_put(int32, h, i, &ret);
        kh_value(h, k) = i * 10;
    }

    // Test iteration
    int count = 0;
    long sum = 0;
    int32_t key, value;
    kh_foreach(h, key, value, {
        assert(value == key * 10);
        count++;
        sum += value;
    });
    assert(count == 500);
    assert(sum == 1247500);

    kh_destroy(int32, h);
    printf("Iteration tests passed!\n");
}

void test_probe_statistics()
{
    printf("Testing hash table probe statistics...\n");

    khash_t(int32) *h = kh_init(int32);
    int ret;

    const int n_items = 10000;
    for (int i = 0; i < n_items; i++)
    {
        // Insert numbers that will map to same bucket (using modulo)
        int key = i * i + 1234;
        khint_t k = kh_put(int32, h, key, &ret);
        kh_value(h, k) = i;
    }

    // Get probe statistics
    kh_probe_stat_t stats = kh_probe_stats(int32, h);

    // Verify statistics
    printf("Probe Statistics:\n");
    printf("  Maximum probes: %d\n", stats.max_probes);
    printf("  Average probes: %.2f\n", stats.avg_probes);
    printf("  Probe variance: %.2f\n", stats.variance);

    // Test after deletion
    printf("\nTesting after deletions...\n");
    for (int i = 0; i < n_items / 3; i++)
    {
        int key = i * i + 1234;
        khint_t k = kh_get(int32, h, key);
        if (k != kh_end(h))
        {
            kh_del(int32, h, k);
        }
    }

    stats = kh_probe_stats(int32, h);
    printf("After deletions:\n");
    printf("  Maximum probes: %d\n", stats.max_probes);
    printf("  Average probes: %.2f\n", stats.avg_probes);
    printf("  Probe variance: %.2f\n", stats.variance);

    // Test after rehashing
    printf("\nTesting after rehashing...\n");
    kh_resize(int32, h, h->n_buckets - 1);
    stats = kh_probe_stats(int32, h);
    printf("After rehashing:\n");
    printf("  Maximum probes: %d\n", stats.max_probes);
    printf("  Average probes: %.2f\n", stats.avg_probes);
    printf("  Probe variance: %.2f\n", stats.variance);

    kh_destroy(int32, h);
    printf("Probe statistics tests passed!\n");
}

int main()
{
    printf("Starting khash.h unit tests...\n\n");

    test_int_hash_map();
    test_string_hash_map();
    test_int_set();
    test_resize();
    test_iteration();
    test_probe_statistics(); // Add the new test

    printf("\nAll tests passed successfully!\n");
    return 0;
}
