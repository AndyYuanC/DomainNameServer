#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>
#include "dir.h"
#include "usage.h"
#include "netbuffer.h"
#include "server.h"



#define MAX_LINE_LENGTH 1024

static void handle_client(int fd);
static void while_logged_in(int fd);
static void handle_cwd(int fd, char cmd1[], char cmd2[]);
static void handle_cdup(int fd, char cmd1[], char cmd2[]);
static void handle_type(int fd, char cmd1[], char cmd2[]);
static void handle_mode(int fd, char cmd1[], char cmd2[]);
static void handle_stru(int fd, char cmd1[], char cmd2[]);
static void handle_retr(int fd, char cmd1[], char cmd2[]);
static void handle_pasv(int fd, char cmd1[], char cmd2[]);
static void handle_nlst(int fd, char cmd1[], char cmd2[]);

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Invalid arguments. Expected: %s <port>\n", argv[0]);
        return 1;
    }
    run_server(argv[1], handle_client);

    return 0;
}

void handle_client(int fd) {
    net_buffer_t recvBuf = nb_create(fd, MAX_LINE_LENGTH);
    char buf[MAX_LINE_LENGTH + 1] = {0};
    send_string(fd, "Connected to CSftp.\r\n");
    send_string(fd, "220\r\n");

    while(1){
        nb_read_line(recvBuf, buf);
        buf[strcspn(buf, "\r\n")] = 0;

        if(strcasecmp(buf, "QUIT") == 0){
            break;
        }

        char dup[MAX_LINE_LENGTH];
        strcpy(dup, buf);
        char* tok;
        tok = strtok(dup, " ");

        int acc = 0;
        int flag = 0;
        char a[3][90];
        strcpy(a[0], "@#$%^&");
        strcpy(a[1], "@#$%^&");
        strcpy(a[2], "@#$%^&");
        while(tok != NULL){
            printf("%s\n", tok);
            strcpy(a[acc], tok);
            tok = strtok(NULL, " ");
            acc++;
            if (acc > 2) {
                send_string(fd, "501\r\n");  // Syntax error in parameters or arguments.
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            if ((strcasecmp(a[0], "USER") == 0) && (strncmp(a[1], "@#$%^&", 6) == 0)) {
                send_string(fd, "501\r\n");  // Syntax error in parameters or arguments.
            } else if ((strcasecmp(a[0], "USER") == 0) && (strncmp(a[1], "cs317", 5) != 0)) {
                send_string(fd, "530\r\n");  // Not logged in.
            } else if ((strcasecmp(a[0], "USER") == 0) && (strncmp(a[1], "cs317", 5) == 0)) {
                send_string(fd, "230\r\n");  // Logged in.
                while_logged_in(fd);
                break;
            } else
                send_string(fd, "530\r\n");  // Not logged in.
        }

    }

    send_string(fd, "Goodbye.\r\n");
    nb_destroy(recvBuf);
}

void while_logged_in(fd) {
    net_buffer_t workingBuf= nb_create(fd, MAX_LINE_LENGTH);
    char working_buf[MAX_LINE_LENGTH + 1] = {0};
    send_string(fd, "Making sure we got here.\r\n");
    while(1) {
        nb_read_line(workingBuf, working_buf);
        working_buf[strcspn(working_buf, "\r\n")] = 0;

        if(strcasecmp(working_buf, "QUIT") == 0){
            break;
        }

        char duplicate[MAX_LINE_LENGTH];
        strcpy(duplicate, working_buf);
        char* tok;
        tok = strtok(duplicate, " ");

        int acc = 0;
        int flag = 0;
        char a[3][90];
        strcpy(a[0], "@#$%^&");
        strcpy(a[1], "@#$%^&");
        strcpy(a[2], "@#$%^&");
        while(tok != NULL){
            printf("%s\n", tok);
            strcpy(a[acc], tok);
            tok = strtok(NULL, " ");
            acc++;
            if (acc > 2) {
                send_string(fd, "501\r\n");  // Syntax error in parameters or arguments.
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            if (strlen(a[0]) > 4) {
                send_string(fd, "501\r\n");  // Syntax error in parameters or arguments.
            } else if (strcasecmp(a[0], "CWD") == 0) {
                handle_cwd(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "CDUP") == 0) {
                handle_cdup(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "TYPE") == 0) {
                handle_type(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "MODE") == 0) {
                handle_mode(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "STRU") == 0) {
                handle_stru(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "RETR") == 0) {
                handle_retr(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "PASV") == 0) {
                handle_pasv(fd, a[0], a[1]);
            } else if (strcasecmp(a[0], "NLST") == 0) {
                handle_nlst(fd, a[0], a[1]);
            } else
                send_string(fd,
                            "500\r\n");  // Syntax error, command unrecognized and the requested action did not take place. IS THIS RIGHT?
        }
    }
}

void handle_cwd(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some CWD stuff.\r\n");

}

void handle_cdup(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some CDUP stuff.\r\n");
}

void handle_type(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some TYPE stuff.\r\n");
}

void handle_mode(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some MODE stuff.\r\n");
}

void handle_stru(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some STRU stuff.\r\n");
}

void handle_retr(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some RETR stuff.\r\n");
}

void handle_pasv(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some PASV stuff.\r\n");
    char* pport = "11122";
    int pasv_FD = run_PASV_server(pport, handle_client);
}

void handle_nlst(int fd, char cmd1[], char cmd2[]) {
    send_string(fd, "Better do some NLST stuff.\r\n");
}








//    char str[MAX_LINE_LENGTH];
//    while(1) {
//        printf("Enter string: ");
//        fgets(str, sizeof(str), stdin);
//        str[strcspn(str, "\r\n")] = 0;
//
//        if(strcasecmp(str, "QUIT") == 0){
//            break;
//        }
//
//        char dup[MAX_LINE_LENGTH];
//        strcpy(dup, str);
//        char* tok;
//        tok = strtok(dup, " ");
//        int acc = 0;
//        while(tok != NULL){
//            printf("%s\n", tok);
//            tok = strtok(NULL, " ");
//            acc++;
//        }
//        printf("%i\n", acc);
//    }
//}


    //run_server(argv[1], handle_client);
   // printf("Whelp, we are under way\n");
//    return 0;
//}

//void handle_client(int fd) {
//    net_buffer_t recvBuf = nb_create(fd, MAX_LINE_LENGTH);
//    char buf[MAX_LINE_LENGTH + 1] = {0};
//
//    send_string(fd, "Welcome to Echo Server.\r\n");
//
//    int acc = 0;
//    while(acc < 5){
//        nb_read_line(recvBuf, buf);
//        send_string(fd, buf);
//        acc++;
//    }
//    send_string(fd, "Goodbye.\r\n");
//    nb_destroy(recvBuf);
//}


//
//int main(int argc, char *argv[]) {
//
//    // This is some sample code feel free to delete it
//    // This is the main program for the thread version of nc
//
//    int i;
//
//    // Check the command line arguments
//    if (argc != 2) {
//      usage(argv[0]);
//      return -1;
//    }
//
//    // This is how to call the function in dir.c to get a listing of a directory.
//    // It requires a file descriptor, so in your code you would pass in the file descriptor
//    // returned for the ftp server's data connection
//
//    printf("Printed %d directory entries\n", listFiles(1, "."));
//    return 0;
//
//}
