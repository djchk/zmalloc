#include <assert.h>
#include "zmalloc.h"

void test_new_alloc_from_os() {
    int *num = (int *) zmalloc(sizeof(int), FIRST_FIT);
    zfree(num);
}

void test_reuse_single_block() {
    int *num1 = (int *) zmalloc(sizeof(int), FIRST_FIT);
    assert(num1 != NULL);
    zfree(num1);

    int *num2 = (int *) zmalloc(sizeof(int), FIRST_FIT);
    assert(num1 == num2);
    zfree(num2);
}

void test_reuse_first_fit() {
    int *num1 = (int *)zmalloc(sizeof(int), FIRST_FIT);
    assert(num1 != NULL);
    int *num2 = (int *)zmalloc(sizeof(int) * 3, FIRST_FIT);
    assert(num2 != NULL);
    int *num3 = (int *)zmalloc(sizeof(int) * 2, FIRST_FIT);
    assert(num3 != NULL);

    zfree(num2);
    zfree(num3);

    int *num4 = (int *)zmalloc(sizeof(int), FIRST_FIT);
    assert(num4 == num2);

    zfree(num1);
    zfree(num4);
}

void test_reuse_best_fit() {
    int *num1 = (int *)zmalloc(sizeof(int), BEST_FIT);      // 4
    assert(num1 != NULL);
    int *num2 = (int *)zmalloc(sizeof(int) * 3, BEST_FIT);      // 12
    assert(num2 != NULL); 
    int *num3 = (int *)zmalloc(sizeof(int) * 2, BEST_FIT);      // 8
    assert(num3 != NULL);

    zfree(num2);
    zfree(num3);

    int *num4 = (int *)zmalloc(sizeof(int), BEST_FIT);
    assert(num4 == num3);

    zfree(num1);
    zfree(num4);
}

int main() {
    // test_new_alloc_from_os();
    // test_reuse_first_fit();
    // test_reuse_single_block();
    test_reuse_best_fit();
}