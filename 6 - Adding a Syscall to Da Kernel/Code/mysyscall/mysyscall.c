#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(mysyscall, int, x)

{
    return 10 * x;
}
