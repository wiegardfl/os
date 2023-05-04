#include <stdio.h>

int f(int n) {
    if (n <= 1) return 1;
    else return f(n-1) + f(n-2);
}

int main (int argc, char** argv){
    if (argc > 1) {
        int n = (int) argv[1];
    return f(n);
    }

}