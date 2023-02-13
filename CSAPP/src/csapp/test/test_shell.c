#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>

#include "../headers/color.h"

#define MAX_ARG_COUNT (32)
#define MAX_LENGTH_USERINPUT (128)

typedef enum {
  EMPTY,
  // built-in command
  // "quit" to terminate shell
  // "jobs" to list all background jobs
  // "bg <pid>" to restart job in background by sending SIGCONT
  // "fg <pid>" to restart job in foreground by sending SIGCONT
  BUILTIN_QUIT,
  BUILTIN_JOBS,
  BUILTIN_FGCONT,
  BUILTIN_BGCONT,
  // foreground job taking the control flow
  // shell should wait for forground job to finish
  FOREGROUND_JOB,
  // background job leaving the control flow
  // background job are ending with '&', e.g.,
  // > /bin/sleep 5 &
  // shell should NOT wait for background job to finish
  BACKGROUND_JOB
} job_t;

typedef struct USERINPUT_STRUCT{
  // the jop type of current user input
  job_t type;
  // the count of arguments
  int argc;
  // the pointers to buffers
  char *(argv[MAX_ARG_COUNT]);
  // the parsed buffer
  char buffer[MAX_ARG_COUNT][MAX_LENGTH_USERINPUT];
  // the raw char pointer
  char *raw;
} userinput_t;

typedef enum {
  UNKNOWN,
  FG_RUNNING,
  BG_RUNNING,
  STOPPED,
} job_state_t;

static char *JOB_STATE_STR[4] = {"UK", "FG", "BG", "ST"};

typedef struct JOB_TASK_STRUCT {
  pid_t pid;
  job_state_t state;
  char argstr[MAX_LENGTH_USERINPUT];
} job_task_t;

#define MAX_NUM_JOBS (64)
job_task_t job_list[MAX_NUM_JOBS];

static int job_add(pid_t pid, job_state_t state, char *argstr);
static int job_delete(pid_t pid);
static void job_print();
static void job_init();
static int job_state(pid_t pid, job_state_t state);

static void parse_userinput(const char *input, userinput_t *result);
static void evaluate(userinput_t *input);

static int safe_printf(const char *fmt, ...);

// handler for SIGCHLD when a child process terminates
static void sigchld_handler(int sig);

// handler for SIGINT sent by Ctrl-C or SIGTSTP sent by Ctrl-Z
// when Ctrl-C, the signal should be sent to foreground job group
// i.e., child process may create child process
// when Ctrl-Z, the signal should be sent to foreground job group
// i.e., child process may create child process
// and shell process & other background processes
// should not be interrupted
static void sigproxy_handler(int sig);

typedef void (sighandler_t)(int);
sighandler_t *register_sighandler(int sig, sighandler_t *handler, int falgs) {
  struct sigaction action, old_action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = falgs;

  if (sigaction(sig, &action, &old_action) < 0) {
    write(1, "Signal error\n", 13);
    exit(1);
  }

  return old_action.sa_handler;
}

static volatile sig_atomic_t fg_reaped = 1;
static volatile sig_atomic_t fg_pid = -1;

int main() {
  userinput_t input;
  char input_chars[MAX_LENGTH_USERINPUT];

  // register handlers for signals
  // with this, this program can only run on *unix
  register_sighandler(SIGCHLD, sigchld_handler, SA_RESTART);
  register_sighandler(SIGINT, sigproxy_handler, SA_RESTART);
  register_sighandler(SIGTSTP, sigproxy_handler, SA_RESTART);

  job_init();
  pid_t shell_pid = getpid();

  while(1) {
    printf(GREENSTR("[%d] > "), shell_pid);

    // read user input
    fgets(input_chars, MAX_LENGTH_USERINPUT, stdin);
    parse_userinput(input_chars, &input);

    if (input.type == EMPTY) {
      continue;
    } else {
      evaluate(&input);
    }
  }
  return 0;
}

static void child_process(userinput_t *input) {
  char *envp[] = {NULL};

  sigset_t mask, prev;
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD | SIGINT | SIGTSTP);
  // block
  sigprocmask(SIG_BLOCK, &mask, &prev);

  pid_t pid = fork();
  if (pid == 0) {
    // new created child process will inherit the signal
    // pending vector, blocking vector from parent.
    // unblock and reset in case further execve will fork
    sigprocmask(SIG_SETMASK, &prev, NULL);

    // do not trigger copy on write here
    if (execve(input->argv[0], input->argv, envp) < 0) {
      printf(REDSTR("failed to execute %s\n"), input->argv[0]);
      exit(-1);
    }
  } else {
    if (input->type == FOREGROUND_JOB) {
      job_add(pid, FG_RUNNING, input->raw);
      fg_reaped = 0;
      fg_pid = pid;

      // unblock
      sigprocmask(SIG_SETMASK, &prev, NULL);

      while (fg_reaped == 0) {
        printf("main process running\n");
        sleep(1);
      }
    } else if (input->type == BACKGROUND_JOB) {
      job_add(pid, BG_RUNNING, input->raw);
      assert(fg_reaped == 1);
      // unblock
      sigprocmask(SIG_SETMASK, &prev, NULL);
    }
  }
}

static void evaluate(userinput_t *input) {
  switch (input->type) {
  case BUILTIN_QUIT:
    printf(YELLOWSTR("Shell terminating ...\n"));
    exit(0);
  case BUILTIN_JOBS:
    job_print();
    break;
  case BUILTIN_FGCONT:
    break;
  case BUILTIN_BGCONT:
    break;
  case FOREGROUND_JOB: // 注意这里会fallthrough
  case BACKGROUND_JOB:
    child_process(input);
    break;
  default:
    break;
  }
}

// SIGCHLD Handler
static void sigchld_handler(int sig) {
  int _errno = errno;
  int status;

  // waitpid(-1) -- match any process
  // other child processes may terminate
  // but at most one SIGCHLD would be pending
  //
  // waitpid - wait for state change of a child:
  //  1.  child terminated
  //  2.  child stopped by a signal (SIGTSTP)
  //  3.  child resumed by a signal (CONTINUE)
  // Wait-No-Hang flag to avoid wait and return immediately
  pid_t pid = waitpid(-1, &status, WUNTRACED | WNOHANG); // 等所有的子进程
  while (pid  > 0) {
    if (pid == fg_pid) {
      fg_reaped = 1;
    }

    if (WIFEXITED(status)) {
      // child was exited normally: exit() or return from main()
      safe_printf(BLUESTR("[%u] exits normally\n"));
      job_delete(pid);
    }

    if (WIFSIGNALED(status)) {
      // child was terminated by signal: SIGINT
      safe_printf(BLUESTR("[%u] is terminated by %u\n"), pid, WTERMSIG(status));
      job_delete(pid);
    }

    if (WIFSTOPPED(status)) {
      // child was stopped by signal
      // must unset action SA_NOCLDSTOP
      safe_printf(BLUESTR("[%u] is stopped by %u\n"), pid, WSTOPSIG(status));
      job_state(pid, STOPPED);
    }

    if (WIFCONTINUED(status)) {
      // child was resumed by delivery of SIGCONT
      // must unset action SA_NOCLDSTOP
      safe_printf(BLUESTR("[%u] is resumed by SIGCONT\n"), pid);
    }

    // Delivery of SIGCHLD is blocked during handler execution
    // unless set SA_NODEFER in sigaction()
    // If one SIGCHLD is generated again, it's marked as pending
    // and would be delivered **WHEN HANDLER RETURNS**
    pid = waitpid(-1, &status, WUNTRACED | WNOHANG);
  }

  // restore errno
  errno = _errno;
}

// SIGINT/SIGTSTP should be invoked only after fg_pid is valid
static void sigproxy_handler(int sig) {
  int _errno = errno;
  if (fg_pid > 0) {
    killpg(fg_pid, sig);
  }
  errno = _errno;
}

static void job_init() {
  for (int i = 0;i < MAX_NUM_JOBS; ++i) {
    job_list[i].state = UNKNOWN;
    job_list[i].pid = -1;
    memset(job_list[i].argstr, '\0', sizeof(job_list[i].argstr));
  }
}

static int job_add(pid_t pid, job_state_t state, char *argstr) {
  if (pid == -1) {
    return -1;
  }
  for (int i = 0;i < MAX_NUM_JOBS;++i) {
    if (job_list[i].state == UNKNOWN) {
      job_list[i].pid = pid;
      job_list[i].state = state;
      strcpy(job_list[i].argstr, argstr);
      return 1;
    }
  }
  printf(REDSTR("job add::no free slot for a new job\n"));
  return 0;
}

static int job_delete(pid_t pid) {
   if (pid == -1) {
    return -1;
  }
  for (int i = 0;i < MAX_NUM_JOBS;++i) {
    if (job_list[i].state == UNKNOWN) {
      job_list[i].pid = pid;
      job_list[i].state = UNKNOWN;
      memset(job_list[i].argstr, '\0', sizeof(job_list[i].argstr));
      return 1;
    }
  }
  printf(REDSTR("job delete::pid {%d} not found from job list\n"), pid);
  return 0;
}

static int job_state(pid_t pid, job_state_t state) {
  if (pid == -1) {
    return -1;
  }

  for (int i = 0; i < MAX_NUM_JOBS; ++i) {
    if (job_list[i].pid == pid) {
      job_list[i].pid = 0;
      job_list[i].state = state;
      return 1;
    }
  }

  printf(REDSTR("job state::pid {%d} not found from job list\n"), pid);
  return 0;
}

static void job_print() {
  for (int i = 0; i < MAX_NUM_JOBS; ++i) {
    if (job_list[i].state == UNKNOWN) {
      continue;
    }
    printf("[%d]\t%s\t%s\n", job_list[i].pid, JOB_STATE_STR[job_list[i].state],
           job_list[i].argstr);
  }
}

static void parse_userinput(const char *input, userinput_t *result) {
  if (result == NULL) {
    printf(REDSTR("Failed to fetch space for user input parsing result\n"));
    exit(-1);
  }

  // empty as default
  result->type = EMPTY;
  result->argc = 0;
  result->raw = NULL;
  memset(result->argv, 0, sizeof(result->argv));
  memset(result->buffer, 0, sizeof(result->buffer));
  if (input == NULL || *input == '\0') {
    return;
  }
  result->raw = (char *)input;

  // the previous char
  int p = -1;
  // first we count the size of argv
  for (int i = 0; i < MAX_LENGTH_USERINPUT; ++i) {
    if (input[i] == '\0' || input[i] == '\n')
      break;

    if (input[i] != ' ' && (p == -1 || input[p] == ' ')) {
      result->argc += 1;
    }
    p = i;
  }

  // the current parsed argument char
  int j = 0, w = 0;
  p = -1;
  for (int i = 0; i < MAX_LENGTH_USERINPUT; ++i) {
    if ((input[i] == ' ' || input[i] == '\0' || input[i] == '\n') &&
        (p != -1 || input[p] != ' ')) {
      strncpy(result->buffer[j], &input[w], i - w);
      result->buffer[j][i - w] = '\0';
      result->argv[j] = result->buffer[j];

      // reset to next word
      j += 1;
    } else if (input[i] != ' ' && (p == -1 || input[p] == ' ')) {
      w = i;
    }

    // go to next input char
    p = i;

    if (input[i] == '\0' || input[i] == '\n') {
      break;
    }
  }

  if (result->argc >= 2 && strcmp(result->buffer[result->argc - 1], "&") == 0) {
    result->type = BACKGROUND_JOB;
    result->argv[result->argc - 1] = NULL;
    result->argc -= 1;
  } else if (result->argc >= 1) {
    result->type = FOREGROUND_JOB;
  }

  if (result->argc == 1) {
    if (strcmp(result->buffer[0], "quit") == 0) {
      result->type = BUILTIN_QUIT;
    } else if (strcmp(result->buffer[0], "jobs") == 0) {
      result->type = BUILTIN_JOBS;
    }
  } else if (result->argc == 2) {
    if (strcmp(result->buffer[0], "fg") == 0) {
      result->type = BUILTIN_FGCONT;
    } else if (strcmp(result->buffer[0], "bg") == 0) {
      result->type = BUILTIN_BGCONT;
    }
  }
}

// async-signal-safe printf: support %s and %u only
static int convert_unsigned(unsigned u, char *buf) {
  int len = 0, is_leading_zero = 1;
  unsigned b = 1000000000, t = 0;
  while (b > 0) {
    t = u / b;
    u = u - t * b;
    if (t != 0 || b == 10) {
      is_leading_zero = 0;
    }
    if (t != 0 || is_leading_zero == 0) {
      buf[len] = t + '0';
      len += 1;
    }
    b = b / 10;
  }
  return len;
}

static int safe_printf(const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);

  // 2 pointers:
  // i for scanned in format string
  // j for written in stdout
  // write for: (1) string in fmt; (2) escape in fmt; in each loop
  int i = 0, j = 0;
  int start = 0, len = 0;
  while (fmt[i] != '\0') {
    // Handle format characters
    if (fmt[i] == '%') {
      switch (fmt[i + 1]) {
      // string format
      case 's':
        // write previous string in fmt
        write(STDOUT_FILENO, &fmt[start], i - start);
        // write string in arg
        char *str = va_arg(argp, char *);
        len = strlen(str);
        write(STDOUT_FILENO, str, len);
        // reset pointer
        i += 2;
        start = i;
        j += len;
        break;

      // unsigned format
      case 'u':
        // write previous string in fmt
        write(STDOUT_FILENO, &fmt[start], i - start);
        // write unsigned in arg
        char buf[11];
        unsigned u = va_arg(argp, unsigned);
        len = convert_unsigned(u, buf);
        write(STDOUT_FILENO, buf, len);
        // reset pointer
        i += 2;
        start = i;
        j += len;
        break;

      default:
        // like continue
        i += 1;
        j += 1;
        break;
      }
    } else {
      i += 1;
      j += 1;
    }
  }
  // print the tailing strings
  if (i - start > 0) {
    write(STDOUT_FILENO, &fmt[start], i - start);
  }

  va_end(argp);
  return j;
}
