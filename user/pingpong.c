#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // 注意：需要两个管道
    int p2c[2];
    int c2p[2];
    pipe(p2c);
    pipe(c2p);
    char word = 'p';
    char buf;

    // 子进程
    if (fork() == 0)
    {
        close(p2c[1]);
        close(c2p[0]);
        read(p2c[0], &buf, 1);
        // 注意：用完文件描述符就关闭是为了避免如下情况，如果还有一条read语句，且所有持有该管道写端的文件描述符未全部关闭，则这条read会永远阻塞下去
        //  read(p2c[0], &buf, 1);
        close(p2c[0]);
        printf("%d: received ping\n", getpid());
        write(c2p[1], &buf, 1);
        close(c2p[1]);
        exit(0);
    }
    // 父进程
    else
    {
        close(p2c[0]);
        close(c2p[1]);
        write(p2c[1], &word, 1);
        close(p2c[1]);
        read(c2p[0], &buf, 1);
        close(c2p[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}