# Banking Management System

The repository contains a simple banking management system created using low level C programming coupled with system calls. 

## TO RUN

1. Populate the database with dummy values by running the following commands:
    - `gcc -I ../include populate_admins.c`
    - `./a.out`

    - `gcc -I ../include populate_users.c`
    - `./a.out`

    - `gcc -I ../include populate_accounts.c`
    - `./a.out`

2. The server and client can be compiled using the following commands :
    - `gcc -I include server.c dboperations.c -o server `
    -` gcc -I include client.c -o client`

3. The server and client can be instantiated using:
    - `./server`
    - `./client`


## Features

- The database has been populated with the following accounts:
    - User account : ID: 1
                     Password: abcd123
    - Admin account: username: admin1
                     password: adminlyf
(These can be used for testing)

1. There can be 2 kinds of accounts on the platform:
    - Admin account
    - User account

### User Account
    Every User is assigned one account (since, it doesn't make sense to have a user with no account in the system).
    -Deposit: Deposit money to assigned account
    -Withdraw: Withdraw money from assigned account
    -Balance Enquiry: Recieve updated balance 
    -Password Change: Change the password for a user.
    -Account Details: Retrieve account details. (also includes account type)
    -Exit: Quit the application.
    
### Admin Account
    Admin account have special priviledges to control the usage of the database.
    -Add Account: Input a new account ID to create a new user account. A new user ID is also created which is associated with this new account (since every account has to be assigned to some user, and every user can be linked to only one account). 
    -Delete Account: Input an account number to delete it. 
    -Balance Enquiry: input an account number to query for it's current balance. 
    -Password Change: Change the password for the admin .
    -Account Details: Input the account number and recieve it's details. 
    -Exit: Quit the application.
    
