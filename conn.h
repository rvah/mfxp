#pragma once
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "general.h"

#include "crypto.h"
#include "filesystem.h"
#include "site.h"

#define CONTROL_BUF_SZ 1024 // max number of bytes we can get at once 
#define CONTROL_LINE_SZ 1024

bool ftp_connect(struct site_info *site);
void ftp_disconnect(struct site_info *site);
