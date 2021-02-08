#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
static char starter_pid_file_default[] = "./test.pid";

static bool check_pid(char *pid_file) {
    struct stat stb;
    FILE *pidfile;

    if (stat(pid_file, &stb) == 0) {
        pidfile = fopen(pid_file, "r+");
        if (pidfile) {
            char buf[64];
            pid_t pid = 0;
            memset(buf, 0, sizeof(buf));//填充0
            if (fread(buf, 1, sizeof(buf), pidfile)) {
                buf[sizeof(buf) - 1] = '\0';
                pid = atoi(buf);
            }
            fclose(pidfile);
            if (pid && kill(pid, 0) == 0) { /* such a process is running */
                return 1;
            }
        }
        printf("removing pidfile '%s', process not running", pid_file);
        unlink(pid_file);
    }
    return 0;
}

int main() {
    FILE *fd = fopen(starter_pid_file_default, "w+");

    if (fd) {
        fprintf(fd, "%u\n", getpid());
        fclose(fd);
    }
    sleep(5);
    if (check_pid(starter_pid_file_default)) {
        printf("test is already running (%s exists)\n", starter_pid_file_default);

    } else
      printf("test is NOT running (%s NOT exists)\n", starter_pid_file_default);

    unlink(starter_pid_file_default);

    return 0;
}
