#include <stdio.h>
#include <unistd.h>

#define MYSYSCALL(x) syscall(451, x)

int main(void)
{
    int x = MYSYSCALL(11);
    printf("%d\n", x);
}
