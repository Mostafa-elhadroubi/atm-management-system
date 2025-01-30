#include "header.h"

const char *RECORDS2 = "./data/records.txt";


int getDataUserFromFile(FILE *fp, struct Record *r) {
    return fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
    &r->id, &r->userId, r->name, &r->accountNbr, &r->deposit.month,
    &r->deposit.day, &r->deposit.year, r->country, &r->phone, &r->amount, r->accountType) ;
}
void saveUpdatedDataToFile(FILE *fp, struct User u, struct Record r) {
    fprintf(fp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
    r.id, r.userId, r.name, r.accountNbr, r.deposit.month,
    r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
}

void resultData(FILE *fp, FILE *tempFp, struct User u, int found, int invalid){
    fclose(fp);
    fclose(tempFp);
    remove(RECORDS2);
    rename("./data/tempFile.txt", RECORDS2);
    if(invalid){
        success(u, "X Failed");
    }
    if (!found) {
        printf("Account ID does not exist for this user!!!!\n");
        success(u, "X Failed");
    } else if(found && !invalid) {
        success(u, "✔ Success");
    }
}
void updateAccountInformation(struct User u) {
    struct Record r;
    int accountId;
    FILE *fp = fopen(RECORDS2, "r");  // Open for reading and updating
    FILE *tempFp = fopen("./data/tempFile.txt", "w");  // Temp file for writing updated data

    if (fp == NULL || tempFp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    system("clear");
    printf("\t\t\t ====== Update your Account: ======\n\n");
    printf("Enter your account id:");
    scanf("%d", &accountId);

    int found = 0;
    while (getDataUserFromFile(fp, &r) != EOF) {
        if (strcmp(r.name, u.name) == 0 && r.accountNbr == accountId) {
            found = 1;
            int option;
            invalidUpdate:
            system("clear");
            printf("\t\t\tWhat do you want to change:\n\n");
            printf("\n\t\t[1]- Phone Number\n");
            printf("\n\t\t[2]- Country\n");
            scanf("%d", &option);
            switch (option) {
                case 1:
                    printf("\n\t\tEnter your new phone number:");
                    scanf("%d", &r.phone);
                    break;
                case 2:
                    printf("\n\t\tEnter your new country:");
                    scanf("%s", r.country);
                    break;
                default:
                    printf("Insert a valid operation!\n");
                    goto invalidUpdate;
            }
        }
            saveUpdatedDataToFile(tempFp, u, r);
    }

    resultData(fp, tempFp, u, found, 0);
}

void accountInformation(struct Record r) {
    printf("The date of the account is: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
    printf("The amount of the account is: %.2f\n", r.amount);
    printf("The accountType is: %s\n", r.accountType);
    float interest;
    if(strcmp(r.accountType, "saving") == 0) {
                interest = (r.amount * 7) / 1200;
                printf("You will get $%.2lf as interest on day 10 of every month\n", interest);
            } else if(strcmp(r.accountType, "fixed01") == 0) {
                interest = (r.amount * 4) / 100;
                printf("You will get $%.2lf of interest on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 1);
            } else if (strcmp(r.accountType, "fixed02") == 0)  {
                interest = ((r.amount * 5) / 100) * 2;
                printf("You will get $%.2lf of interest on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 2);
            } else if (strcmp(r.accountType, "fixed03") == 0) {
                interest = ((r.amount * 8) / 100) * 3;
                printf("You will get $%.2lf of interest on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 3);
            } else if (strcmp(r.accountType, "current") == 0) {
                printf("You will not get interests because the account is of type current\n");
            }
}

void checkAccountInformation(struct User u) {
    struct Record r;
    FILE *fp = fopen(RECORDS2, "r");
    int accountID;
    int found = 0;
    system("clear");
    printf("\t\tEnter your account ID:");
    scanf("%d", &accountID);
    while(getDataUserFromFile(fp, &r) != EOF) {
        if(strcmp(r.name, u.name) == 0 && accountID == r.accountNbr) {
            found = 1;
            accountInformation(r);
        }
    }
    fclose(fp);
    if(!found) {
        printf("This account ID does not exist for this user!!!");
        success(u, "failed");
    } else {
        success(u, "✔ Success");
    }
}

void RemoveAccount(struct User u) {
    FILE *fp = fopen(RECORDS2, "r");
    FILE *ftemp = fopen("./data/tempFile.txt", "w");
    struct Record r;
    int accountId;
    system("clear");
    printf("Enter the account ID to remove: ");
    scanf("%d", &accountId);
    int found = 0;
    while(getDataUserFromFile( fp, &r) != EOF) {
        if(strcmp(r.name, u.name) == 0 && r.accountNbr == accountId) {
            found = 1;
            continue;
        }else {
            saveUpdatedDataToFile(ftemp, u, r);
        }
    }
    resultData(fp, ftemp, u, found, 0);

}

void withdrawAmount(struct User u, int accountID){
    FILE *fp = fopen(RECORDS2, "r");
    FILE *fptemp = fopen("./data/tempFile.txt", "w");
    struct Record r;
    // int accountID;
    float amount;
    int found = 0;
    int invalid = 0;
    // printf("Enter your account ID to withdraw from:");
    // scanf("%d", &accountID);
    while(getDataUserFromFile(fp, &r) != EOF) {
        if(strcmp(u.name, r.name) == 0 && r.accountNbr == accountID)  {
            found = 1;
            printf("Enter the amount to withdraw: $");
            scanf("%f", &amount);
            if (r.amount >= amount){
                // if ((strcmp(r.accountType, "current") == 0 || strcmp(r.accountType, "saving") == 0)) {
                r.amount -= amount;
                saveUpdatedDataToFile(fptemp, u, r);
                // }else {
                //     invalid = 1;
                //     printf("Can not withdraw from this kind of account\n");
                // }
            } else {
                invalid = 1;
                saveUpdatedDataToFile(fptemp, u, r);
                printf("Invalid amount\n");
            }
        } else {
            saveUpdatedDataToFile(fptemp, u, r);
        }
    }
    resultData(fp, fptemp, u, found, invalid);
}

void depositAmount(struct User u, int accountID){
    FILE *fp = fopen(RECORDS2, "r");
    FILE *fptemp = fopen("./data/tempFile.txt", "w");
    struct Record r;
    // int accountID;
    float amount;
    int found = 0;
    int invalid = 0;
    // printf("Enter your account ID to deposit into:");
    // scanf("%d", &accountID);
    while(getDataUserFromFile(fp, &r) != EOF) {
        if(strcmp(u.name, r.name) == 0 && r.accountNbr == accountID)  {
            found = 1;
            printf("Enter the amount to deposit: $");
            scanf("%f", &amount);
            if (amount > 0){
                // if ((strcmp(r.accountType, "current") == 0 || strcmp(r.accountType, "saving") == 0)) {
                r.amount += amount;
                saveUpdatedDataToFile(fptemp, u, r);
                // }else {
                //     invalid = 1;
                //     printf("Can not withdraw from this kind of account\n");
                // }
            } else {
                invalid = 1;
                saveUpdatedDataToFile(fptemp, u, r);
                printf("Amount should be greater than 0\n");
            }
        } else {
            saveUpdatedDataToFile(fptemp, u, r);
        }
    }
    resultData(fp, fptemp, u, found, invalid);
}

int checkAccountType(struct User u, int ID) {
    FILE *fp = fopen(RECORDS2, "r");
    struct Record r;
    while(getDataUserFromFile(fp, &r) != EOF)  {
        if(strcmp(u.name, r.name) == 0 && ID == r.accountNbr && ((strcmp(r.accountType, "saving") == 0) || (strcmp(r.accountType, "current") == 0))) {
            return 1;
        }
    }
    return 0;
}
void makeTransaction(struct User u) {
    
    int option, accountID;
        system("clear");
        printf("\t\t =====Make Transaction======\n");
        
        printf("Enter your account ID:");
        scanf("%d", &accountID);
        if(checkAccountType(u, accountID)){
        while(1){
            system("clear");
            printf("Choose one of the following transaction:\n");
            printf("\t\t[1] Withdraw\n");
            printf("\t\t[2] Deposit\n");
            scanf("%d", &option);
            switch(option) {
                case 1:
                    withdrawAmount(u, accountID);
                    break;
                case 2:
                    depositAmount(u, accountID);
                break;
                default:
                printf("Invalid option. Please choose 1 or 2.\n");
                continue;
            }
        } 
    }else {
            printf("It is not possible to withdraw or deposit for fixed account!!!\n");
            success(u, "X Failed");
        }
}


void transferOwnerShip(struct User u) {
    FILE *fp = fopen(RECORDS2, "r"); // Open for reading and writing
    FILE *tempFp = fopen("./data/tempFile.txt", "w"); // Temporary file to write updated records
    struct Record r;
    int accountID;
    char name[50];
    char fname[50];
    int found = 0;

    if(fp == NULL || tempFp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    system("clear");
    printf("\t\t===== Transfer Ownership =====\n");
    printf("Enter account ID to transfer: ");
    scanf("%d", &accountID);
    printf("Enter the user to transfer to: ");
    scanf("%s", name);

    while(getDataUserFromFile(fp, &r) != EOF) {

        if(strcmp(u.name, r.name) == 0 && accountID == r.accountNbr) {
            found = 1;
            strcpy(r.name, name);
        }

        saveUpdatedDataToFile(tempFp, u, r);
    }
resultData(fp, tempFp, u, found, 0);
}
