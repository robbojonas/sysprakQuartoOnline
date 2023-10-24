#include "connectServer.h"

int sock;
struct sockaddr_in server;
struct hostent *he;

int connectServer(int port, char *host)
{
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }
    if ((he = gethostbyname(host)) == NULL)
    {
        perror("Could not resolve hostname");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy(&server.sin_addr.s_addr, he->h_addr_list[0], he->h_length);

    // socket auf non- blocking stellen
    long arg;
    if ((arg = fcntl(sock, F_GETFL, NULL)) < 0)
    {
        perror("Could not get socket status");
        exit(EXIT_FAILURE);

        arg |= O_NONBLOCK;
        if (fcntl(sock, F_SETFL, arg) < 0)
        {
            perror("Could not set socket to nonblocking");
            exit(EXIT_FAILURE);
        }
    }

    printf("Connecting ...\n");
    int conn_fd = connect(sock, (struct sockaddr *)&server, sizeof(server));
    if (conn_fd != 0)
    {
        perror("Could not connect to server");
        exit(EXIT_FAILURE);
    }

    fd_set fset;
    struct timeval tv;

    do
    {
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;

        FD_ZERO(&fset);
        FD_SET(sock, &fset);

        conn_fd = select(sock + 1, NULL, &fset, NULL, &tv);
        if (conn_fd > 0)
        {
            printf("Connected to %s\n\n", host);
            break;
        }
        else if (conn_fd < 0 && errno != EINTR)
        {
            perror("Could not connect to server");
            exit(EXIT_FAILURE);
        }
        else
        {
            perror("Connection failed: timeout");
            exit(EXIT_FAILURE);
        }

    } while (1);

    // Socket auf blocking stellen
    if ((arg = fcntl(sock, F_GETFL, NULL)) < 0)
    {
        perror("Could not get socket status");
        exit(EXIT_FAILURE);
    }
    arg &= (~O_NONBLOCK);
    if (fcntl(sock, F_SETFL, arg) < 0)
    {
        perror("Could not set socket status to blocking");
        exit(EXIT_FAILURE);
    }
    return sock;
}
