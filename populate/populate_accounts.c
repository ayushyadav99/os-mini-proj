/*
 * populate_accounts.c
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "datastructures.h"


int main(){
    int fd = open("../data/Accounts.dat", O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        printf("unable to create and open file\n");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET);

    Account acco1 = {
        .id = 1,
        .balance = 62500,
    };

    write(fd, &acco1, sizeof(acco1));

    Account acco2 = {
        .id = 2,
        .balance = 40500,
    };

    write(fd, &acco2, sizeof(acco2));
    
    Account acco3 = {
        .id = 3,
        .balance = 80001,
    };

    write(fd, &acco3, sizeof(acco3));
    
    Account acco4 = {
        .id = 4,
        .balance = 87961,
    };

    write(fd, &acco4, sizeof(acco4));
    close(fd);
}


