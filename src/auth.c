#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s",&userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

void registerMenu(char a[50],char pass[50]) {
    //clear the terminal screen
    system("clear");
    printf("\n\n\n\t\t\t\t  ==== Register ====\n\t\t\t\t\t User Login:");
    scanf("%s", a);
    printf("\n\n\n\n\n\t\t\t\tEnter the password to register:");
    scanf("%s", pass);
}

int checkUserExist(char a[50]) {
    FILE *fp;
    struct User checkUser;
    if((fp = fopen("./data/users.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    
    while(fscanf(fp, "%d %s %s", &checkUser.id, checkUser.name, checkUser.password) != EOF) {
        if(strcmp(checkUser.name, a) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
void getName(struct User *u) {
    FILE *fp;
    struct User checkUser;
    //return afile pointer used to read from
    if((fp = fopen("./data/users.txt", "a+"))== NULL) {
        printf("Error! opening file");
        exit(1);
    }
    while(fscanf(fp, "%d %s %s", &checkUser.id, checkUser.name, checkUser.password) != EOF){

    }
    fprintf(fp, "%d %s %s\n", checkUser.id + 1, u->name, u->password);
    fclose(fp);
}