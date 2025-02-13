#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User{
    int id;
    char name[50];    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);

int checkUserExist(char a[50]);
void getName(struct User *u);

void success(struct User u, const char *res);
void resultData(FILE *fp, FILE *tempFp, struct User u, int found, int invalid);
// void saveUpdatedDataToFile(FILE *fp, struct User u, struct Record r);
// int getDataUserFromFile(FILE *fp, struct Record *r);

void updateAccountInformation(struct User u);
int getDataUserFromFile(FILE *fp, struct Record *r);
void saveUpdatedDataToFile(FILE *fp, struct User u, struct Record r);

void checkAccountInformation(struct User u);
void accountInformation(struct Record r);

void RemoveAccount(struct User u);

void withdrawAmount(struct User u, int option);
void makeTransaction(struct User u);

void transferOwnerShip(struct User u);
