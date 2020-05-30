/*
 * client.c
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<arpa/inet.h>
#include "client.h"

#define PORT 4444
extern int errno;

void ExitHandler(long cookie, int socket_d){
    int option = 6;
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    close(socket_d);
    printf("Disconnected from server\n");
    exit(0);
}

void AccountHandler(long cookie, int socket_d){
    int option = 5;
    int account_details[4] = {0,0,0,0}; 
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option)); 
    
    read(socket_d, account_details, sizeof(account_details)); 
    printf("UserID: %d\n", account_details[0]);
    printf("AccountNo: %d\n", account_details[1]);
    printf("accountType: %d\n", account_details[2]);
    printf("account balance: %d\n", account_details[3]);
}

void PasswordHandler(long cookie, int socket_d){
    int option = 4;
    char pass[50];
    int servResp;
    printf("input your new password: ");
    scanf("%s", pass);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, pass, sizeof(pass));


    read(socket_d, &servResp, sizeof(servResp));
    if (servResp > 0){
        printf("Password Changed successfully\n");
    }
    else{
        printf("Unable to change password\n");
    }
}

void BalanceHandler(long cookie, int socket_d){
    int servResp;
    int option = 3;
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp > 0){
        printf("current balance: %d\n", servResp);
    }
    else{
        printf("unable to display balance\n");
    }
}
void DepositHandler(long cookie, int socket_d){
    int deposit;
    int servResp;
    int option = 1;
    printf("Input amount you would like to deposit: ");
    scanf("%d", &deposit);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &deposit, sizeof(deposit));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp == 1){
        printf("successfully deposited money\n");
    }
    else{
        printf("couldn't deposit money\n");
    }
}

void WithdrawHandler(long cookie, int socket_d){
    int withdraw;
    int servResp;
    int option = 2;
    printf("Input the amount you would like to withdraw: ");
    scanf("%d", &withdraw);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &withdraw, sizeof(withdraw));

    read(socket_d, &servResp, sizeof(servResp));
    if (servResp == 1){
        printf("successfully withdrawn money\n");
    }
    else{
        printf("couldn't withdrawn money\n");
    }
}

int optionToggle(long cookie, int socket_d){
    write(1, "1. Deposit\n", sizeof("1. Deposit\n")); 
    write(1, "2. Withdraw\n", sizeof("1. Withdraw\n")); 
    write(1, "3. Balance Enquiry\n", sizeof("3. Balance Enquiry\n")); 
    write(1, "4. Password Change\n", sizeof("4. Password Change\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int response;
    scanf("%d", &response);
    switch(response){
        case 1:
            DepositHandler(cookie, socket_d);break;
        case 2:
            WithdrawHandler(cookie, socket_d);break;
        case 3:
            BalanceHandler(cookie, socket_d);break;
        case 4:
            PasswordHandler(cookie, socket_d);break;
        case 5:
            AccountHandler(cookie, socket_d);break;
        case 6:
            ExitHandler(cookie, socket_d);break;
    }
    optionToggle(cookie, socket_d);
}

int adminoptionToggle(long cookie, int socket_d, char *username){
    write(1, "1. Add Account\n", sizeof("1. Add Account\n")); 
    write(1, "2. Delete Account\n", sizeof("1. Delete Account\n")); 
    write(1, "3. Modify Account Type\n", sizeof("3. Modify Account Type\n")); 
    write(1, "4. Password Change\n", sizeof("4. Password Change\n")); 
    write(1, "5. Account Details\n", sizeof("5. Account Details\n")); 
    write(1, "6. Exit\n", sizeof("6. Exit\n")); 
    int response;
    scanf("%d", &response);
    printf("recorded response: %d\n", response);
    switch(response){
        case 1:
            AddAccountHandler(cookie, socket_d);break;
        case 2:
            DeleteAccountHandler(cookie, socket_d);break;
        case 3:
            ModifyAccountTypeHandler(cookie, socket_d);break;
        case 4:
            ModifyAdminPasswordHandler(cookie, socket_d, username);break;
        case 5:
            viewAccountDetailsHandler(cookie, socket_d);break;
        case 6:
            ExitHandler(cookie, socket_d);break;
    }
    adminoptionToggle(cookie, socket_d, username);
}

void viewAccountDetailsHandler(long cookie, int socket_d){
    int user_id;
    int option = 5;
    printf("Input user ID to check: ");
    scanf("%d", &user_id);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &user_id, sizeof(user_id));
    printf("written everything\n"); 
    int account_details[4] = {0,0,0,0}; 
    
    read(socket_d, account_details, sizeof(account_details)); 
    printf("UserID: %d\n", account_details[0]);
    printf("AccountNo: %d\n", account_details[1]);
    printf("accountType: %d\n", account_details[2]);
    printf("account balance: %d\n", account_details[3]);

}


void ModifyAdminPasswordHandler(long cookie, int socket_d, char *username){
    char newpass[50];
    int option = 4;
    printf("Input your new admin account password: ");
    scanf("%s", newpass);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, newpass, sizeof(newpass));
    write(socket_d, username, sizeof(username));
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("admin password changed successfully\n");
    }
    else{
        printf("password change failed \n");
    }
}
void ModifyAccountTypeHandler(long cookie, int socket_d){
    int account_no;
    int account_type;
    int option = 3;
    printf("Input the account_no you want to modify: ");
    scanf("%d", &account_no);
    printf("New Account type:\n1. Single Account\n2. Joint Account\n");
    scanf("%d", &account_type);
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &account_no, sizeof(account_no));
    write(socket_d, &account_type, sizeof(account_type));
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no:%d's type successfully set\n", account_no);
    }
    else{
        printf("failed to change account type\n");
    }

}
void DeleteAccountHandler(long cookie, int socket_d){
    int account_no;
    int option = 2;
    printf("Input the account_no you want to delete: ");
    scanf("%d", &account_no);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &account_no, sizeof(account_no));

    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no: %d successfully delelted\n", account_no);
    }
    else{
        printf("delete account failed\n");
    }
}

void AddAccountHandler(long cookie, int socket_d){
    int account_no;
    int option = 1;
    printf("Input the account_no you want to add: ");
    scanf("%d", &account_no);
    
    write(socket_d, &cookie, sizeof(cookie));
    write(socket_d, &option, sizeof(option));
    write(socket_d, &account_no, sizeof(account_no));
   
    int ret;
    read(socket_d, &ret, sizeof(ret));
    if (ret == 1){
        printf("account no: %d successfully added\n", account_no);
    }
    else{
        printf("add account failed\n");
    }
}

int loginToggle(int socket_d){
    write(1, "0. RegularLogin\n", sizeof("1. RegularLogin\n")); 
    write(1, "1. AdminLogin\n", sizeof("1. AdminLogin\n")); 
    int response;
    scanf("%d", &response);
    switch(response){
        case 0:
            normallogin(socket_d);break;
        case 1:
            adminlogin(socket_d);break;
    }

}
int normallogin(int socket_d){
    enum LoginRequest type = NormalLoginRequest;
    int temp = write(socket_d, &type, sizeof(type));
    
    Credentials *request = malloc(sizeof(Credentials));
    int userID;
    char password[50];
    printf("--------- Login ---------\n");
    printf("userID: ");
    scanf("%d", &request->id);
    printf("password: ");
    scanf("%s", password);
    strcpy(request->password, password);
    int ret = write(socket_d, request, sizeof(Credentials));
    if (ret < 0){
        int err = errno;	
        fprintf(stderr, "unable to write to server: %s\n",strerror(err));
        return -1;
    }
    long cookie;
    read(socket_d, &cookie, sizeof(cookie));
    if (cookie == -1)
        loginToggle(socket_d);
    else
        optionToggle(cookie, socket_d); 
        return 1;
}

int adminlogin(int socket_d){
    enum LoginRequest type = AdminLoginRequest;
    int temp = write(socket_d, &type, sizeof(type));
    AdminCredentials *request = malloc(sizeof(AdminCredentials));
    char username[50];
    char password[50];
    printf("--------- Admin Login ---------\n");
    printf("username: ");
    scanf("%s", username);
    printf("password: ");
    scanf("%s", password);
    strcpy(request->password, password);
    strcpy(request->username, username);
    int ret = write(socket_d, request, sizeof(AdminCredentials));
    if (ret < 0){
        int err = errno;	
        fprintf(stderr, "unable to write to server: %s\n",strerror(err));
        return -1;
    }
    long cookie;
    read(socket_d, &cookie, sizeof(cookie));
    if (cookie == -1)
        loginToggle(socket_d);
    else
        adminoptionToggle(cookie, socket_d, username); 
        return 1;
}

int main(){
    struct sockaddr_in server;
    int socket_d;
    int connection;
    char buff[5];
    socket_d = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    connection = connect(socket_d, (struct sockaddr *)&server, sizeof(server));
    if (connection< 0){
        int err = errno;	
        fprintf(stderr, "unable to connect to server: %s\n",strerror(err));
        return -1;
    }
    long cookie = loginToggle(socket_d); 
    printf("returned to main\n");
    return 0;
}
