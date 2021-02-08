#include <crypt.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
	fprintf(stderr, "Usage...\n");
	exit(1);
    }
    char* input_pass;
    struct spwd* shadowline;
    char* cryptd_pass;

    input_pass = getpass("PassWord: ");
    shadowline = getspnam(argv[1]);
    cryptd_pass = crypt(input_pass, shadowline->sp_pwdp);

    if (strcmp(shadowline->sp_pwdp, cryptd_pass) == 0)
	puts("ok\n");
    else
	puts("failed\n");

    exit(0);
}
