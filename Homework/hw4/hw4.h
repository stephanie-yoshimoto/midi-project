#ifndef HW4_H
#define HW4_H

struct complex_struct {
  double x;
  double y;
};

typedef struct complex_struct complex_t;

complex_t add_complex(complex_t, complex_t);
complex_t sub_complex(complex_t, complex_t);
complex_t mul_complex(complex_t, complex_t);
complex_t div_complex(complex_t, complex_t);

complex_t neg_complex(complex_t);
complex_t inv_complex(complex_t);
complex_t exp_complex(complex_t);

double  dot_complex(complex_t, complex_t);

int mandelbrot(complex_t);

#define MAX_MANDELBROT 255

#define _COMPLEX_H  1

#endif // HW3_H
