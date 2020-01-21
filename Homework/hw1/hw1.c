/* Stephanie Yoshimoto, hw1.c, CS 24000, Spring 2020
 * Last updated January 17, 2020
 */

/* Add any includes here */

#include "hw1.h"

/*
 *  This function computes the hailstone numbers starting at the first
 *  parameter upto limit. It calculates the sum of the values upto the
 *  limit and stores the numbers in an array.
 */

int compute_hailstone(int starting_value, int limit) {
  int sum = starting_value;
  int n = starting_value;
  int i = 0;
  int temp = starting_value;

  if ((starting_value <= 0) || (limit <= 0) || (limit > ARRAY_SIZE)) {
    return HAILSTONE_ERROR;
  }

  g_hailstone_array[0] = n;
  for (i = 1; i < limit; i++) {
    if (n % 2 == 0) {
      temp = n / 2;
    } else {
      temp = (3 * n) + 1;
    }
    n = temp;
    g_hailstone_array[i] = temp;
    sum += temp;
  }

  return sum;
} /* compute_hailstone() */

/*
 *  This function checks if the numbers in an array upto the nth element,
 *  denoted by limit, form a Hailstone sequence. It returns correct, 0,
 *  if so, -1 if there is an error, and a positive integer indicating the
 *  index of the first element denoting the sequence is not a Hailstone
 *  sequence.
 */

int check_hailstone(int limit) {
  int i = 0;
  int value = g_hailstone_array[0];
  int temp = 0;
  if ((limit < 1) || (value < 1) || (limit > ARRAY_SIZE)) {
    return HAILSTONE_ERROR;
  }

  for (i = 1; i < limit; i++) {
    if (value % 2 == 0) {
      temp = value / 2;
    } else {
      temp = (3 * value) + 1;
    }

    if (temp != g_hailstone_array[i]) {
      return i;
    }

    value = g_hailstone_array[i];
  }

  return HAILSTONE_CORRECT;
} /* check_hailstone() */

/* Remember, you don't need a main function!
 * it is provided by hw1_main.c or hw1_test.o
 */
