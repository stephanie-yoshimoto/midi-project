#include "hw4.h"

#include <stdio.h>

#define TRUE (1)
#define QUIT (0)
#define NEG_COMPLEX (1)
#define INV_COMPLEX (2)
#define EXP_COMPLEX (3)
#define MANDELBROT (4)
#define ADD_COMPLEX (5)
#define SUB_COMPLEX (6)
#define MUL_COMPLEX (7)
#define DIV_COMPLEX (8)
#define DOT_COMPLEX (9)

/*
 *  This function is used to run the functions implemented in hw3.c.
 *  User input is used to determine which function to run and what
 *  input is passed to it. main() prints the return value of the
 *  selected function.
 */

int main() {
  while (TRUE) {
    printf("\n\n");
    printf("0) Quit\n"
           "1) neg_complex(complex_t)\n"
           "2) inv_complex(complex_t)\n"
           "3) exp_complex(complex_t)\n"
           "4) mandelbrot(complex_t)\n"
           "5) add_complex(complex_t, complex_t)\n"
           "6) sub_complex(complex_t, complex_t)\n"
           "7) mul_complex(complex_t, complex_t)\n"
           "8) div_complex(complex_t, complex_t)\n"
           "9) dot_complex(complex_t, complex_t)\n"
           "\nSelect a function: ");

    int num = 333333;
    scanf("%d", &num);

    if ((num > DOT_COMPLEX) || (num < QUIT)) {
      printf("Invalid selection.\n");
      continue;
    }

    if (num == QUIT) {
      return QUIT;
    }

    printf("Enter a complex_t number in the form x,y:       ");

    complex_t comp_one = {0.0, 0.0};
    complex_t comp_two = {0.0, 0.0};
    scanf("%lf,%lf", &(comp_one.x), &(comp_one.y));

    if (num > MANDELBROT) {
      printf("Enter another complex_t number in the form x,y: ");
      scanf("%lf,%lf", &(comp_two.x), &(comp_two.y));
    }

    complex_t result = {0.0, 0.0};
    double double_result = 0.0;
    int int_result = 0;
    switch (num) {
      case NEG_COMPLEX: {
        result = neg_complex(comp_one);
        break;
      }
      case INV_COMPLEX: {
        result = inv_complex(comp_one);
        break;
      }
      case EXP_COMPLEX: {
        result = exp_complex(comp_one);
        break;
      }
      case MANDELBROT: {
        int_result = mandelbrot(comp_one);
        break;
      }
      case ADD_COMPLEX: {
        result = add_complex(comp_one, comp_two);
        break;
      }
      case SUB_COMPLEX: {
        result = sub_complex(comp_one, comp_two);
        break;
      }
      case MUL_COMPLEX: {
        result = mul_complex(comp_one, comp_two);
        break;
      }
      case DIV_COMPLEX: {
        result = div_complex(comp_one, comp_two);
        break;
      }
      case DOT_COMPLEX: {
        double_result = dot_complex(comp_one, comp_two);
        break;
      }
    }

    if (num == MANDELBROT) {
      printf("The value returned was %d\n", int_result);
    }
    else if (num == DOT_COMPLEX) {
      printf("The value returned was %f\n", double_result);
    }
    else {
      printf("The value returned was (%f,%f)\n", result.x, result.y);
    }
  }

  /* not reached. */

  return QUIT;
} /* main() */
