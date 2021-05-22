#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port)
{
    // initialize
    char buf[RECV_BUFFER_SIZE];
    struct sockaddr_in serv;
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    // check socket status
    if (sockfd == -1)
    {
        fprintf(stderr, "[ERROR] Fail to create socket!\n");
        return 1;
    }

    // start server
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(server_port));
    serv.sin_addr.s_addr = INADDR_ANY;

    // check bind and listen
    if ((bind(sockfd, (struct sockaddr *)&serv, sizeof(serv))) == -1)
    {
        fprintf(stderr, "[ERROR] Fail to bind!\n");
        return 1;
    }
    if ((listen(sockfd, QUEUE_LENGTH)) == -1)
    {
        fprintf(stderr, "[ERROR] Fail to listen!\n");
        return 1;
    }

    // loop to receive
    while (1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int result = accept(sockfd, (struct sockaddr *)&client, &len);
        if (result == -1)
            continue;
        while (1)
        {
            bzero(buf, RECV_BUFFER_SIZE);
            int bytes_recv = recv(result, buf, RECV_BUFFER_SIZE, 0);
            if (bytes_recv <= 0)
                break;
            else
            {
                fwrite(buf, sizeof(char), bytes_recv, stdout);
                fflush(stdout);
            }
        }
    }
    return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv)
{
    char *server_port;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./server-c [server port]\n");
        exit(EXIT_FAILURE);
    }

    server_port = argv[1];
    return server(server_port);
}
