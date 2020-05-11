#include "config.h"

struct config *app_conf = NULL;

struct config *config_get_conf() {
	return app_conf;
}

void config_init() {
	app_conf = malloc(sizeof(struct config));
	app_conf->sites = NULL;
	app_conf->enable_xdupe = false;
}

void print_site_configs() {
	if(app_conf->sites == NULL) {
		printf("no sites added!\n");
		return;
	}

	struct site_config *p = app_conf->sites;

	while(p != NULL) {
		printf("--- %s ---\n", p->name);
		printf("name: %s\n", p->name);
		printf("host: %s\n", p->host);
		printf("port: %s\n", p->port);
		printf("user: %s\n", p->user);
		printf("pass: %s\n", p->pass);
		p = p->next;
	}
}

void config_cleanup() {
	if(app_conf->sites != NULL) {
		struct site_config *p = app_conf->sites;
		struct site_config *t;

		while(p != NULL) {
			t = p;
			p = p->next;
			free(t);
		}
	}

	free(app_conf);
}

struct site_config *get_site_config(uint32_t id) {
	struct site_config *p = app_conf->sites;

	while(p != NULL) {
		if(p->id == id) {
			return p;
		}

		p = p->next;
	}

	return NULL;
}

struct site_config *get_site_config_by_name(char *name) {
	struct site_config *p = app_conf->sites;

	while(p != NULL) {
		if(strcasecmp(p->name, name) == 0) {
			return p;
		}

		p = p->next;
	}

	return NULL;
}

void add_site_config(struct site_config *conf) {
	if(app_conf->sites == NULL) {
		app_conf->sites = conf;
		return;
	}

	struct site_config *p = app_conf->sites;
	
	while(p->next != NULL) {
		p = p->next;
	}
	p->next = conf;
}

static int ini_read_handler(void* user, const char* section, const char* name, const char* value) {
	char *save;
	char *s_name = strtok_r(strdup(section), "_", &save);

	if(strcmp(s_name, "site") == 0) {
		char *s_id = strtok_r(NULL, "_", &save);
		uint32_t id = atoi(s_id);

		struct site_config *s = get_site_config(id);
		
		if(s == NULL) {
			s = malloc(sizeof(struct site_config));
			s->id = id;
			s->next = NULL;
			add_site_config(s);
		}
		
				
		if(strcmp(name, "name") == 0) {
			strlcpy(s->name, value, 254);
		} else if(strcmp(name, "hostname") == 0) {
			strlcpy(s->host, value, 254);
		} else if(strcmp(name, "port") == 0) {
			strlcpy(s->port, value, 254);
		} else if(strcmp(name, "username") == 0) {
			strlcpy(s->user, value, 254);
		} else if(strcmp(name, "password") == 0) {
			strlcpy(s->pass, value, 254);
		} else {
			printf("%s: bad config key\n", name);
			return 0;
		}
		
	} else if(strcmp(s_name, "general") == 0) {
		if(strcmp(name, "skiplist") == 0) {
			if(!skiplist_init(value)) {
				printf("failed to init skiplist.\n");
				return 0;
			}
		} else if(strcmp(name, "priolist") == 0) {
			if(!priolist_init(value)) {
				printf("failed to init priolist.\n");
				return 0;
			}
		} else if(strcmp(name, "hilight") == 0) {
			if(!hilight_init(value)) {
				printf("failed to init hilight.\n");
				return 0;
			}
		} else if(strcmp(name, "enable_xdupe") == 0) {
			char *s_xdupe = strdup(value);
			str_trim(s_xdupe);
			app_conf->enable_xdupe = strcmp(s_xdupe, "true") == 0;
			free(s_xdupe);	
		} else if(strcmp(name, "default_local_dir") == 0) {
			char *d_val = strdup(value);
			str_trim(d_val);

			//expand ~
			if(d_val[0] == '~') {
				wordexp_t exp_result;
				wordexp(d_val, &exp_result, 0);

				free(d_val);
				d_val = strdup(exp_result.we_wordv[0]);
			}

			if(chdir(d_val) != 0) {
				free(d_val);
				printf("failed to set default dir: %s\n", value);
				return 0;
			}

			free(d_val);
		} else if(strcmp(name, "show_dirlist_on_cwd") == 0) {
			char *s_show = strdup(value);
			str_trim(s_show);
			app_conf->show_dirlist_on_cwd = strcmp(value, "true") == 0;
			free(s_show);
		}
	} else if(strcmp(s_name, "ident") == 0) {
		ident_set_setting(name, value);
	}

	return 1;
}

bool config_read(char *path) {
	config_init();

	if (ini_parse(path, ini_read_handler, NULL) < 0) {
		return false;
	}

	//print_site_configs();

	return true;
}
