#include <stdio.h>
#include <stdlib.h>

#include "llist.h"

int main()
{
    LLIST *headler;
    headler = llist_careate(sizeof(int));
    if (headler == NULL) {
        exit(1);
    }
    
    int data = 12;
    if (llist_insert(headler,&data,B) < 0){
        llist_destroy(headler);
        exit(1);
    }

    llist_destroy(headler);

    exit(0);
}

