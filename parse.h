#pragma once
#include "general.h"

bool parse_pasv(const char *in, char *out_addr, uint32_t *out_port);
char *parse_pwd(const char *s);
