#pragma once
#include "general.h"
#include "libs/inih/ini.h"
#include "skiplist.h"
#include "priolist.h"
#include "hilight.h"
#include "ident.h"

void config_cleanup();
struct site_config *get_site_config_by_name(char *name);
bool config_read(char *path);

struct site_config {
	uint32_t id;
	char name[255];
	char host[255];
	char port[5];
	char user[255];
	char pass[255];
	bool tls;
	struct site_config *next;
};

struct config {
	struct site_config *sites;
	bool enable_xdupe;
	bool show_dirlist_on_cwd;
};

struct config *config_get_conf();
