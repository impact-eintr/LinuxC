// lhq yyds
#include <glob.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define BUFSIZE 1024
#define DELIMS " \t\n"

extern char **environ;
static int logout = 0;

static int cd(char *path) {
  int ret = chdir(path);
  if (ret == -1) {
    perror("chdir");
  }
  return ret;
}

static void readrc(char *name) {
  FILE *fp;
  fp = fopen(name, "r+");
  if (fp == NULL) {
    perror("file open error");
    return;
  }
  //处理文件内容
  fclose(fp);
}

static void prompt() {
  char pwd[BUFSIZE];
  char name[BUFSIZE];
  getcwd(pwd, BUFSIZE);
  getlogin_r(name, BUFSIZE);
  printf("%s %s $ ", name, pwd);
}

static int parse(char *linebuf, glob_t *globres) {
  char *tok;
  int flag = 0;

  while (1) {
    tok = strsep(&linebuf, DELIMS);
    if (tok == NULL) {
      break;
      // return -1;
    } else if (strcmp(tok, "cd") == 0) {
      char *path = strsep(&linebuf, DELIMS);
      return cd(path);
    } else if (tok[0] == '\0') {
      continue;
    }

    glob(tok, GLOB_NOCHECK | GLOB_APPEND * flag, NULL,
         globres); //第一次不能append glob_argv中是随机值 GLOB_NOCHECK |
                   //(GLOB_APPEND*flag)==0 第一次不append
    flag = 1;
  }
  return 1;
}

void handler(int sig) {
  write(1, "\n", 1);
  prompt();
  fflush(NULL);
}

static int getcommand(char **linebuf, size_t *linebuf_size, FILE *stream) {
  return (getline(linebuf, linebuf_size, stream));
}

void test(void) {
  if (logout)
    printf("\nGoodBye!\n");
}

int main() {
  printf("This is YSHELL\n");

  pid_t pid;
  char *linebuf = NULL;
  size_t linebuf_size = 0;
  glob_t globres; //解析命令行

  //读取配置文件
  char *yshrc = "/home/eintr/.yshrc"; //填一个绝对路径
  readrc(yshrc);

  atexit(test);

  while (1) {
    signal(SIGINT, handler);
    prompt();

    //获取命令
    if (getcommand(&linebuf, &linebuf_size, stdin) == -1) {
      logout = 1;
      exit(0);
    }
    //解析命令
    int ret = parse(linebuf, &globres);

    if (ret == -1) {

    } else if (ret == 0) { //内部命令

    } else if (ret == 1) { //外部命令
      fflush(NULL);
      pid = fork();
      if (pid < 0) {
        perror("fork()");
        exit(1);
      } else if (pid == 0) {
        execvp(globres.gl_pathv[0], globres.gl_pathv);
        perror("comman not found");
        exit(1);
      }
    }
    wait(NULL);
  }

  exit(0);
}
