#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *args[2];
    args[0] = "/bin/sh";
    args[1] = NULL;
    printf(" --> %d\n",seteuid(0));
    execve(args[0], args, NULL);
    return 0; 
}
