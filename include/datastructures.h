/*
 * datastructures.h
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

enum LoginRequest{
    NormalLoginRequest,
    AdminLoginRequest,
};

enum AdminRequestType{
    AdminDummyRequest,
    AddAccountRequest,
    DeleteAccountRequest,
    ModifyAccountTypeRequest,
    ModifyPasswordRequest,
    AccountDetailsRequest,
    AdminExitRequest,
};
enum RequestType{
    DummyRequest,
    DepositRequest,
    WithdrawalRequest,    
    BalanceRequest,
    PasswordChangeRequest,
    viewDetailsRequest,
    ExitRequest,
};

enum AccountType{
    singleAccount,
    jointAccount
};

enum TransactionType{
    Deposit,
    Withdrawal
};

typedef struct Credentials{
    int id;
    char password[50];
}Credentials;

typedef struct AdminCredentials{
    char username[50];
    char password[50];
}AdminCredentials;

typedef struct Account{
    int id;
    int balance;
}Account;

typedef struct User{
    int id;
    char password[50];
    int account_id;
    enum AccountType account_type;
}User;

//no features developed using transaction
typedef struct Transaction{
    int id;
    int acc_id;
    int user_id;
    int opening_balance;
    int closing_balance;
    enum TransactionType transaction_type;
}Transaction;


#endif /* !DATASTRUCTURES_H */



