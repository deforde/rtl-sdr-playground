#include "server.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int accept_connection(uint16_t port)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        fprintf(stderr, "Socket failed: %i: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        fprintf(stderr, "Setsockopt failed: %i: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        fprintf(stderr, "Bind failed: %i: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0) {
        fprintf(stderr, "Listen failed: %i: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        fprintf(stderr, "Accept failed: %i: %s.\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return new_socket;
}
