#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

typedef enum Sizes
{
    sizeName = 50,
    sizeEGN = 50,
    sizeYear = 10,
    sizeDiag = 50,
    sizeProg = 50

} eSizes;

typedef struct _register
{
    char nameOfPatient[sizeName];
    char EGN[sizeEGN];
    char years[sizeYear];
    char diagnosis[sizeDiag];
    char prognosis[sizeProg];
    struct _register *next;

} registert;

registert *root = NULL;
registert *node = NULL;
registert *local_tmp = NULL;

int Menu();

void AppendToBinary();
void ReadFromBinary();
void WriteToBinary();

void AddPatient();
void KillPatient();
void Print();
void sendToGraveyard(char *mail);

int main()
{
    while (1)
    {
        switch (Menu())
        {
        case 1:
            AddPatient();
            break;
        case 2:
            KillPatient();
            break;
        case 3:
            Print();
            break;
        case 0:
        {
            exit(0);
        }
        default:
            break;
        }
    }
    return 0;
}

int Menu()
{
    int i;
    printf("****************      WELCOME      ******************\n");
    printf("****************        TO         ******************\n");
    printf("**************** M A I N   M E N U ******************\n");
    printf("*                                                   *\n");
    printf("* 1. Add patient!                                   *\n");
    printf("* 2. Kill patient!                                  *\n");
    printf("* 3. Print all patients!                            *\n");
    printf("* 0. Exit                                           *\n");
    printf("*                                                   *\n");
    printf("*****************************************************\n");
    printf("*****************************************************\n");
    printf("*****************************************************\n");

    do
    {
        printf("Select Option: ");
        scanf("%d", &i);

    } while (i < 0 || i > 3);

    return i;
}

void AppendToBinary()
{
    int fd = open("hospitalRegister.bin", O_WRONLY | O_APPEND);
    if (fd < 0)
    {
        perror("a1");
    }

    write(fd, node->nameOfPatient, sizeName);
    write(fd, node->EGN, sizeEGN);
    write(fd, node->years, sizeYear);
    write(fd, node->diagnosis, sizeDiag);
    write(fd, node->prognosis, sizeProg);

    if (close(fd) < 0)
    {
        perror("a1");
    }
}

void ReadFromBinary()
{
    root = node = NULL;
    char buff[50] = {0};

    int fd = open("hospitalRegister.bin", O_RDONLY | O_CREAT);
    access("hospitalRegister.bin", R_OK | W_OK);

    if (fd < 0)
    {
        perror("r1");
    }

    while (1)
    {
        if (read(fd, &buff, sizeof(char) * sizeName) <= 0)
            break;

        if (root == NULL)
        {
            root = (registert *)calloc(5, sizeof(registert));
            node = root;
            node->next = NULL;
        }
        else
        {
            node->next = (registert *)calloc(5, sizeof(registert));
            node = node->next;
            node->next = NULL;
        }

        strcpy(node->nameOfPatient, buff);

        memset(buff, 0, sizeof(buff));
        read(fd, &buff, sizeof(char) * sizeEGN);
        strcpy(node->EGN, buff);

        memset(buff, 0, sizeof(buff));
        read(fd, &buff, sizeof(char) * sizeYear);
        strcpy(node->years, buff);

        memset(buff, 0, sizeof(buff));
        read(fd, &buff, sizeof(char) * sizeDiag);
        strcpy(node->diagnosis, buff);

        memset(buff, 0, sizeof(buff));
        read(fd, &buff, sizeof(char) * sizeProg);
        strcpy(node->prognosis, buff);

        memset(buff, 0, sizeof(buff));
    }
    if (close(fd) < 0)
    {
        perror("r1");
    }
}

void WriteToBinary()
{
    int fd = open("hospitalRegister.bin", O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        perror("w1");
    }

    node = root;
    if (node != NULL)
    {
        while (1)
        {
            write(fd, node->nameOfPatient, sizeName);
            write(fd, node->EGN, sizeEGN);
            write(fd, node->years, sizeYear);
            write(fd, node->diagnosis, sizeDiag);
            write(fd, node->prognosis, sizeProg);

            if (node->next == NULL)
                break;
            node = node->next;
        }
    }

    if (close(fd) < 0)
    {
        perror("w1");
    }
}

void AddPatient()
{
    ReadFromBinary();

    char nameOfPatient[sizeName] = {0};
    char EGN[sizeEGN] = {0};
    char years[sizeYear] = {0};
    char diagnosis[sizeDiag] = {0};
    char prognosis[sizeProg] = {0};

    if (root == NULL)
    {
        //root = malloc(sizeof(_register));
        root = (registert *)calloc(5, sizeof(registert));
        node = root;
        node->next = NULL;
    }
    else
    {
        //node->next = malloc(sizeof(register_t));
        node->next = (registert *)calloc(5, sizeof(registert));
        node = node->next;
        node->next = NULL;
    }

    printf("Enter name of patient:\n");
    scanf("%s", nameOfPatient);

    printf("Enter EGN of patient:\n");
    scanf("%s", EGN);

    printf("Enter years of patient:\n");
    scanf("%s", years);

    printf("Enter diagnosis of patient:\n");
    scanf("%s", diagnosis);

    printf("Enter prognosis of patient:\n");
    scanf("%s", prognosis);

    strcpy(node->nameOfPatient, nameOfPatient);
    strcpy(node->EGN, EGN);
    strcpy(node->years, years);
    strcpy(node->diagnosis, diagnosis);
    strcpy(node->prognosis, prognosis);

    printf("\nStruct data:\n");
    printf("%s\n", node->nameOfPatient);
    printf("%s\n", node->EGN);
    printf("%s\n", node->years);
    printf("%s\n", node->diagnosis);
    printf("%s\n", node->prognosis);

    AppendToBinary();
    printf("\nAppend done!\n");
}

void KillPatient()
{
    char killName[100] = {0};
    int kill = 0;

    ReadFromBinary();

    printf("Enter name of patient to kill:\n");
    scanf("%s", killName);

    local_tmp = node = root;
    if (root != NULL)
    {
        while (1)
        {
            if ((strcmp(local_tmp->nameOfPatient, killName)) == 0)
            {
                strcat(killName, " ");
                strcat(killName, local_tmp->EGN);
                kill = 1;
                break;
            }
            else if (local_tmp->next != NULL)
            {
                node = local_tmp;
                local_tmp = local_tmp->next;
            }
            else
                break;
        }

        if (kill == 1)
        {
            if (local_tmp == root)
            {
                root = root->next;
            }

            else
                node->next = local_tmp->next;

            free(local_tmp);
            WriteToBinary();
            kill = 0;
            printf("Patient killed!.\n");
            sendToGraveyard(killName);
        }
        else
        {
            printf("There is no such patient!\n");
        }
    }
}

void Print()
{
    ReadFromBinary();
    node = root;
    if (node != NULL)
    {
        while (1)
        {
            printf("\nElement data:\n");
            printf("%s\n", node->nameOfPatient);
            printf("%s\n", node->EGN);
            printf("%s\n", node->years);
            printf("%s\n", node->diagnosis);
            printf("%s\n", node->prognosis);

            if (node->next == NULL)
                break;
            node = node->next;
        }
    }
}

void sendToGraveyard(char *mail)
{
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from Hospital";
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;

    sendto(sockfd, (const char *)mail, strlen(mail),
           MSG_CONFIRM, (const struct sockaddr *)&servaddr,
           sizeof(servaddr));
    printf("Patient sent.\n");

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                 MSG_WAITALL, (struct sockaddr *)&servaddr,
                 &len);
    buffer[n] = '\0';
    printf("Graveyard : %s\n", buffer);

    close(sockfd);
}