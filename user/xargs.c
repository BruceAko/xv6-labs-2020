#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int i;
    char *nargv[MAXARG];

    if (argc < 2)
    {
        fprintf(2, "Usage: %s command\n", argv[0]);
        exit(1);
    }

    for (i = 1; i < argc && i < MAXARG; i++)
    {
        nargv[i - 1] = argv[i];
    }

    char buf[1024];
    read(0, buf, sizeof(buf));
    char *p = buf;
    while (*p != '\0')
    {
        int num_i = i;
        char *start = p;
        while (*p != '\n' && *p != '\0')
        {
            p++;
        }
        *p = '\0';
        nargv[num_i - 1] = start;
        p++;
        if (fork() == 0)
        {
            exec(nargv[0], nargv);
        }
    }
    // 注意：要回收子进程！否则程序结果可能每次都不一样！(此处卡了半小时)
    while (wait((int *)0) != -1)
    {
    }
    exit(0);
}