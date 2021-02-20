#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>

int main(int argc,char **argv)
{
    char *input_passwd;//来自命令行的密码原文
    char *crypted_passwd;
    struct spwd *shadowline;
    
    if (argc < 2) {
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    input_passwd = getpass("PassWoed:");

    shadowline = getspnam(argv[1]);

    crypted_passwd = crypt(input_passwd,shadowline->sp_pwdp);
    
    if (strcmp(shadowline->sp_pwdp,crypted_passwd) == 0)
      puts("ok!");
    else
      puts("failed!");

    return 0;
}

