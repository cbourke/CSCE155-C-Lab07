/**
 * array_test_cmocka.c
 *
 * cmocka-based test suite for the following functions:
 *   - double getMean(const int *arr, int size)
 *   - int getMin(const int *arr, int size)
 *   - int getIndexOfMin(const int *arr, int size)
 *   - int getMax(const int *arr, int size)
 *   - int getIndexOfMax(const int *arr, int size)
 *   - int *filterThreshold(const int *arr, int size, int threshold, int *resultSize)
 *   - int **createMultiplicationTable(int n, int m)
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "array_utils.h"

/**
 * EPSILON
 *
 * Shared tolerance used for floating-point results.
 */
static const double EPSILON = 1e-9;

/* ------------------------------------------------------------------ */
/* Test utility functions (all prefixed with test_)                    */
/* ------------------------------------------------------------------ */

/**
 * test_format_int_array
 * ----------------------
 * Formats an int array into a human-readable string of the form
 * "[a, b, c]" for use in failure messages.
 *
 * buf      - destination buffer
 * bufsize  - size of destination buffer
 * arr      - array to format (may be NULL if size <= 0)
 * size     - number of elements in arr
 */
static void test_format_int_array(char *buf, size_t bufsize,
                                   const int *arr, int size)
{
    size_t offset = 0;
    int written;

    written = snprintf(buf + offset, bufsize - offset, "[");
    if (written > 0) {
        offset += (size_t)written;
    }

    for (int i = 0; i < size && offset < bufsize; i++) {
        written = snprintf(buf + offset, bufsize - offset, "%d%s",
                            arr[i], (i < size - 1) ? ", " : "");
        if (written > 0) {
            offset += (size_t)written;
        }
    }

    if (offset < bufsize) {
        snprintf(buf + offset, bufsize - offset, "]");
    }
}

/**
 * test_check_mean
 * ----------------
 * Calls getMean() on the given input, compares the result to the
 * expected value within a small epsilon, and fails (via fail_msg)
 * with a formatted message containing the input array, expected
 * mean, and actual mean if they do not match.
 *
 * arr      - input array
 * size     - number of elements in arr
 * expected - expected mean value
 */
static void test_check_mean(const int *arr, int size, double expected)
{
    double actual = getMean(arr, size);

    if (fabs(actual - expected) > EPSILON) {
        char arrbuf[1024];
        test_format_int_array(arrbuf, sizeof(arrbuf), arr, size);
        fail_msg("getMean() mismatch\n"
                  "  Input:    arr=%s, size=%d\n"
                  "  Expected: %f\n"
                  "  Actual:   %f",
                  arrbuf, size, expected, actual);
    }
}

/**
 * test_check_min
 * ----------------
 * Calls getMin() on the given input and fails (via fail_msg) with a
 * formatted message containing the input array, expected minimum,
 * and actual minimum if they do not match.
 */
static void test_check_min(const int *arr, int size, int expected)
{
    int actual = getMin(arr, size);

    if (actual != expected) {
        char arrbuf[1024];
        test_format_int_array(arrbuf, sizeof(arrbuf), arr, size);
        fail_msg("getMin() mismatch\n"
                  "  Input:    arr=%s, size=%d\n"
                  "  Expected: %d\n"
                  "  Actual:   %d",
                  arrbuf, size, expected, actual);
    }
}

/**
 * test_check_index_of_min
 * -------------------------
 * Calls getIndexOfMin() on the given input and fails (via fail_msg)
 * with a formatted message containing the input array, expected
 * index, and actual index if they do not match.
 */
static void test_check_index_of_min(const int *arr, int size, int expected)
{
    int actual = getIndexOfMin(arr, size);

    if (actual != expected) {
        char arrbuf[1024];
        test_format_int_array(arrbuf, sizeof(arrbuf), arr, size);
        fail_msg("getIndexOfMin() mismatch\n"
                  "  Input:    arr=%s, size=%d\n"
                  "  Expected index: %d\n"
                  "  Actual index:   %d",
                  arrbuf, size, expected, actual);
    }
}

/**
 * test_check_max
 * ----------------
 * Calls getMax() on the given input and fails (via fail_msg) with a
 * formatted message containing the input array, expected maximum,
 * and actual maximum if they do not match.
 */
static void test_check_max(const int *arr, int size, int expected)
{
    int actual = getMax(arr, size);

    if (actual != expected) {
        char arrbuf[1024];
        test_format_int_array(arrbuf, sizeof(arrbuf), arr, size);
        fail_msg("getMax() mismatch\n"
                  "  Input:    arr=%s, size=%d\n"
                  "  Expected: %d\n"
                  "  Actual:   %d",
                  arrbuf, size, expected, actual);
    }
}

/**
 * test_check_index_of_max
 * -------------------------
 * Calls getIndexOfMax() on the given input and fails (via fail_msg)
 * with a formatted message containing the input array, expected
 * index, and actual index if they do not match.
 */
static void test_check_index_of_max(const int *arr, int size, int expected)
{
    int actual = getIndexOfMax(arr, size);

    if (actual != expected) {
        char arrbuf[1024];
        test_format_int_array(arrbuf, sizeof(arrbuf), arr, size);
        fail_msg("getIndexOfMax() mismatch\n"
                  "  Input:    arr=%s, size=%d\n"
                  "  Expected index: %d\n"
                  "  Actual index:   %d",
                  arrbuf, size, expected, actual);
    }
}

/**
 * test_check_filter_threshold
 * -----------------------------
 * Calls filterThreshold() on the given input, compares the returned
 * array and resultSize against the expected array/size, and fails
 * (via fail_msg) with a formatted message containing the input,
 * expected output, and actual output if they do not match. Frees
 * the array returned by filterThreshold() before returning.
 */
static void test_check_filter_threshold(const int *arr, int size,
                                         int threshold,
                                         const int *expected,
                                         int expectedSize)
{
    int resultSize = -1;
    int *actual = filterThreshold(arr, size, threshold, &resultSize);
    int mismatch = 0;

    if (resultSize != expectedSize) {
        mismatch = 1;
    } else {
        for (int i = 0; i < expectedSize; i++) {
            if (actual == NULL || actual[i] != expected[i]) {
                mismatch = 1;
                break;
            }
        }
    }

    if (mismatch) {
        char inbuf[1024];
        char expbuf[1024];
        char actbuf[1024];

        test_format_int_array(inbuf, sizeof(inbuf), arr, size);
        test_format_int_array(expbuf, sizeof(expbuf), expected, expectedSize);
        test_format_int_array(actbuf, sizeof(actbuf), actual, resultSize);

        if (actual != NULL) {
            free(actual);
        }

        fail_msg("filterThreshold() mismatch\n"
                  "  Input:    arr=%s, size=%d, threshold=%d\n"
                  "  Expected: %s (resultSize=%d)\n"
                  "  Actual:   %s (resultSize=%d)",
                  inbuf, size, threshold,
                  expbuf, expectedSize,
                  actbuf, resultSize);
    }

    if (actual != NULL) {
        free(actual);
    }
}

/**
 * test_check_multiplication_table
 * ---------------------------------
 * Calls createMultiplicationTable() with the given dimensions,
 * verifies every cell equals (row+1)*(col+1), and fails (via
 * fail_msg) with a formatted message containing the inputs, the
 * expected value, and the actual value of the first mismatching
 * cell found. Frees the returned table before returning.
 */
static void test_check_multiplication_table(int n, int m)
{
    int **table = createMultiplicationTable(n, m);
    int mismatch_row = -1;
    int mismatch_col = -1;
    int expected_val = 0;
    int actual_val = 0;
    int mismatch = 0;

    for (int i = 0; i < n && !mismatch; i++) {
        for (int j = 0; j < m; j++) {
            int expected = (i + 1) * (j + 1);
            int actual = table[i][j];
            if (actual != expected) {
                mismatch = 1;
                mismatch_row = i;
                mismatch_col = j;
                expected_val = expected;
                actual_val = actual;
                break;
            }
        }
    }

    if (mismatch) {
        for (int i = 0; i < n; i++) {
            free(table[i]);
        }
        free(table);

        fail_msg("createMultiplicationTable() mismatch\n"
                  "  Input:    n=%d, m=%d\n"
                  "  Cell:     row=%d, col=%d\n"
                  "  Expected: %d\n"
                  "  Actual:   %d",
                  n, m, mismatch_row, mismatch_col,
                  expected_val, actual_val);
    }

    for (int i = 0; i < n; i++) {
        free(table[i]);
    }
    free(table);
}

/* ------------------------------------------------------------------ */
/* getMean tests                                                       */
/* ------------------------------------------------------------------ */

/**
 * test_getMean_positive_numbers
 *
 * Verifies the mean of a typical array of positive integers.
 *
 * Input:    {2, 4, 6, 8, 10}
 * Expected: 6.0
 */
static void test_getMean_positive_numbers(void **state)
{
    (void)state;
    int arr[] = {2, 4, 6, 8, 10};
    test_check_mean(arr, 5, 6.0);
}

/**
 * test_getMean_single_element
 *
 * Verifies the mean of a single-element array equals that element,
 * since the average of one value is trivially itself.
 *
 * Input:    {42}
 * Expected: 42.0
 */
static void test_getMean_single_element(void **state)
{
    (void)state;
    int arr[] = {42};
    test_check_mean(arr, 1, 42.0);
}

/** Verifies the mean of an array of all zeros is zero. */
static void test_getMean_all_zeros(void **state)
{
    (void)state;
    int arr[] = {0, 0, 0, 0};
    test_check_mean(arr, 4, 0.0);
}

/** Verifies the mean of an array of negative numbers. */
static void test_getMean_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-2, -4, -6, -8};
    test_check_mean(arr, 4, -5.0);
}

/** Verifies the mean of an array with both positive and negative values. */
static void test_getMean_mixed_positive_negative(void **state)
{
    (void)state;
    int arr[] = {-10, 5, 15, -20, 30};
    test_check_mean(arr, 5, 4.0);
}

/** Verifies the mean of a two-element array. */
static void test_getMean_two_elements(void **state)
{
    (void)state;
    int arr[] = {3, 8};
    test_check_mean(arr, 2, 5.5);
}

/** Verifies the mean of a larger array of consecutive integers. */
static void test_getMean_large_array(void **state)
{
    (void)state;
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = i + 1; /* 1..100 */
    }
    test_check_mean(arr, 100, 50.5);
}

/** Verifies the mean correctly produces a non-integer result. */
static void test_getMean_non_integer_result(void **state)
{
    (void)state;
    int arr[] = {1, 2, 4};
    test_check_mean(arr, 3, 7.0 / 3.0);
}

/** Verifies the mean of an array where every element is identical. */
static void test_getMean_all_same_value(void **state)
{
    (void)state;
    int arr[] = {7, 7, 7, 7, 7};
    test_check_mean(arr, 5, 7.0);
}

/** Verifies the mean of an array with alternating signs sums to zero mean. */
static void test_getMean_alternating_signs(void **state)
{
    (void)state;
    int arr[] = {5, -5, 5, -5, 5, -5};
    test_check_mean(arr, 6, 0.0);
}

/** Verifies the mean of an array containing large magnitude values. */
static void test_getMean_large_values(void **state)
{
    (void)state;
    int arr[] = {1000000, 2000000, 3000000};
    test_check_mean(arr, 3, 2000000.0);
}

/* ------------------------------------------------------------------ */
/* getMin tests                                                        */
/* ------------------------------------------------------------------ */

/**
 * test_getMin_at_start
 *
 * Verifies getMin finds the minimum when it is the first element,
 * guarding against implementations that skip index 0 while
 * initializing their running minimum.
 *
 * Input:    {1, 5, 9, 3, 7}
 * Expected: 1
 */
static void test_getMin_at_start(void **state)
{
    (void)state;
    int arr[] = {1, 5, 9, 3, 7};
    test_check_min(arr, 5, 1);
}

/**
 * test_getMin_at_end
 *
 * Verifies getMin finds the minimum when it is the last element,
 * ensuring the full array is scanned rather than stopping early.
 *
 * Input:    {5, 9, 3, 7, 1}
 * Expected: 1
 */
static void test_getMin_at_end(void **state)
{
    (void)state;
    int arr[] = {5, 9, 3, 7, 1};
    test_check_min(arr, 5, 1);
}

/** Verifies getMin finds the minimum when it is in the middle. */
static void test_getMin_in_middle(void **state)
{
    (void)state;
    int arr[] = {5, 9, 1, 7, 3};
    test_check_min(arr, 5, 1);
}

/** Verifies getMin correctly handles negative numbers. */
static void test_getMin_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-3, -7, -1, -9, -4};
    test_check_min(arr, 5, -9);
}

/** Verifies getMin on a single-element array returns that element. */
static void test_getMin_single_element(void **state)
{
    (void)state;
    int arr[] = {17};
    test_check_min(arr, 1, 17);
}

/** Verifies getMin on an array where all values are identical. */
static void test_getMin_all_same(void **state)
{
    (void)state;
    int arr[] = {4, 4, 4, 4};
    test_check_min(arr, 4, 4);
}

/** Verifies getMin when the minimum value appears multiple times. */
static void test_getMin_duplicates_of_min(void **state)
{
    (void)state;
    int arr[] = {2, -1, 5, -1, 8};
    test_check_min(arr, 5, -1);
}

/** Verifies getMin on a large array of consecutive integers. */
static void test_getMin_large_array(void **state)
{
    (void)state;
    int arr[200];
    for (int i = 0; i < 200; i++) {
        arr[i] = 200 - i; /* 200 down to 1 */
    }
    test_check_min(arr, 200, 1);
}

/** Verifies getMin on an array containing a mix of positive, negative, and zero. */
static void test_getMin_mixed_with_zero(void **state)
{
    (void)state;
    int arr[] = {3, -2, 0, 5, -8, 1};
    test_check_min(arr, 6, -8);
}

/** Verifies getMin on a strictly descending sorted array. */
static void test_getMin_descending_sorted(void **state)
{
    (void)state;
    int arr[] = {9, 7, 5, 3, 1};
    test_check_min(arr, 5, 1);
}

/** Verifies getMin on a strictly ascending sorted array. */
static void test_getMin_ascending_sorted(void **state)
{
    (void)state;
    int arr[] = {1, 3, 5, 7, 9};
    test_check_min(arr, 5, 1);
}

/* ------------------------------------------------------------------ */
/* getIndexOfMin tests                                                 */
/* ------------------------------------------------------------------ */

/**
 * test_getIndexOfMin_at_start
 *
 * Verifies getIndexOfMin returns 0 when the minimum is the first
 * element of the array.
 *
 * Input:    {1, 5, 9, 3, 7}
 * Expected index: 0
 */
static void test_getIndexOfMin_at_start(void **state)
{
    (void)state;
    int arr[] = {1, 5, 9, 3, 7};
    test_check_index_of_min(arr, 5, 0);
}

/** Verifies getIndexOfMin returns the last index when the minimum is last. */
static void test_getIndexOfMin_at_end(void **state)
{
    (void)state;
    int arr[] = {5, 9, 3, 7, 1};
    test_check_index_of_min(arr, 5, 4);
}

/** Verifies getIndexOfMin returns the correct index when minimum is in the middle. */
static void test_getIndexOfMin_in_middle(void **state)
{
    (void)state;
    int arr[] = {5, 9, 1, 7, 3};
    test_check_index_of_min(arr, 5, 2);
}

/** Verifies getIndexOfMin on a single-element array returns index 0. */
static void test_getIndexOfMin_single_element(void **state)
{
    (void)state;
    int arr[] = {99};
    test_check_index_of_min(arr, 1, 0);
}

/**
 * test_getIndexOfMin_duplicates_first_occurrence
 *
 * Verifies getIndexOfMin returns the index of the first occurrence
 * of the minimum value when that value appears more than once in
 * the array, rather than a later occurrence.
 *
 * Input:    {4, 1, 6, 1, 8}
 * Expected index: 1
 */
static void test_getIndexOfMin_duplicates_first_occurrence(void **state)
{
    (void)state;
    int arr[] = {4, 1, 6, 1, 8};
    test_check_index_of_min(arr, 5, 1);
}

/** Verifies getIndexOfMin correctly handles negative numbers. */
static void test_getIndexOfMin_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-2, -8, -3, -1};
    test_check_index_of_min(arr, 4, 1);
}

/** Verifies getIndexOfMin returns index 0 when all elements are identical. */
static void test_getIndexOfMin_all_same_value(void **state)
{
    (void)state;
    int arr[] = {6, 6, 6, 6};
    test_check_index_of_min(arr, 4, 0);
}

/** Verifies getIndexOfMin on a strictly descending sorted array. */
static void test_getIndexOfMin_descending_sorted(void **state)
{
    (void)state;
    int arr[] = {9, 7, 5, 3, 1};
    test_check_index_of_min(arr, 5, 4);
}

/** Verifies getIndexOfMin on a strictly ascending sorted array. */
static void test_getIndexOfMin_ascending_sorted(void **state)
{
    (void)state;
    int arr[] = {1, 3, 5, 7, 9};
    test_check_index_of_min(arr, 5, 0);
}

/** Verifies getIndexOfMin on a larger array of consecutive integers. */
static void test_getIndexOfMin_large_array(void **state)
{
    (void)state;
    int arr[150];
    for (int i = 0; i < 150; i++) {
        arr[i] = i + 10;
    }
    test_check_index_of_min(arr, 150, 0);
}

/* ------------------------------------------------------------------ */
/* getMax tests                                                        */
/* ------------------------------------------------------------------ */

/**
 * test_getMax_at_start
 *
 * Verifies getMax finds the maximum when it is the first element,
 * guarding against implementations that skip index 0 while
 * initializing their running maximum.
 *
 * Input:    {9, 5, 1, 3, 7}
 * Expected: 9
 */
static void test_getMax_at_start(void **state)
{
    (void)state;
    int arr[] = {9, 5, 1, 3, 7};
    test_check_max(arr, 5, 9);
}

/** Verifies getMax finds the maximum when it is the last element. */
static void test_getMax_at_end(void **state)
{
    (void)state;
    int arr[] = {5, 1, 3, 7, 9};
    test_check_max(arr, 5, 9);
}

/** Verifies getMax finds the maximum when it is in the middle. */
static void test_getMax_in_middle(void **state)
{
    (void)state;
    int arr[] = {5, 1, 9, 7, 3};
    test_check_max(arr, 5, 9);
}

/**
 * test_getMax_negative_numbers
 *
 * Verifies getMax correctly handles an array of entirely negative
 * numbers, where the "maximum" is the value closest to zero.
 *
 * Input:    {-3, -7, -1, -9, -4}
 * Expected: -1
 */
static void test_getMax_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-3, -7, -1, -9, -4};
    test_check_max(arr, 5, -1);
}

/** Verifies getMax on a single-element array returns that element. */
static void test_getMax_single_element(void **state)
{
    (void)state;
    int arr[] = {23};
    test_check_max(arr, 1, 23);
}

/** Verifies getMax on an array where all values are identical. */
static void test_getMax_all_same(void **state)
{
    (void)state;
    int arr[] = {8, 8, 8, 8};
    test_check_max(arr, 4, 8);
}

/** Verifies getMax when the maximum value appears multiple times. */
static void test_getMax_duplicates_of_max(void **state)
{
    (void)state;
    int arr[] = {2, 9, 5, 9, 1};
    test_check_max(arr, 5, 9);
}

/** Verifies getMax on a large array of consecutive integers. */
static void test_getMax_large_array(void **state)
{
    (void)state;
    int arr[200];
    for (int i = 0; i < 200; i++) {
        arr[i] = i + 1; /* 1..200 */
    }
    test_check_max(arr, 200, 200);
}

/** Verifies getMax on an array containing a mix of positive, negative, and zero. */
static void test_getMax_mixed_with_zero(void **state)
{
    (void)state;
    int arr[] = {-3, -2, 0, -5, -8, -1};
    test_check_max(arr, 6, 0);
}

/** Verifies getMax on a strictly descending sorted array. */
static void test_getMax_descending_sorted(void **state)
{
    (void)state;
    int arr[] = {9, 7, 5, 3, 1};
    test_check_max(arr, 5, 9);
}

/** Verifies getMax on a strictly ascending sorted array. */
static void test_getMax_ascending_sorted(void **state)
{
    (void)state;
    int arr[] = {1, 3, 5, 7, 9};
    test_check_max(arr, 5, 9);
}

/* ------------------------------------------------------------------ */
/* getIndexOfMax tests                                                 */
/* ------------------------------------------------------------------ */

/**
 * test_getIndexOfMax_at_start
 *
 * Verifies getIndexOfMax returns 0 when the maximum is the first
 * element of the array.
 *
 * Input:    {9, 5, 1, 3, 7}
 * Expected index: 0
 */
static void test_getIndexOfMax_at_start(void **state)
{
    (void)state;
    int arr[] = {9, 5, 1, 3, 7};
    test_check_index_of_max(arr, 5, 0);
}

/** Verifies getIndexOfMax returns the last index when the maximum is last. */
static void test_getIndexOfMax_at_end(void **state)
{
    (void)state;
    int arr[] = {5, 1, 3, 7, 9};
    test_check_index_of_max(arr, 5, 4);
}

/** Verifies getIndexOfMax returns the correct index when maximum is in the middle. */
static void test_getIndexOfMax_in_middle(void **state)
{
    (void)state;
    int arr[] = {5, 1, 9, 7, 3};
    test_check_index_of_max(arr, 5, 2);
}

/** Verifies getIndexOfMax on a single-element array returns index 0. */
static void test_getIndexOfMax_single_element(void **state)
{
    (void)state;
    int arr[] = {55};
    test_check_index_of_max(arr, 1, 0);
}

/**
 * test_getIndexOfMax_duplicates_first_occurrence
 *
 * Verifies getIndexOfMax returns the index of the first occurrence
 * of the maximum value when that value appears more than once in
 * the array, rather than a later occurrence.
 *
 * Input:    {4, 9, 6, 9, 8}
 * Expected index: 1
 */
static void test_getIndexOfMax_duplicates_first_occurrence(void **state)
{
    (void)state;
    int arr[] = {4, 9, 6, 9, 8};
    test_check_index_of_max(arr, 5, 1);
}

/** Verifies getIndexOfMax correctly handles negative numbers. */
static void test_getIndexOfMax_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-2, -8, -1, -9};
    test_check_index_of_max(arr, 4, 2);
}

/** Verifies getIndexOfMax returns index 0 when all elements are identical. */
static void test_getIndexOfMax_all_same_value(void **state)
{
    (void)state;
    int arr[] = {6, 6, 6, 6};
    test_check_index_of_max(arr, 4, 0);
}

/** Verifies getIndexOfMax on a strictly descending sorted array. */
static void test_getIndexOfMax_descending_sorted(void **state)
{
    (void)state;
    int arr[] = {9, 7, 5, 3, 1};
    test_check_index_of_max(arr, 5, 0);
}

/** Verifies getIndexOfMax on a strictly ascending sorted array. */
static void test_getIndexOfMax_ascending_sorted(void **state)
{
    (void)state;
    int arr[] = {1, 3, 5, 7, 9};
    test_check_index_of_max(arr, 5, 4);
}

/** Verifies getIndexOfMax on a larger array of consecutive integers. */
static void test_getIndexOfMax_large_array(void **state)
{
    (void)state;
    int arr[150];
    for (int i = 0; i < 150; i++) {
        arr[i] = i + 10;
    }
    test_check_index_of_max(arr, 150, 149);
}

/* ------------------------------------------------------------------ */
/* filterThreshold tests                                               */
/* ------------------------------------------------------------------ */

/**
 * test_filterThreshold_spec_example
 *
 * Verifies the canonical example given in the function
 * specification, where elements greater than or equal to a
 * threshold of 10 are kept in their original relative order.
 *
 * Input:     {10, 5, 32, 8, 7, 28, 15, 12}, threshold = 10
 * Expected:  {10, 32, 28, 15, 12}, resultSize = 5
 */
static void test_filterThreshold_spec_example(void **state)
{
    (void)state;
    int arr[] = {10, 5, 32, 8, 7, 28, 15, 12};
    int expected[] = {10, 32, 28, 15, 12};
    test_check_filter_threshold(arr, 8, 10, expected, 5);
}

/** Verifies filterThreshold returns an empty array when threshold exceeds all elements. */
static void test_filterThreshold_threshold_above_all(void **state)
{
    (void)state;
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {0}; /* unused, expectedSize is 0 */
    test_check_filter_threshold(arr, 5, 100, expected, 0);
}

/** Verifies filterThreshold returns the full array when threshold is below all elements. */
static void test_filterThreshold_threshold_below_all(void **state)
{
    (void)state;
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};
    test_check_filter_threshold(arr, 5, -10, expected, 5);
}

/** Verifies filterThreshold includes elements exactly equal to the threshold. */
static void test_filterThreshold_equal_to_threshold(void **state)
{
    (void)state;
    int arr[] = {5, 10, 15, 10, 20};
    int expected[] = {10, 15, 10, 20};
    test_check_filter_threshold(arr, 5, 10, expected, 4);
}

/** Verifies filterThreshold when every element equals the threshold. */
static void test_filterThreshold_all_equal_threshold(void **state)
{
    (void)state;
    int arr[] = {7, 7, 7, 7};
    int expected[] = {7, 7, 7, 7};
    test_check_filter_threshold(arr, 4, 7, expected, 4);
}

/**
 * test_filterThreshold_negative_numbers
 *
 * Verifies filterThreshold behaves correctly when both the input
 * array and the threshold contain negative values, confirming the
 * ">= threshold" comparison still works below zero.
 *
 * Input:     {-10, -5, -1, -8, 0, 3}, threshold = -8
 * Expected:  {-5, -1, -8, 0, 3}, resultSize = 5
 */
static void test_filterThreshold_negative_numbers(void **state)
{
    (void)state;
    int arr[] = {-10, -5, -1, -8, 0, 3};
    int expected[] = {-5, -1, -8, 0, 3};
    test_check_filter_threshold(arr, 6, -8, expected, 5);
}

/** Verifies filterThreshold on an empty input array (size 0). */
static void test_filterThreshold_empty_array(void **state)
{
    (void)state;
    int *arr = NULL;
    int expected[] = {0}; /* unused, expectedSize is 0 */
    test_check_filter_threshold(arr, 0, 5, expected, 0);
}

/** Verifies filterThreshold on a single-element array that passes the threshold. */
static void test_filterThreshold_single_element_passes(void **state)
{
    (void)state;
    int arr[] = {50};
    int expected[] = {50};
    test_check_filter_threshold(arr, 1, 10, expected, 1);
}

/** Verifies filterThreshold on a single-element array that fails the threshold. */
static void test_filterThreshold_single_element_fails(void **state)
{
    (void)state;
    int arr[] = {5};
    int expected[] = {0}; /* unused, expectedSize is 0 */
    test_check_filter_threshold(arr, 1, 10, expected, 0);
}

/** Verifies filterThreshold correctly preserves duplicate qualifying values. */
static void test_filterThreshold_duplicates(void **state)
{
    (void)state;
    int arr[] = {3, 8, 8, 2, 8, 1};
    int expected[] = {8, 8, 8};
    test_check_filter_threshold(arr, 6, 8, expected, 3);
}

/** Verifies filterThreshold with a threshold of zero on mixed sign values. */
static void test_filterThreshold_zero_threshold(void **state)
{
    (void)state;
    int arr[] = {-3, 0, 5, -1, 2, -7};
    int expected[] = {0, 5, 2};
    test_check_filter_threshold(arr, 6, 0, expected, 3);
}

/* ------------------------------------------------------------------ */
/* createMultiplicationTable tests                                     */
/* ------------------------------------------------------------------ */

/**
 * test_createMultiplicationTable_spec_example
 *
 * Verifies the canonical 3x5 example from the function
 * specification, checking every cell against the expected
 * multiplication table:
 *
 *   [ 1   2   3   4   5 ]
 *   [ 2   4   6   8  10 ]
 *   [ 3   6   9  12  15 ]
 *
 * Input: n = 3, m = 5
 */
static void test_createMultiplicationTable_spec_example(void **state)
{
    (void)state;
    test_check_multiplication_table(3, 5);
}

/** Verifies a square multiplication table (n == m). */
static void test_createMultiplicationTable_square(void **state)
{
    (void)state;
    test_check_multiplication_table(4, 4);
}

/** Verifies the smallest possible table, 1x1. */
static void test_createMultiplicationTable_1x1(void **state)
{
    (void)state;
    test_check_multiplication_table(1, 1);
}

/** Verifies a single-row table (n = 1, m > 1). */
static void test_createMultiplicationTable_single_row(void **state)
{
    (void)state;
    test_check_multiplication_table(1, 6);
}

/** Verifies a single-column table (n > 1, m = 1). */
static void test_createMultiplicationTable_single_column(void **state)
{
    (void)state;
    test_check_multiplication_table(6, 1);
}

/**
 * test_createMultiplicationTable_larger_square
 *
 * Verifies a larger 10x10 square table, checking every cell
 * including the diagonal, to catch off-by-one errors that might
 * not surface on the small 3x5 spec example.
 *
 * Input: n = 10, m = 10
 */
static void test_createMultiplicationTable_larger_square(void **state)
{
    (void)state;
    test_check_multiplication_table(10, 10);
}

/** Verifies a rectangular table that is taller than it is wide (n > m). */
static void test_createMultiplicationTable_tall_rectangle(void **state)
{
    (void)state;
    test_check_multiplication_table(7, 3);
}

/** Verifies a rectangular table that is wider than it is tall (m > n). */
static void test_createMultiplicationTable_wide_rectangle(void **state)
{
    (void)state;
    test_check_multiplication_table(3, 7);
}

/** Verifies a 2x2 table explicitly. */
static void test_createMultiplicationTable_2x2(void **state)
{
    (void)state;
    test_check_multiplication_table(2, 2);
}

/** Verifies the first row of a table equals 1..m directly (spot check). */
static void test_createMultiplicationTable_first_row_values(void **state)
{
    (void)state;
    int n = 5, m = 6;
    int **table = createMultiplicationTable(n, m);

    for (int j = 0; j < m; j++) {
        if (table[0][j] != j + 1) {
            int expected = j + 1;
            int actual = table[0][j];
            for (int i = 0; i < n; i++) {
                free(table[i]);
            }
            free(table);
            fail_msg("createMultiplicationTable() first-row mismatch\n"
                      "  Input:    n=%d, m=%d\n"
                      "  Cell:     row=0, col=%d\n"
                      "  Expected: %d\n"
                      "  Actual:   %d",
                      n, m, j, expected, actual);
        }
    }

    for (int i = 0; i < n; i++) {
        free(table[i]);
    }
    free(table);
}

/** Verifies the first column of a table equals 1..n directly (spot check). */
static void test_createMultiplicationTable_first_column_values(void **state)
{
    (void)state;
    int n = 6, m = 5;
    int **table = createMultiplicationTable(n, m);

    for (int i = 0; i < n; i++) {
        if (table[i][0] != i + 1) {
            int expected = i + 1;
            int actual = table[i][0];
            for (int k = 0; k < n; k++) {
                free(table[k]);
            }
            free(table);
            fail_msg("createMultiplicationTable() first-column mismatch\n"
                      "  Input:    n=%d, m=%d\n"
                      "  Cell:     row=%d, col=0\n"
                      "  Expected: %d\n"
                      "  Actual:   %d",
                      n, m, i, expected, actual);
        }
    }

    for (int i = 0; i < n; i++) {
        free(table[i]);
    }
    free(table);
}

/* ------------------------------------------------------------------ */
/* Test runner                                                         */
/* ------------------------------------------------------------------ */

int main(void)
{
    const struct CMUnitTest tests[] = {
        /* getMean */
        cmocka_unit_test(test_getMean_positive_numbers),
        cmocka_unit_test(test_getMean_single_element),
        cmocka_unit_test(test_getMean_all_zeros),
        cmocka_unit_test(test_getMean_negative_numbers),
        cmocka_unit_test(test_getMean_mixed_positive_negative),
        cmocka_unit_test(test_getMean_two_elements),
        cmocka_unit_test(test_getMean_large_array),
        cmocka_unit_test(test_getMean_non_integer_result),
        cmocka_unit_test(test_getMean_all_same_value),
        cmocka_unit_test(test_getMean_alternating_signs),
        cmocka_unit_test(test_getMean_large_values),

        /* getMin */
        cmocka_unit_test(test_getMin_at_start),
        cmocka_unit_test(test_getMin_at_end),
        cmocka_unit_test(test_getMin_in_middle),
        cmocka_unit_test(test_getMin_negative_numbers),
        cmocka_unit_test(test_getMin_single_element),
        cmocka_unit_test(test_getMin_all_same),
        cmocka_unit_test(test_getMin_duplicates_of_min),
        cmocka_unit_test(test_getMin_large_array),
        cmocka_unit_test(test_getMin_mixed_with_zero),
        cmocka_unit_test(test_getMin_descending_sorted),
        cmocka_unit_test(test_getMin_ascending_sorted),

        /* getIndexOfMin */
        cmocka_unit_test(test_getIndexOfMin_at_start),
        cmocka_unit_test(test_getIndexOfMin_at_end),
        cmocka_unit_test(test_getIndexOfMin_in_middle),
        cmocka_unit_test(test_getIndexOfMin_single_element),
        cmocka_unit_test(test_getIndexOfMin_duplicates_first_occurrence),
        cmocka_unit_test(test_getIndexOfMin_negative_numbers),
        cmocka_unit_test(test_getIndexOfMin_all_same_value),
        cmocka_unit_test(test_getIndexOfMin_descending_sorted),
        cmocka_unit_test(test_getIndexOfMin_ascending_sorted),
        cmocka_unit_test(test_getIndexOfMin_large_array),

        /* getMax */
        cmocka_unit_test(test_getMax_at_start),
        cmocka_unit_test(test_getMax_at_end),
        cmocka_unit_test(test_getMax_in_middle),
        cmocka_unit_test(test_getMax_negative_numbers),
        cmocka_unit_test(test_getMax_single_element),
        cmocka_unit_test(test_getMax_all_same),
        cmocka_unit_test(test_getMax_duplicates_of_max),
        cmocka_unit_test(test_getMax_large_array),
        cmocka_unit_test(test_getMax_mixed_with_zero),
        cmocka_unit_test(test_getMax_descending_sorted),
        cmocka_unit_test(test_getMax_ascending_sorted),

        /* getIndexOfMax */
        cmocka_unit_test(test_getIndexOfMax_at_start),
        cmocka_unit_test(test_getIndexOfMax_at_end),
        cmocka_unit_test(test_getIndexOfMax_in_middle),
        cmocka_unit_test(test_getIndexOfMax_single_element),
        cmocka_unit_test(test_getIndexOfMax_duplicates_first_occurrence),
        cmocka_unit_test(test_getIndexOfMax_negative_numbers),
        cmocka_unit_test(test_getIndexOfMax_all_same_value),
        cmocka_unit_test(test_getIndexOfMax_descending_sorted),
        cmocka_unit_test(test_getIndexOfMax_ascending_sorted),
        cmocka_unit_test(test_getIndexOfMax_large_array),

        /* filterThreshold */
        cmocka_unit_test(test_filterThreshold_spec_example),
        cmocka_unit_test(test_filterThreshold_threshold_above_all),
        cmocka_unit_test(test_filterThreshold_threshold_below_all),
        cmocka_unit_test(test_filterThreshold_equal_to_threshold),
        cmocka_unit_test(test_filterThreshold_all_equal_threshold),
        cmocka_unit_test(test_filterThreshold_negative_numbers),
        cmocka_unit_test(test_filterThreshold_empty_array),
        cmocka_unit_test(test_filterThreshold_single_element_passes),
        cmocka_unit_test(test_filterThreshold_single_element_fails),
        cmocka_unit_test(test_filterThreshold_duplicates),
        cmocka_unit_test(test_filterThreshold_zero_threshold),

        /* createMultiplicationTable */
        cmocka_unit_test(test_createMultiplicationTable_spec_example),
        cmocka_unit_test(test_createMultiplicationTable_square),
        cmocka_unit_test(test_createMultiplicationTable_1x1),
        cmocka_unit_test(test_createMultiplicationTable_single_row),
        cmocka_unit_test(test_createMultiplicationTable_single_column),
        cmocka_unit_test(test_createMultiplicationTable_larger_square),
        cmocka_unit_test(test_createMultiplicationTable_tall_rectangle),
        cmocka_unit_test(test_createMultiplicationTable_wide_rectangle),
        cmocka_unit_test(test_createMultiplicationTable_2x2),
        cmocka_unit_test(test_createMultiplicationTable_first_row_values),
        cmocka_unit_test(test_createMultiplicationTable_first_column_values),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
