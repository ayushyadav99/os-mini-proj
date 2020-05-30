/*
 * server.h
 * Copyright (C) 2020 ayushyadav99 <ayushyadav99@ayushyadav99-VirtualBox>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SERVER_H
#define SERVER_H

int balance(int client, int userID);
int deposit(int client, int userID);
int withdraw(int client, int userID);
int password_change(int client, int userID);
int view_details(int client, int userID);
int client_exit(int client, int userID);
int normallogin(int client);
int adminlogin(int client);
int loginHandler(int client);
int mainController(int client, int userID, long cookie);
int modify_account_type(int client);
int modify_admin_password(int client);
int add_account(int client);
int viewAccountDetails(int client);
int delete_account(int client);
#endif /* !SERVER_H */
