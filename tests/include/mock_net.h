#pragma once

#include <stdint.h>
#include <openssl/ssl.h>
#include "general.h"

void mock_net_set_socket_request(char **data);
void mock_net_set_socket_response(char **data);
int32_t mock_net_socket_opener(char *address, char *port);
bool mock_net_socket_opener_verify_opened(uint32_t times);
int32_t mock_net_socket_closer(int32_t fd);
ssize_t mock_net_socket_receiver(int fd, void *buf, size_t n, int flags);
ssize_t mock_net_socket_sender(int fd, const void *buf, size_t n, int flags);
int mock_net_socket_secure_receiver(SSL *ssl, void *buf, int num);
int mock_net_socket_secure_sender(SSL *ssl, const void *buf, int num);
bool mock_net_socket_closer_verify_opened(uint32_t times);
void mock_net_reset();
