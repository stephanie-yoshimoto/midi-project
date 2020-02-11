/* Stephanie Yoshimoto, hw4.c, CS 24000, Fall 2020
 * Last updated February 5, 2020
 */

#include "hw4.h"

#include <math.h>

complex_t add_complex(complex_t complex_1, complex_t complex_2) {
  complex_1.x += complex_2.x;
  complex_1.y += complex_2.y;
  return complex_1;
} /* add_complex() */

complex_t neg_complex(complex_t complex_num) {
  complex_num.x *= -1;
  complex_num.y *= -1;
  return complex_num;
} /* neg_complex() */

complex_t sub_complex(complex_t complex_1, complex_t complex_2) {
  return add_complex(complex_1, neg_complex(complex_2));
} /* sub_complex() */

double dot_complex(complex_t complex_1, complex_t complex_2) {
  return (complex_1.x * complex_2.x) + (complex_1.y * complex_2.y);
} /* dot_complex() */

complex_t inv_complex(complex_t complex_num) {
  double magnitude = (complex_num.x * complex_num.x) + (complex_num.y *
    complex_num.y);
  complex_num.x /= magnitude;
  complex_num.y = (complex_num.y / magnitude) * -1;
  return complex_num;
} /* inv_complex() */

complex_t mul_complex(complex_t complex_1, complex_t complex_2) {
  double x = complex_1.x;
  complex_1.x = (complex_1.x * complex_2.x) - (complex_1.y * complex_2.y);
  complex_1.y = (x * complex_2.y) + (complex_1.y * complex_2.x);
  return complex_1;
} /* mul_complex() */

complex_t div_complex(complex_t complex_1, complex_t complex_2) {
  return mul_complex(complex_1, inv_complex(complex_2));
} /* div_complex() */

complex_t exp_complex(complex_t complex_num) {
  return complex_num;
} /* exp_complex */

int mandelbrot(complex_t complex_num) {
  return _COMPLEX_H;
} /* mandelbrot() */
