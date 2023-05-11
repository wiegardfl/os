#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

u_int32_t f(u_int32_t n) {
    if (n <= 1) return 1;
    else return f(n-1) + f(n-2);
}

int main (int argc, char** argv){
    if (argc > 1) {
        u_int32_t n = atoi(argv[1]);
    printf("%u\n", f(n));
    }
}