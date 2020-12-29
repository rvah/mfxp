#pragma once

#include "general.h"
#include "config.h"

bool sm_add_site(const char *name, const char *username, const char *password,
		const char *host, const char *port);
bool sm_remove_site(const char *name);
void sm_list_all();
void sm_list(const char *name);
void sm_edit(const char *name, const char *setting, const char *value);
