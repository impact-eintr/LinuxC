#include "mytbf.h"
#include <stdio.h>
#include <stdlib.h>

#define CPS 10
#define BUFSIZE 1024
#define BURST 100

int main(int argc, char *argv[]) {
  int sfd, dfd = 1; //源文件 目标文件
  char buf[BUFSIZ];
  int len, ret, pos, size;
  mytbf_t *tbf;

  if (argc < 2) {
    fprintf(stderr, "Usage...\n");
    exit(1);
  }

  tbf = mytbf_init(CPS, BURST);

  do {
    sfd = open(argv[1], O_RDONLY);
    if (sfd < 0) {
      if (errno != EINTR) {
        perror("open()");
        exit(1);
      }
    }
  } while (sfd < 0);

  while (1) {

    size = mytbf_fetchtoken(tbf, BUFSIZE);
    if (size < 0) {
      fprintf(stderr, "fetcherr:%s\n", strerror(-size));
      exit(1);
    }

    while ((len = read(sfd, buf, size)) < 0) {

      if (errno == EINTR)
        continue;
      perror("read()");
      break;
    }
    if (len == 0)
      break;

    if (size - len > 0)
      mytbf_returntoken(tbf, size - len);

    pos = 0;
    while (len > 0) {
      ret = write(dfd, buf + pos, len);
      if (ret < 0) {
        if (errno == EINTR)
          continue;
        perror("write()");
        exit(1);
      }
      pos += ret;
      len -= ret;
    }
    sleep(1);
  }
  close(sfd);
  mytbf_destrory(tbf);

  exit(0);
}
