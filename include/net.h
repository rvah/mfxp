#pragma once

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <openssl/ssl.h>

#include "general.h"
#include "log.h"

#define CONN_BACKLOG 20

void *net_get_in_addr(struct sockaddr *sa);
int32_t net_open_server_socket(char *port);
int32_t net_open_socket(char *address, char *port);
int net_open_socket_secure(SSL *ssl);
int32_t net_close_socket(int32_t fd);
ssize_t net_socket_recv(int fd, void *buf, size_t n, int flags);
ssize_t net_socket_send(int fd, const void *buf, size_t n, int flags);
int net_socket_secure_recv(SSL *ssl, void *buf, int num);
int net_socket_secure_send(SSL *ssl, const void *buf, int num);
void net_set_socket_opener(int32_t (*opener)(char *, char *));
void net_set_socket_secure_opener(int (*secure_opener)(SSL *));
void net_set_socket_closer(int32_t (*closer)(int32_t));
void net_set_socket_receiver(ssize_t (*receiver)(int, void *, size_t, int));
void net_set_socket_sender(ssize_t (*sender)(int, const void *, size_t, int));
void net_set_socket_secure_receiver(int (*receiver)(SSL *, void *, int));
void net_set_socket_secure_sender(int (*sender)(SSL *, const void *, int));
