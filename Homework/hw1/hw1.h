#ifndef HW1_H
#define HW1_H

#define ARRAY_SIZE 40
extern int g_hailstone_array[ARRAY_SIZE];

#define HAILSTONE_ERROR (-1)
#define HAILSTONE_CORRECT (0)

int  compute_hailstone(int, int);
int  check_hailstone(int);

#endif // HW1_H
