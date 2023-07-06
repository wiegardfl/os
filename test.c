#include <stdio.h>

int main()
{

    while (1)
    {
        puts("Input number:");
        int i;
        scanf("%d", &i);
        void* ptr = NULL + i;
        printf("ptr=%p\ti=%d\n", ptr, i);
        i = ptr - NULL;
        
        printf("%d\n", i);
    }
}