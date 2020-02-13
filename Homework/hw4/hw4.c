/* Stephanie Yoshimoto, hw4.c, CS 24000, Fall 2020
 * Last updated February 5, 2020
 */

#include "hw4.h"

#include <math.h>

/*
 * Adds two complex numbers' parts and returns the sum.
 */

complex_t add_complex(complex_t complex_1, complex_t complex_2) {
  complex_1.x += complex_2.x;
  complex_1.y += complex_2.y;
  return complex_1;
} /* add_complex() */

/*
 * Negates the real and imaginary part of the complex number.
 */

complex_t neg_complex(complex_t complex_num) {
  complex_num.x *= -1;
  complex_num.y *= -1;
  return complex_num;
} /* neg_complex() */

/*
 * Subtracts the second complex number from the first.
 */

complex_t sub_complex(complex_t complex_1, complex_t complex_2) {
  return add_complex(complex_1, neg_complex(complex_2));
} /* sub_complex() */

/*
 * Calculates and returns the dot product of the complex arguments.
 */

double dot_complex(complex_t complex_1, complex_t complex_2) {
  return (complex_1.x * complex_2.x) + (complex_1.y * complex_2.y);
} /* dot_complex() */

/*
 * Calculates and returns the reciprocal of the complex number.
 */

complex_t inv_complex(complex_t complex_num) {
  double magnitude = (complex_num.x * complex_num.x) + (complex_num.y *
    complex_num.y);
  complex_num.x /= magnitude;
  complex_num.y = (complex_num.y / magnitude) * -1;
  return complex_num;
} /* inv_complex() */

/*
 * Calculates and returns complex product of complex arguments.
 */

complex_t mul_complex(complex_t complex_1, complex_t complex_2) {
  double x = complex_1.x;
  complex_1.x = (complex_1.x * complex_2.x) - (complex_1.y * complex_2.y);
  complex_1.y = (x * complex_2.y) + (complex_1.y * complex_2.x);
  return complex_1;
} /* mul_complex() */

/*
 * Calculates and returns divison of first complex number by thhe second.
 */

complex_t div_complex(complex_t complex_1, complex_t complex_2) {
  return mul_complex(complex_1, inv_complex(complex_2));
} /* div_complex() */

/*
 * Calculates and returns complex exponential function for a complex number
 * argument using the math library functions.
 */

complex_t exp_complex(complex_t complex_num) {
  double x = complex_num.x;
  complex_num.x = exp(x) * cos(complex_num.y);
  complex_num.y = exp(x) * sin(complex_num.y);
  return complex_num;
} /* exp_complex() */

/*
 * Generates a Mandelbrot set for a complex number, returns iterations
 * necessary until magnitude is larger than 2.0.
 */

int mandelbrot(complex_t complex_num) {
  int calculations = 0;
  double dot_product = 0.0;
  complex_t z;
  z.x = 0.0;
  z.y = 0.0;
  while (dot_product < 4.0) {
    z = add_complex(mul_complex(z, z), complex_num);
    dot_product = dot_complex(z, z);
    calculations++;
    if (calculations == MAX_MANDELBROT) {
      return MAX_MANDELBROT;
    }
  }
  return calculations;
} /* mandelbrot() */
