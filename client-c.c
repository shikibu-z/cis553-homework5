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
#include <arpa/inet.h>

#define SEND_BUFFER_SIZE 2048

/* TODO: client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port)
{
    // initialization
    char buf[SEND_BUFFER_SIZE];
    struct sockaddr_in serv;
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    // check socket status
    if (sockfd == -1)
    {
        fprintf(stderr, "[ERROR] Fail to create socket!\n");
        return 1;
    }

    // generate, connect to server
    bzero(&serv, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(server_port));
    serv.sin_addr.s_addr = inet_addr(server_ip);

    // connect
    if ((connect(sockfd, (struct sockaddr *)&serv, sizeof(struct sockaddr))) == -1)
    {
        fprintf(stderr, "[ERROR] Fail to connect!\n");
        return 1;
    }

    // loop to read stdin, and send
    while (1)
    {
        // using fread, from the open group
        bzero(buf, SEND_BUFFER_SIZE);
        size_t bytes_read = fread(buf, sizeof(char), SEND_BUFFER_SIZE, stdin);
        if (bytes_read < 0)
        {
            fprintf(stderr, "[ERROR] Fail to read stdin!\n");
            return 1;
        }
        else if (bytes_read == 0)
        {
            fprintf(stderr, "[STATUS] Reading EOF!\n");
            return 0;
        }
        else
        {
            // partial send
            int total = 0;
            int bytes_left = bytes_read;
            int n;
            while (total < bytes_read)
            {
                n = send(sockfd, buf + total, bytes_left, 0);
                if (n == -1)
                    return 1;
                total += n;
                bytes_left -= n;
            }
        }
    }
    close(sockfd);
    return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv)
{
    char *server_ip;
    char *server_port;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = argv[2];
    return client(server_ip, server_port);
}
