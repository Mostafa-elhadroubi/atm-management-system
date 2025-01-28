#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                &r->id,
                &r->userId,
                name,
                &r->accountNbr,
                &r->deposit.month,
                &r->deposit.day,
                &r->deposit.year,
                r->country,
                &r->phone,
                &r->amount,
                r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id + 1,
	    r.userId,
	    u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u, const char *res)
{
    int option;
    printf("\n %s!\n\n", res);
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "r+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    r.id = cr.id;
    r.userId = cr.userId;
    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u, "✔ Success");
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u, "✔ Success");
}


void updateAccountInformation(struct User u) {
    struct Record r;
    int accountId;
    FILE *fp = fopen(RECORDS, "r+");  // Open for reading and updating
    FILE *tempFp = fopen("./data/tempfile.txt", "w+");  // Temp file for writing updated data

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
            // Write the updated record to the temp file
            saveUpdatedDataToFile(tempFp, u, r);
        } else {
            // Write unchanged record to the temp file
            saveUpdatedDataToFile(tempFp, u, r);
        }
    }


    fclose(fp);
    fclose(tempFp);

    // Replace the original file with the updated temporary file
    remove(RECORDS);  // Remove the original file
    rename("./data/tempfile.txt", RECORDS);  // Rename temp file to original file name
    if (!found) {
        system("clear");
        printf("Account does not exist!!\n");
        // success(u, "X Failed");
    }
}

int getDataUserFromFile(FILE *fp, struct Record *r) {
    return fscanf(fp, "%d %d %s %d %d/%d/%d %s %d %lf %s",
    &r->id, &r->userId, r->name, &r->accountNbr, &r->deposit.month,
    &r->deposit.day, &r->deposit.year, r->country, &r->phone, &r->amount, r->accountType) ;
}
void saveUpdatedDataToFile(FILE *fp, struct User u, struct Record r) {
    fprintf(fp, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
    r.id, r.userId, u.name, r.accountNbr, r.deposit.month,
    r.deposit.day, r.deposit.year, r.country, r.phone, r.amount, r.accountType);
}


void checkAccountInformation(struct User u) {
    struct Record r;
    FILE *fp = fopen(RECORDS, "r");
    int accountID;
    int found = 0;
    system("clear");
    printf("\t\tEnter your account ID:");
    scanf("%d", &accountID);
    while(getDataUserFromFile(fp, &r) != EOF) {
        if(strcmp(r.name, u.name) == 0 && accountID == r.accountNbr) {
            float interest;
            found = 1;
            accountInformation(r);
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
    }
    fclose(fp);
    if(!found) {
        printf("This account ID does not exist!!!");
        success(u, "failed");
    } else {
        success(u, "✔ Success");
    }
}

void accountInformation(struct Record r) {
    printf("The date of the account is: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
    printf("The amount of the account is: %.2f\n", r.amount);
    printf("The accountType is: %s\n", r.accountType);
}