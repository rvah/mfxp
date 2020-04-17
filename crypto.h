#pragma once

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "general.h"

void ssl_init();
void ssl_cleanup();
SSL_CTX *ssl_get_context();