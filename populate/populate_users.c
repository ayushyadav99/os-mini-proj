/*
 * populate_users.c
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
    int fd = open("../data/Users.dat", O_CREAT | O_RDWR, 0777);
    if (fd == -1){
        printf("unable to create and open file\n");
        exit(1);
    }
    lseek(fd, 0, SEEK_SET);

    User user1 = {
        .id = 1,
        .password = "abcd123",
        .account_type = singleAccount,
        .account_id = 1,
    };

    write(fd, &user1, sizeof(user1));

    User user2 = {
        .id = 2,
        .password = "abcd124",
        .account_type = singleAccount,
        .account_id = 2,
    };

    write(fd, &user2, sizeof(user2));
    
    User user3 = {
        .id = 3,
        .password = "abcd125",
        .account_type = singleAccount,
        .account_id = 3,
    };

    write(fd, &user3, sizeof(user3));
    
    User user4 = {
        .id = 4,
        .password = "abcd126",
        .account_type = singleAccount,
        .account_id = 4,
    };

    write(fd, &user4, sizeof(user4));
    close(fd);
    return 0;
}


