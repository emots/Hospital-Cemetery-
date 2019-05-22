#include <stdio.h>
#include <time.h>
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
    sizeEGN = 50

} eSizes;

typedef enum
{
    false,
    true
} bool;

typedef struct Cemetery
{
    char nameOfPerson[50];
    char EGN[50];
    int row;
    int column;
    bool isReserved;

} grave;

void AddToCemetery(char *buff);
void CreateDateFile(char *egn);
void ReadFromBinary();

int main()
{

    int sockfd;
    char buffer[MAXLINE];
    char *msgToClient = "The person is registered and a grave is determined! \n";
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int len, n;

    do
    {
        //check data
        ReadFromBinary();

        n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Hospital : %s\n", buffer);

        AddToCemetery(buffer);

        sendto(sockfd, (const char *)msgToClient, strlen(msgToClient),
               MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Response message sent.\n");

    } while (n > 0);

    return 0;
}

void AddToCemetery(char *buff)
{
    char *enter;
    int choice = 0;
    grave grave1;

    memset(&grave1, 0, sizeof(grave));

    enter = strtok(buff, " ");
    strcpy(grave1.nameOfPerson, enter);

    enter = strtok(NULL, " ");
    strcpy(grave1.EGN, enter);

    srand(time(NULL));
    grave1.row = rand();
    grave1.column = rand();

    printf("Enter 1 for reserving the grave for 15 years and 0 to have it forever: \n");
    scanf("%d", &choice);
    grave1.isReserved = choice;

    printf("%s\n", grave1.nameOfPerson);
    printf("%s\n", grave1.EGN);
    printf("%d\n", grave1.row);
    printf("%d\n", grave1.column);
    printf("%d\n", grave1.isReserved);

    int fd = open("CemeteryRegister.bin", O_WRONLY | O_APPEND | O_CREAT);
    if (fd < 0)
    {
        perror("a1");
    }
    
    write(fd, &grave1.nameOfPerson, sizeName * sizeof(char));
    write(fd, &grave1.EGN, sizeEGN * sizeof(char));
    write(fd, &grave1.row, sizeof(int));
    write(fd, &grave1.column, sizeof(int));
    write(fd, &grave1.isReserved, sizeof(bool));

    if (close(fd) < 0)
    {
        perror("a1");
    }

    CreateDateFile(grave1.EGN);
}

void CreateDateFile(char *egn)
{
    FILE *file;
    char filename[15] = {0};
    int y = 0, m = 0, d = 0;

    strcpy(filename, egn);
    strcat(filename, ".txt");
    printf("file: %s\n", filename);

    printf("Enter day, month, year:\n");
    scanf("%d%d%d", &d, &m, &y);
    struct tm t = {.tm_year = y - 1900, .tm_mon = m - 1, .tm_mday = d};

    printf("%s\n", asctime(&t));
    file = fopen(filename, "w");

    t.tm_mday += 40;
    mktime(&t);
    fprintf(file, "40 days: %s\n", asctime(&t));

    t.tm_mday -= 40;
    t.tm_mon += 6;
    mktime(&t);
    fprintf(file, "Six months: %s\n", asctime(&t));

    t.tm_mon -= 6;
    t.tm_year += 1;
    mktime(&t);
    fprintf(file, "One year: %s\n", asctime(&t));

    fclose(file);
}

void ReadFromBinary()
{
    grave grave1;
    memset(&grave1, 0, sizeof(grave));
    int fd = open("CemeteryRegister.bin", O_RDONLY);

    if (fd < 0)
    {
        perror("r1");
    }

    while (read(fd, &grave1.nameOfPerson, sizeName * sizeof(char)) > 0)
    {

        read(fd, &grave1.EGN, sizeEGN * sizeof(char));
        read(fd, &grave1.row, sizeof(int));
        read(fd, &grave1.column, sizeof(int));
        read(fd, &grave1.isReserved, sizeof(bool));

        printf("%s\n", grave1.nameOfPerson);
        printf("%s\n", grave1.EGN);
        printf("%d\n", grave1.row);
        printf("%d\n", grave1.column);
        printf("%d\n", grave1.isReserved);

        memset(&grave1, 0, sizeof(grave));
    }

    if (close(fd) < 0)
    {
        perror("r1");
    }
}