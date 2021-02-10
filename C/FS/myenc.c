#include <stdio.h>
#include <stdlib.h>

extern char **environ;

static void getEnv(char *key){
    puts(getenv(key));
}

static void setEnv(){

}

int main()
{
    
    for (int i = 0;environ[i] != NULL;i++){
        puts(environ[i]);
    }
    getEnv("ZSH");
    getchar();
    return 0;
}

