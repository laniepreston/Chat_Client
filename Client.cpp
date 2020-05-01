//
// Created by Floor Kouwenberg on 12-2-2019.
//

#include "Client.h"

/**
 * Assignment 2
 *
 * See the lab manual for the assignment description.
 */
void Client::tick() {
    const int FLAGS = 0;
    const int MAX = 1000;
    char smsg[MAX], rmsg[MAX];
    char username;
    memset(smsg, 0, MAX);
    memset(rmsg, 0, MAX);

    std::cout << "Enter command: ";
    while (fgets(smsg, MAX, stdin) != NULL) {

        //implementing the quit command
        if (strcmp(smsg, "!quit\n")==0){
            stopApplication();
            this->closeSocket();
            return;
        }

        //implementing the who command
        if (strcmp(smsg, "!who\n") == 0) {
            send(sock, (char *) "WHO\n", strlen("WHO\n"), FLAGS);

            recv(sock, rmsg, MAX, FLAGS);
            while (strstr(rmsg, "WHO-OK") == 0) {
                memset(rmsg, 0, MAX);
                recv(sock, rmsg, MAX, FLAGS);
            }
            std::cout << rmsg;
            memset(rmsg, 0, MAX);
            memset(smsg, 0, MAX);
            return;
        }

        //implementing the send command
        if (strstr(smsg, "@") !=0) {

            //send the right command to the server
            char *ps = smsg + 1;
            char smsg1[MAX];
            strcpy(smsg1, "SEND ");
            strcat(smsg1, ps);
            send(sock, smsg1, strlen(smsg1), FLAGS);

            //checking for send ok
            recv(sock, rmsg, MAX, FLAGS);
            while (strstr(rmsg, "SEND-OK") == 0) {
                memset(rmsg, 0, MAX);
                recv(sock, rmsg, MAX, FLAGS);
            }
            std::cout << rmsg;

            //checking for delivery
            while (strstr(rmsg, "DELIVERY") == 0){
                memset(rmsg, 0, MAX);
                recv(sock, rmsg, MAX, FLAGS);
            }
            std::cout << rmsg;


            memset(rmsg, 0, MAX);
            memset(smsg, 0, MAX);
            return;
        }
    }
}

/**
 * Assignment 4
 *
 * See the lab manual for the assignment description.
 */
int Client::readFromStdin() {
    return 0;
}

/**
 * Assignment 4
 *
 * See the lab manual for the assignment description.
 */
int Client::readFromSocket() {
    return 0;
}



void Client::createSocketAndLogIn() {

    //Declarations
    const int FLAGS = 0;
    const int MAX = 1000;
    SOCKADDR_IN info;
    char smsg[MAX], rmsg[MAX];
    char username;
    memset(smsg, 0, MAX);
    memset(rmsg, 0, MAX);


    //Starting up
    if (sock_init()!=0){
        printf("Error at start");
        return;
    };


    //Creating socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!sock_valid(sock)) {
        std::cout << "Error when creating socket. Error code: " << sock_error_code() << "\n";
        return;
    }

    //Connecting to socket
    info.sin_family = AF_INET;
    info.sin_port = htons(5378);
    info.sin_addr.s_addr = inet_addr("52.58.97.202");
    if (connect (sock, (SOCKADDR *)&info, sizeof(info)) == SOCKET_ERROR){
        std::cout << "Error when connecting to socket" << sock_error_code() << "\n";
        return;
    }


    //Logging in
    std::cout << "Enter username: ";
    strcpy(smsg, "HELLO-FROM");

    while(fgets(&username, MAX, stdin) != NULL) {
        strcat(smsg, &username);
        send(sock, smsg, strlen(smsg), FLAGS);
        recv(sock, rmsg, MAX, FLAGS);
        if (strstr(rmsg, "HELLO ") !=NULL){
            std::cout << "You are logged in\n";
            break;
        }
        else{
            memset(smsg, 0, MAX);
            memset(rmsg, 0, MAX);
            strcpy(smsg, "HELLO-FROM");
            std::cout << "Enter username again:" << " ";
        }
    };
}

void Client::closeSocket() {
    sock_close(sock);
    sock_quit();
    return;
}
