#include <hellomake.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    // call a function in another file
    sleep(30);
    myPrintHelloMake();

    int x = 30;
    printf("%d\n", x);

    return(0);
}