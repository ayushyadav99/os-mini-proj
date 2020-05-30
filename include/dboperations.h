/*
 * dboperations.h
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H

#include "datastructures.h"

int ValidateLogin(Credentials *loginattm);
int ValidateAdminLogin(AdminCredentials *loginattm);
int dbDeposit(int deposit, int userID);
int dbWithdraw(int withdraw, int userID);
int dbBalance(int userID);
int dbpassword_change(char *newpass, int userID);
int *dbaccount_details(int userID);
int db_delete_account(int val);
int db_modify_account_type(int account_no, int account_type);
int db_modify_admin_password(char *newpass, char *username);
int db_add_account(int account_no);

#endif /* !DBOPERATIONS_H */
