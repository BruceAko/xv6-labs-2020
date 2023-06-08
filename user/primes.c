#include "kernel/types.h"
#include "user/user.h"

void primes(int *pip)
{
    // 注意：及时关闭文件描述符，释放资源
    close(pip[1]);
    int p;
    if (read(pip[0], &p, 4) == 0)
    {
        exit(0);
    }
    printf("prime %d\n", p);
    int pip2[2];
    pipe(pip2);
    if (fork() == 0)
    {
        primes(pip2);
    }
    else
    {
        while (1)
        {
            int n;
            if (read(pip[0], &n, 4) != 0)
            {
                if (n % p != 0)
                {
                    write(pip2[1], &n, 4);
                }
            }
            else
            {
                close(pip[0]);
                close(pip2[1]);
                wait((int *)0);
                exit(0);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int pip[2];
    pipe(pip);
    // 子进程
    if (fork() == 0)
    {
        primes(pip);
    }
    // 父进程
    else
    {
        close(pip[0]);
        for (int num = 2; num <= 35; num++)
        {
            write(pip[1], &num, 4);
        }
        close(pip[1]);
        // 等待子进程结束，此处只有一个子进程（孙子进程结束不触发返回）
        wait((int *)0);
    }
    exit(0);
}