#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
int main(void)
{
    pid_t childpid;
    int status;
    int retval;

    childpid = fork();
    if (-1 == childpid)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    else if (0 == childpid)
    {
        puts("In child process");
        sleep(1000);
        exit(EXIT_SUCCESS);
    }
    else
    {
        if (0 == (waitpid(childpid, &status, WNOHANG)))
        {
            retval = kill(childpid, SIGKILL);

            if (retval)
            {
                puts("kill failed.");
                perror("kill");
                waitpid(childpid, &status, 0);
            }
            else
            {
                printf("%d killed\n", childpid);
            }

        }
    }

    exit(EXIT_SUCCESS);
}
