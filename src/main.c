#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ll.h"


// function prototypes
void num_teardown(void *n);
void num_printer(void *n);
int num_equals_3(void *n);
void *test();
void *test_org();

// global variables
ll_t *list;


int main()
{
    int i;
    int num_threads = 3;
    pthread_t *threads;
    list = ll_new(num_teardown);
    list->val_printer = num_printer;

    int x1 = 1;
    int x2 = 2;
    int x3 = 23;
    int x4 = 10000;

    if (ll_insert_n(list, &x1, 0) != -1)
        puts("succeed!");
    if (ll_insert_n(list, &x2, 1) != -1)
        puts("succeed!");
    if (ll_insert_n(list, &x3, 2) != -1)
        puts("succeed!");
    if (ll_insert_n(list, &x4, 3) != -1)
        puts("succeed!");



    if ((threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t))) == NULL) {
        perror("malloc");
        exit(1);
    }

    for (i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, test, NULL) != 0) {
            fprintf(stderr, "Error creating thread\n");
            exit(1);
        }
    }

    for (i = 0; i < num_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error waiting for thread completion\n");
            exit(1);
        }
    }

    //ll_delete(list);

    return 0;
}

void num_teardown(void *n) 
{
    *(int *)n *= -1; // just so we can visually inspect removals afterwards
}

void num_printer(void *n) 
{
    printf(" %d", *(int *)n);
}

int num_equals_3(void *n) 
{
    return *(int *)n == 3;
}

void *test()
{
    ll_print(*list);

    return NULL;
}

void *test_org()
{
    int *_n; // for storing returned ones
    int test_count = 1;
    int fail_count = 0;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 3;
    int i = 3;
    
    if(list->step == 0){
        list->step++;
        ll_insert_first(list, &c); // 2 in front

        _n = (int *)ll_get_first(list);
        if (!(*_n == c)) {
            fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, c, *_n);
            fail_count++;
        } else
            fprintf(stderr, "PASS Test %d!\n", test_count);
        test_count++;
    }
    
    if(list->step == 1){
        list->step++;
        if (list->len != 1) {
            fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 1, list->len);
            fail_count++;
        } else
            fprintf(stderr, "PASS Test %d!\n", test_count);
        test_count++;
    }

    ll_insert_first(list, &b); // 1 in front
    ll_insert_first(list, &a); // 0 in front -> 0, 1, 2

    _n = (int *)ll_get_first(list);
    if (!(*_n == a)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, a, *_n);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 3)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 3, list->len);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_last(list, &d); // 3 in back
    ll_insert_last(list, &e); // 4 in back
    ll_insert_last(list, &f); // 5 in back

    _n = (int *)ll_get_n(list, 5);
    if (!(*_n == f)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, f, *_n);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    if (!(list->len == 6)) {
        fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 6, list->len);
        fail_count++;
    } else
        fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;

    ll_insert_n(list, &g, 6); // 6 at index 6 -> 0, 1, 2, 3, 4, 5, 6

    int _i;
    printf("len:%d\n", list->len);
    for (_i = 0; _i < list->len; _i++) { // O(n^2) test lol
        _n = (int *)ll_get_n(list, _i);
        if (!(*_n == _i)) {
            fail_count++;
            fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", 1, _i, *_n);
        } else
            fprintf(stderr, "PASS Test %d!\n", test_count);
        test_count++;
    }

    // (ll: 0 1 2 3 4 5 6), length: 7

    ll_remove_first(list);                // (ll: 1 2 3 4 5 6), length: 6
    ll_remove_n(list, 1);                 // (ll: 1 3 4 5 6),   length: 5
    ll_remove_n(list, 2);                 // (ll: 1 3 5 6),     length: 4
    ll_remove_n(list, 5);                 // (ll: 1 3 5 6),     length: 4; does nothing
    ll_remove_search(list, num_equals_3); // (ll: 1 5 6),       length: 3
    ll_insert_first(list, &h);            // (ll: 3 1 5 6),     length: 5
    ll_insert_last(list, &i);             // (ll: 3 1 5 6 3),   length: 5
    ll_remove_search(list, num_equals_3); // (ll: 1 5 6 3),     length: 4
    ll_remove_search(list, num_equals_3); // (ll: 1 5 6),       length: 3

    ll_print(*list);

    ll_delete(list);

    if (fail_count) {
        fprintf(stderr, "FAILED %d tests of %d.\n", fail_count, test_count);
        //return fail_count;
    }

    fprintf(stderr, "PASSED all %d tests!\n", test_count);
    return NULL;
}

