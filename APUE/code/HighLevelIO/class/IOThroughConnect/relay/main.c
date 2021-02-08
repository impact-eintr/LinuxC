#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 4096

enum {
    STATE_R = 1,
    STATE_W,
    STATE_AUTO, //分界线
    STATE_Ex,
    STATE_T
};

struct fsm_st {
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    char* errstr;
    int pos;
    int len;
};

static void fsm_driver(struct fsm_st* fsm)
{
    int ret = 0;
    switch (fsm->state) {

    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
        if (fsm->len == 0) {
            fsm->state = STATE_T;
        } else if (fsm->len < 0) {
            if (errno == EAGAIN)
                fsm->state = STATE_W;
            else {
                fsm->errstr = "read()";
                fsm->state = STATE_Ex;
            }
        } else {
            fsm->pos = 0;
            fsm->state = STATE_W;
        }
        break;

    case STATE_W:
        ret = write(fsm->dfd, fsm->buf + fsm->pos, fsm->len);
        if (ret < 0) {
            if (ret == EAGAIN)
                fsm->state = STATE_W;
            else
                fsm->state = STATE_Ex;
        } else {
            fsm->pos += ret; //文件读写位置移动
            fsm->len -= ret;
            if (fsm->len == 0) {
                fsm->state = STATE_R;
            } else {
                fsm->errstr = "write()";
                fsm->state = STATE_W;
            }
        }
        break;

    case STATE_Ex:
        perror(fsm->errstr);
        fsm->state = STATE_Ex;
        break;

    case STATE_T:
        printf("over\n");
        // alarm(1);
        break;

    default:
        break;
    }
}

static void relay(int fd1, int fd2)
{
    int fd1_save, fd2_save;
    struct fsm_st fsm12, fsm21;

    fd1_save = fcntl(fd1, F_GETFL);
    fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);

    fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    while (fsm12.state != STATE_T || fsm21.state != STATE_T) {

        fd_set rset, wset;
        //布置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);

        if (fsm12.state == STATE_R)
            FD_SET(fsm12.sfd, &rset);
        if (fsm12.state == STATE_W)
            FD_SET(fsm12.dfd, &wset);
        if (fsm21.state == STATE_R)
            FD_SET(fsm21.sfd, &rset);
        if (fsm21.state == STATE_W)
            FD_SET(fsm21.dfd, &wset);

        //监视
        if (fsm12.state < STATE_AUTO || fsm21.state < STATE_AUTO) {
            if (select(fd1 > fd2 ? fd1 + 1 : fd2 + 1, &rset, &wset, NULL, NULL) < 0) {
                if (errno == EINTR)
                    continue;
                perror("select()");
                exit(1);
            }
        }
        //查看监视结果
        if (FD_ISSET(fd1, &rset) || FD_ISSET(fd2, &wset) || fsm12.state == STATE_AUTO)
            fsm_driver(&fsm12);
        if (FD_ISSET(fd2, &rset) || FD_ISSET(fd1, &wset) || fsm21.state == STATE_AUTO)
            fsm_driver(&fsm21);
    }

    fcntl(fd1, F_SETFD, fd1_save);
    fcntl(fd2, F_SETFD, fd2_save);
}

int main(int argc, char** argv)
{
    int fd1, fd2;
    struct stat filestat;

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror("open()");
        exit(1);
    }

    stat(argv[1], &filestat);

    fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK);
    if (fd2 < 0) {
        perror("open()");
        exit(1);
    }
    chmod(argv[2], filestat.st_mode);

    relay(fd1, fd2);
    sleep(2);

    printf("回收资源\n");
    close(fd1);
    close(fd2);
    exit(0);
}
