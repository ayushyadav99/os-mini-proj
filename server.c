/*
 * server.c
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include "server.h"
#include "datastructures.h"
#include "dboperations.h"

#define PORT 4444
extern int errno;

int mainController(int client,int userID, long cookie){
    printf("in the mainController\n");
    enum RequestType type;
    long cookie_recv;
    
    read(client, &cookie_recv, sizeof(cookie_recv));
    read(client, &type, sizeof(type));

    if (cookie_recv == cookie){ 
        printf("cookie matched\n");
        printf("type recv: %d\n", type);
        switch(type){
            case DepositRequest:
                deposit(client, userID);break;
            case WithdrawalRequest:
                withdraw(client, userID);break;
            case BalanceRequest:
                balance(client, userID);break;
            case PasswordChangeRequest:
                password_change(client, userID);break;
            case viewDetailsRequest:
                view_details(client, userID);break;
            case ExitRequest:
                client_exit(client, userID);break;
        } 
    }
    mainController(client, userID, cookie);

}

int AdminController(int client, long cookie){
    printf("in the AdminController\n");
    enum AdminRequestType type;
    long cookie_recv;
    
    read(client, &cookie_recv, sizeof(cookie_recv));
    read(client, &type, sizeof(type));

    if (cookie_recv == cookie){ 
        printf("cookie matched\n");
        printf("type recv: %d\n", type);
        switch(type){
            case AddAccountRequest:
                add_account(client);break;
            case DeleteAccountRequest:
                delete_account(client);break;
            case ModifyAccountTypeRequest:
                modify_account_type(client);break;
            case ModifyPasswordRequest:
                modify_admin_password(client);break;
            case AccountDetailsRequest:
                viewAccountDetails(client);break;
            case AdminExitRequest:
                client_exit(client, -1);break;
            default:
                printf("nothing was caught\n");break;
        } 
    }
    AdminController(client, cookie);
}

int viewAccountDetails(int client){
    int user_id;
    read(client, &user_id, sizeof(user_id));
    view_details(client, user_id);
    return 1;

}
int modify_admin_password(int client){
    char newpass[50];
    char username[50];
    printf("inside modify admin password function\n");
    read(client, newpass, sizeof(newpass));
    read(client, username, sizeof(username));
    printf("newpass recv: %s\n", newpass);
    printf("username recv: %s\n", username);
    int ret = db_modify_admin_password(newpass, username);
    write(client, &ret, sizeof(ret)); 
    if (ret == 1){
        printf("successfully changed\n");
    }
    else{
        printf("couldn't change the password\n");
    }
    return ret;
}

int modify_account_type(int client){
    int account_no;
    int account_type; 
    printf("inside modify account type function\n");
    read(client, &account_no, sizeof(account_no));
    read(client, &account_type, sizeof(account_type)) ;
    printf("account_no read : %d\n", account_no);
    printf("account_type read : %d\n", account_type);
    int ret = db_modify_account_type(account_no, account_type);
    write(client, &ret, sizeof(ret)); 
    if (ret == 1){
        printf("successfully changed\n");
    }
    else
        printf("couldn't modify account type\n");
    return ret;
}

int delete_account(int client){
    int account_no;
    read(client, &account_no, sizeof(account_no));
    int ret = db_delete_account(account_no);
    write(client, &ret, sizeof(ret));
    if (ret == 1){
        printf("successfully deleted\n");
    }
    else{
        printf("couldn't delete\n");
    }
    return ret;
}

int add_account(int client){
    printf("in the add_account function\n");
    int account_no;

   read(client, &account_no, sizeof(account_no));
   printf("account no read :%d\n", account_no);
   int ret = db_add_account(account_no);
   write(client, &ret, sizeof(ret));
   return ret;
}

int client_exit(int client,int userID){
    close(client);
    if (client == -1)
        printf("connection ended with admin\n");
    else
        printf("connection with client%d closed\n", client);
    exit(0);
}

int password_change(int client, int userID){
    char newpass[50];
    int ret;
    read(client, newpass, sizeof(newpass));
    ret = dbpassword_change(newpass, userID);
    write(client, &ret, sizeof(ret));
    return ret;
}

int view_details(int client, int userID){
    printf("in the view_details function\n");
    int *details = malloc(sizeof(int)*4);
    details = dbaccount_details(userID); 
    // 4 integers should be written to client
    write(client, details, sizeof(int)*4);
    
    return 1;
} 

int balance(int client, int userID){
    int balance = dbBalance(userID);
    write(client, &balance, sizeof(balance));
    if (balance > 0)
        return 1;
    else
        return -1;
}


int deposit(int client, int userID){
    int deposit ;
    read(client, &deposit, sizeof(deposit));
    int ret = dbDeposit(deposit, userID);
    write(client, &ret, sizeof(ret));
    return ret;
}

int withdraw(int client, int userID){
    int withdraw;
    printf("inside withdrawfunction\n");
    read(client, &withdraw, sizeof(withdraw));
    int ret = dbWithdraw(withdraw, userID);
    write(client, &ret, sizeof(ret));
    return ret;

}

long GenerateCookie(int val){
    if (val == 1){
        return random();
    }
    else{
        return -1;
    }
}

int normallogin(int client){
    Credentials *loginatt = malloc(sizeof(Credentials));
    printf("waiting for login credentials\n");
    int bytes = read(client, loginatt, sizeof(Credentials));
    int val = ValidateLogin(loginatt);
    long cookie = GenerateCookie(val);


    write(client, &cookie, sizeof(cookie));  
    if (val == 1){
        int userID = loginatt->id;
        mainController(client, userID,  cookie);
    } 
    else{
        loginHandler(client);
    }
    return 0;
} 

int adminlogin(int client){
    AdminCredentials *loginatt = malloc(sizeof(AdminCredentials));
    printf("waiting for admin login credentials\n");
    int bytes = read(client, loginatt, sizeof(AdminCredentials));
    printf("bytes read %d\n", bytes);
    int val = ValidateAdminLogin(loginatt);
    long cookie = GenerateCookie(val);


    write(client, &cookie, sizeof(cookie));  
    if (val == 1){
        AdminController(client, cookie);
    } 
    else{
        loginHandler(client);
    }
    return 0;

}


int loginHandler(int client){
    enum LoginRequest type;
    printf("in Login Handler\n");
    int ret; 
    ret = read(client, &type, sizeof(type));
    printf("bytes read : %d\n", ret);
    if (ret < 0){
        printf("first query is not doing anything\n");
        return -1;
    }
    switch(type){
        case NormalLoginRequest:
            normallogin(client);break;
        case AdminLoginRequest:
            adminlogin(client);break;
    } 
    return 0;
}

int connectSocket(){
    int socket_d;
    struct sockaddr_in serveraddr, clientaddr;
    int opt = 1;

    socket_d = socket(AF_INET, SOCK_STREAM, 0);// creating socket in namespace
    if (socket_d == 0){
        int err = errno;	
        fprintf(stderr, "server end socket creation failed: %s\n", strerror(err));
        return -1;
    }
    //forcing port reuse
    setsockopt(socket_d, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    if(bind(socket_d, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0){
        int err = errno;	
        fprintf(stderr, "server end binding failed: %s\n", strerror(err));
        return -1;
    }
    return socket_d;
}

int connectClient(int socket_d)  {
    // handling 10 users at once at max
    struct sockaddr_in clientaddr;
	socklen_t addr_size = sizeof(clientaddr);
    listen(socket_d, 10);
    int childpid;


    while(1){
        int client = accept(socket_d, (struct sockaddr *)&clientaddr,&addr_size);
        if (client < 0){
            int err = errno;	
            fprintf(stderr, "connection failed: %s\n", strerror(err));
            return -1;
        }
        printf("connection accepted from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        if ((childpid=fork()) == 0){
            close(socket_d);
            loginHandler(client);    
            return 1;
        }
        else{
            close(client);
        }
        
    }
return 0;
}


int main(){

    //seed that will be used to generate random cookies for validation of user.
    srandom(42); 
    
    
    int socket_d = connectSocket();
    connectClient(socket_d);
    return 0;
}

