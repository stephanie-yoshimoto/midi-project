#include <hellomake.h>
#include <unistd.h>
#include <stdio.h>
#include "A.h"

int main() {
    int x;
/*
    sleep(30);
*/
    myPrintHelloMake();

    x = 30;
    printf("%d\n", x);
    foo();

    return(0);
}
