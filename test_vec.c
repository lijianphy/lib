#include <assert.h>
#include <stdio.h>
#include "vec.h"

// Define vector types for testing
VEC_IMPL(int, vec_int)
VEC_IMPL(double, vec_double)

void test_vec_init() {
    vec_int v;
    vec_int_init(&v);
    assert(v.size == 0);
    assert(v.capacity == 0);
    assert(v.data == NULL);
    vec_int_destroy(&v);
    printf("Init test passed\n");
}

void test_vec_push_pop() {
    vec_int v;
    vec_int_init(&v);
    
    // Test push
    for (int i = 0; i < 10; i++) {
        vec_int_push(&v, i);
        assert(v.size == (size_t)(i + 1));
        assert(vec_int_get(&v, i) == i);
    }
    
    // Test pop
    for (int i = 9; i >= 0; i--) {
        int val = vec_int_pop(&v);
        assert(val == i);
        assert(v.size == (size_t)i);
    }
    
    vec_int_destroy(&v);
    printf("Push/pop test passed\n");
}

void test_vec_get_set() {
    vec_int v;
    vec_int_init(&v);
    
    vec_int_push(&v, 42);
    vec_int_set(&v, 0, 24);
    assert(vec_int_get(&v, 0) == 24);
    
    vec_int_destroy(&v);
    printf("Get/set test passed\n");
}

void test_vec_copy_move() {
    vec_int src, dst;
    vec_int_init(&src);
    vec_int_init(&dst);
    
    // Test copy
    for (int i = 0; i < 5; i++) {
        vec_int_push(&src, i);
    }
    vec_int_copy(&dst, &src);
    assert(dst.size == src.size);
    for (int i = 0; i < 5; i++) {
        assert(vec_int_get(&dst, i) == vec_int_get(&src, i));
    }
    
    // Test move
    vec_int moved;
    vec_int_init(&moved);
    vec_int_move(&moved, &dst);
    assert(dst.data == NULL);
    assert(dst.size == 0);
    assert(moved.size == 5);
    
    vec_int_destroy(&src);
    vec_int_destroy(&dst);
    vec_int_destroy(&moved);
    printf("Copy/move test passed\n");
}

void test_memory_stress() {
    vec_int v;
    vec_int_init(&v);
    
    // Stress test with multiple resize operations
    for (int i = 0; i < 1000; i++) {
        vec_int_push(&v, i);
    }
    
    // Clear half the elements
    for (int i = 0; i < 500; i++) {
        vec_int_pop(&v);
    }
    
    // Push more elements
    for (int i = 0; i < 2000; i++) {
        vec_int_push(&v, i);
    }
    
    vec_int_destroy(&v);
    printf("Memory stress test passed\n");
}

int main() {
    test_vec_init();
    test_vec_push_pop();
    test_vec_get_set();
    test_vec_copy_move();
    test_memory_stress();
    printf("All tests passed!\n");
    return 0;
}
