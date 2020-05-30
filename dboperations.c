/*
 * dboperations.c
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include<errno.h>
#include "dboperations.h"

extern int errno;

User getUser(int user_id){
    User validate; 
    int seekAmount = user_id -1; 
    int fd = open("./data/Users.dat", O_RDONLY,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_start = seekAmount*sizeof(User);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(User);
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return validate;
    }
    lseek(fd,seekAmount*sizeof(validate),SEEK_SET);
    read(fd, &validate, sizeof(validate));
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return validate;
}

int getAccountID(int user_id){
    User user = getUser(user_id);
    return user.account_id;
}


int dbDeposit(int deposit, int user_id){
    int account_id = getAccountID(user_id);
    Account update;
    int seekAmount = account_id -1; 
    int fd = open("./data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekAmount*sizeof(update);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(update);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekAmount*sizeof(update),SEEK_SET);
    read(fd, &update, sizeof(update));
    printf("initial balace : %d\n", update.balance);

    update.balance += deposit;
    lseek(fd, seekAmount*(sizeof(update)), SEEK_SET);
    write(fd, &update, sizeof(update));
    
    lseek(fd, seekAmount*(sizeof(update)), SEEK_SET);
    read(fd, &update, sizeof(update));
    printf("final balance : %d\n",update.balance);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return 1;
}


int dbWithdraw(int withdraw, int user_id){
    int account_id = getAccountID(user_id);
    Account update;
    int seekAmount = account_id -1; 
    int fd = open("./data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekAmount*sizeof(update);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(update);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekAmount*sizeof(update),SEEK_SET);
    read(fd, &update, sizeof(update));
    printf("initial balace : %d\n", update.balance);
    printf("withdraw amount %d\n", withdraw);
    if (update.balance < withdraw){
        printf("insuffient funds for withdrawal\n");
        return -1;
    }
    update.balance -= withdraw;
    lseek(fd, seekAmount*(sizeof(update)), SEEK_SET);
    write(fd, &update, sizeof(update));
    //getchar(); 
    lseek(fd, seekAmount*(sizeof(update)), SEEK_SET);
    read(fd, &update, sizeof(update));
    printf("final balance : %d\n",update.balance);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return 1;
}
int *dbaccount_details(int user_id){
    static int account_details[4] = {0,0,0,0}; 
    User user = getUser(user_id);
    int account_balance = dbBalance(user_id);
    printf("UserID: %d\n", user_id);
    printf("AccountNo: %d\n", user.account_id);
    printf("accountType: %d\n", user.account_type);
    printf("account balance: %d\n", account_balance);
    account_details[0] = user_id;
    account_details[1] = user.account_id;
    account_details[2] = user.account_type;
    account_details[3] = account_balance;
    return account_details;
}
int dbpassword_change(char *newpass, int user_id){
    User validate; 
    int seekAmount = user_id -1; 
    int fd = open("./data/Users.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = seekAmount*sizeof(validate);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(validate);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
        printf("Attempt to set write lock failed\n");
        return -1;
    }
    lseek(fd,seekAmount*sizeof(validate),SEEK_SET);
    read(fd, &validate, sizeof(validate));
    printf("old pass: %s\n", validate.password);
    
    strcpy(validate.password, newpass);
    lseek(fd,seekAmount*sizeof(validate),SEEK_SET);
    write(fd, &validate, sizeof(validate));
    
    
    lseek(fd,seekAmount*sizeof(validate),SEEK_SET);
    read(fd, &validate, sizeof(validate));
    printf("new pass: %s\n", validate.password);
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    close(fd);
    return 1;
}

int dbBalance(int user_id){
    int account_id = getAccountID(user_id);
    Account update;
    int seekAmount = account_id -1; 
    int fd = open("./data/Accounts.dat", O_RDWR);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_start = seekAmount*sizeof(update);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(update);
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    lseek(fd,seekAmount*sizeof(update),SEEK_SET);
    read(fd, &update, sizeof(update));
    printf("balance : %d\n", update.balance);
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    return update.balance;
}
int ValidateLogin(Credentials *loginatt) {
    int user_id = loginatt->id; 
    User validate = getUser(user_id); 
    int result;
    if (validate.id == loginatt->id){
        if (strcmp(validate.password, loginatt->password) == 0){
            result = 1;
        }
        else{
            result = -1;
        }
    }
    else{
        result = -1;
    }
    return result;
} 


int ValidateAdminLogin(AdminCredentials *loginatt) {
    //int user_id = loginatt->id; 
    printf("inside the validateAdminLogin function\n"); 
    AdminCredentials validate; 
    int seekAmount = 0; 
    int fd = open("./data/Admins.dat", O_RDONLY,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    //applying a readlock on the entire file to check usernames.
    lock.l_type = F_RDLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    int result = -1;
    lseek(fd,0,SEEK_SET); 
    printf("entered username : %s\n", loginatt->username);
    printf("entered password: %s\n", loginatt->password);
    while(read(fd, &validate, sizeof(validate))){
        if (strcmp(validate.username, loginatt->username) == 0){
            if (strcmp(validate.password, loginatt->password) == 0){
                return 1;
            }
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return result;
} 

int db_delete_account(int account_no){
    //first delete the account from Account table.
    Account account;
    Account delete = {
        .id = -1,
        .balance = -1
    };
    int fd = open("./data/Accounts.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    //applying a readlock on the entire file to check usernames.
    lock.l_type = F_WRLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    int result = -1;
    lseek(fd,0,SEEK_SET);
    while(read(fd, &account, sizeof(account))){
        if (account.id == account_no){
            lseek(fd,-sizeof(account), SEEK_CUR);
            write(fd, &delete, sizeof(delete));
            result = 0; 
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
    
    //deleting the account from the Users table.
    
    User user;
    fd = open("./data/Users.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock1;
    
    //applying a readlock on the entire file to check usernames.
    lock1.l_type = F_WRLCK;
    lock1.l_start =0; 
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    
    lseek(fd,0,SEEK_SET);
    while(read(fd, &user, sizeof(user))){
        if (user.account_id == account_no){
            user.account_id = -1;
            lseek(fd,-sizeof(user), SEEK_CUR);
            write(fd, &user, sizeof(user));
            result = 1; 
        }
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    return result;

}

int db_modify_account_type(int account_no, int account_type){
    User user;
    account_type = account_type -1;
    printf("in the db_modify_account_type function\n");
    int fd = open("./data/Users.dat", O_RDWR,0744);
    int result;
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock1;
    
    //applying a readlock on the entire file to check usernames.
    lock1.l_type = F_WRLCK;
    lock1.l_start =0; 
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    
    lseek(fd,0,SEEK_SET);
    while(read(fd, &user, sizeof(user))){
        if (user.account_id == account_no){
            user.account_type = account_type;
            lseek(fd,-sizeof(user), SEEK_CUR);
            write(fd, &user, sizeof(user));
            result = 1; 
            break;
        }
    }
    if (result != 1){
        result = -1;
    }
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    return result;
}

int db_modify_admin_password(char *newpass, char *username){
    AdminCredentials validate; 
    printf("inside db_modify_admin_password function\n");
    int fd = open("./data/Admins.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      exit(1);
    }
    static struct flock lock;
    
    //applying a readlock on the entire file to check usernames.
    lock.l_type = F_WRLCK;
    lock.l_start =0; 
    lock.l_whence = SEEK_SET;
    lock.l_len =0; 
    lock.l_pid = getpid();
    

    int ret = fcntl(fd, F_SETLK, &lock);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock failed %s\n",strerror(err));
        return -1;
    }
    lseek(fd,0,SEEK_SET); 
    while(read(fd, &validate, sizeof(validate))){
        if (strcmp(validate.username, username) == 0){
            strcpy(validate.password, newpass);
            lseek(fd,-sizeof(validate), SEEK_CUR);
            write(fd, &validate, sizeof(validate));
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            close(fd);
            return 1;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return -1;

}

int db_add_account(int account_no){
    //this function adds appends a new account to the accounts file.
    // This account should also be assigned to some user for any realistic funtionality
    // Hence a new user_id is also created and assigned this account.
    Account update = {
        .id = account_no,
        .balance = 0,
    };
    int fd = open("./data/Accounts.dat", O_RDWR | O_APPEND);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1; 
    }
    static struct flock lock;

    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();

    int ret = fcntl(fd, F_SETLKW, &lock);
    if (ret == -1) {
      printf("Attempt to set write lock failed\n");
      return -1;
    }
    
    write(fd, &update, sizeof(update));
    
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW, &lock);
    close(fd);
    printf("account created\n");
    //creating a new user and assigning the user the newly created account. 
    User newUser;
    fd = open("./data/Users.dat", O_RDWR,0744);
    if(fd == -1) {
      printf("Unable to open the file\n");
      return -1;
    }
    static struct flock lock1;

    lock1.l_type = F_WRLCK;
    lock1.l_start = 0;
    lock1.l_whence = SEEK_SET;
    lock1.l_len =0; 
    lock1.l_pid = getpid();
    

    ret = fcntl(fd, F_SETLK, &lock1);
    if (ret == -1) {
        int err = errno;
        fprintf(stderr, "Attempt to set write lock1 failed %s\n",strerror(err));
        return -1;
    }
    
    int max_user_id;
    while(read(fd, &newUser, sizeof(newUser))){
        if (newUser.id > max_user_id)
            max_user_id = newUser.id;
    }
    
    printf("max_user_id: %d\n", max_user_id); 
    newUser.id = max_user_id+1; //creating a new UserID;
    newUser.account_id =account_no; 
    newUser.account_type = singleAccount;
    char pass[50] = "dummypassword123";
    strcpy(newUser.password, pass);
    
    ret = write(fd, &newUser, sizeof(newUser)); 
    if (ret < 0){
        printf("unable to write new User\n");
        return -1;
    }
    printf("written user, but still in critical section\n"); 
    lock1.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock1);
    close(fd);
    printf("successfully created new user and assigned account\n");
    return 1;
}

