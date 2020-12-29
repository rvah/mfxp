#include "sm.h"

bool sm_add_site(const char *name, const char *username, const char *password,
		const char *host, const char *port) {
	if(get_site_config_by_name(name) != NULL) {
		printf("site with name '%s' already exists.\n", name);
		return false;
	}

	struct site_config *new_site = malloc(sizeof(struct site_config));

	new_site->id = 0;

	//assign new id
	struct site_config *all = config_get_conf()->sites;

	while(all != NULL) {
		if(all->id >= new_site->id) {
			new_site->id = all->id+1;
		}
		all = all->next;
	}

	strlcpy(new_site->name, name, 255);
	strlcpy(new_site->pass, password, 255);
	strlcpy(new_site->user, username, 255);
	strlcpy(new_site->host, host, 255);
	strlcpy(new_site->port, port, 6);

	new_site->tls = true;
	new_site->next = NULL;

	add_site_config(new_site);

	printf("added site '%s'\n", new_site->name);

	//commit changes to disk
	if(!write_site_config_file(config_get_conf()->sites, "noob")) {
		printf("Error writing sitedb to disk!\n");
		return false;
	}

	return true;
}

bool sm_remove_site(const char *name) {
	if((name == NULL) || (get_site_config_by_name(name) == NULL)) {
		printf("could not find any site called '%s'\n", name);
		return false;
	}

	struct site_config *all = config_get_conf()->sites;
	struct site_config *prev = NULL;

	while(all != NULL) {
		if(strcasecmp(all->name, name) == 0) {
			if(prev == NULL) {
				config_get_conf()->sites = all->next;
			} else {
				prev->next = all->next;
			}

			printf("deleted site '%s'\n", all->name);

			free(all);
			break;
		}

		prev = all;
		all = all->next;
	}

	//commit changes to disk
	if(!write_site_config_file(config_get_conf()->sites, "noob")) {
		printf("Error writing sitedb to disk!\n");
		return false;
	}

	return true;
}

void sm_list_all() {
	struct site_config *s = config_get_conf()->sites;
	if(s == NULL) {
		printf("no sites added.\n");
		return;
	}

	printf("Added sites:\n");

	while(s != NULL) {
		printf("%s", s->name);

		s = s->next;

		if(s != NULL) {
			printf(", ");
		}
	}

	printf("\n");
}

void sm_list(const char *name) {
	struct site_config *site = get_site_config_by_name(name);

	if(site == NULL) {
		printf("could not find any site called '%s'\n", name);
	} else {
		printf("ID: %d\n", site->id);
		printf("name: %s\n", site->name);
		printf("host: %s\n", site->host);
		printf("port: %s\n", site->port);
		printf("user: %s\n", site->user);
		printf("pass: <hidden>\n");
	}
}

void sm_edit(const char *name, const char *setting, const char *value) {
	struct site_config *site = get_site_config_by_name(name);

	if(site == NULL) {
		printf("could not find any site called '%s'\n", name);
		return;
	}

	if(strcmp(setting, "name") == 0) {
		if(strlen(value) == 0) {
			printf("error: bad name format\n");
			return;
		}

		strlcpy(site->name, value, 255);
	} else if(strcmp(setting, "host") == 0) {
		if(strstr(value, ":") == NULL) {
			printf("bad host:port format, please fix and try again.\n");
			return;
		}

		char *save;
		char *hport = strdup(value);
		char *host = strtok_r(hport, ":", &save);
		char *port = strtok_r(NULL, ":", &save);

		strlcpy(site->host, host, 255);
		strlcpy(site->port, port, 6);

		free(hport);
	} else if(strcmp(setting, "user") == 0) {
		if(strlen(value) == 0) {
			printf("error: bad username format\n");
			return;
		}

		strlcpy(site->user, value, 255);
	} else if(strcmp(setting, "pass") == 0) {
		if(strlen(value) == 0) {
			printf("error: bad password format\n");
			return;
		}

		strlcpy(site->pass, value, 255);
	} else {
		printf("%s: unknown attribute\n", setting);
		return;
	}

	//commit changes
	if(!write_site_config_file(config_get_conf()->sites, "noob")) {
		printf("Error writing sitedb to disk!\n");
		return;
	}
}
