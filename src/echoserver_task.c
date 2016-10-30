/*
 * echoserver_task.c
 *
 *  Created on: Oct 9, 2016
 *      Author: ruschi
 */




#include <stdlib.h>
#include <sys/types.h>
#include <posix/sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128
#define on_error(...)                                                                  \
    {                                                                                  \
        fprintf(stderr, __VA_ARGS__);                                                  \
        fflush(stderr);                                                                \
        exit(1);                                                                       \
    }

void echoServer(void)
{

    int server_fd,  err;
    struct sockaddr_in server;
    char buf[BUFFER_SIZE];
    vTaskDelay(5);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset((char*)&server, 0, sizeof(server));
    server.sin_len = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port =  htons(8000);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);


    err = bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    if (err < 0)
    	return;

    err = listen(server_fd, 3);
    if (err < 0){
    	close(server_fd);
    	return;
    }


    while (1) {
        struct sockaddr_in client_addr;
        socklen_t slen=sizeof(client_addr);

        int client_fd = lwip_accept(server_fd, (struct sockaddr*)&client_addr,  &slen);

        while (1) {
            int read = recv(client_fd, buf, BUFFER_SIZE, 0);

            if (!read)
                break; // done reading
            if (read < 0)
                break;

            err = send(client_fd, buf, read, 0);
            if (err < 0)
                break;
        }
        close(client_fd);
    }

    return;
}
