#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p_p2c[2];
    int p_c2p[2];
    pipe(p_p2c);
    pipe(p_c2p);
    char *c = (char *)malloc(sizeof(char));
    char passChar = 'a';
    if (fork() == 0)
    {
        close(p_p2c[1]);
        close(p_c2p[0]);
        int pid_c = getpid();
        read(p_p2c[0], c, 1);
        printf("%d: received ping\n", pid_c);
        close(p_p2c[0]);
        write(p_c2p[1], c, 1);
        close(p_c2p[1]);
        exit(0);
    }
    else
    {
        close(p_p2c[0]);
        close(p_c2p[1]);
        int pid_p = getpid();
        write(p_p2c[1], &passChar, 1);
        close(p_p2c[1]);
        read(p_c2p[0], c, 1);
        printf("%d: received pong\n", pid_p);
        close(p_c2p[0]);
        free(c);
        exit(0);
    }
}
