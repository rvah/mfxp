#include "command.h"

//general commands
void cmd_help(char *line) {
	printf("<-  -  - - ----\n");
	printf("help coming soon\n");
	printf("---- - -  -  ->\n");
}

char *get_arg(char *line, int i) {
	char *save;
	strtok_r(strdup(line), " \t", &save);
	char *arg = NULL;

	for(int j = 0; j < i; j++) {
		arg = strtok_r(NULL, " \t", &save);
	}

	return arg;
}

void bad_arg(char *cmd) {
	printf("bad arg\n");
}

/*
	command args: open <all>
*/
void cmd_open(char *line, char which) {
	char *arg_site = get_arg(line, 1);
	
	if(which == ' ') {
		printf("not implemented.\n");
		return;
	}

	if(arg_site == NULL) {
		bad_arg("open");
		return;
	}

	struct site_config *site_conf = get_site_config_by_name(arg_site);

	if(site_conf == NULL) {
		printf("could not find site %s.\n", arg_site);	
		return;
	}

	printf("found site %s\n", site_conf->name);
	
	struct site_info *s = site_init(
		site_conf->name,
		site_conf->host,
		site_conf->port,
		site_conf->user,
		site_conf->pass,
		true
	);

	struct site_pair *pair = site_get_current_pair();

	if(which == 'l') {
		pair->left = s;
	} else if(which == 'r') {
		pair->right = s;
	}
	
	pthread_create(&s->thread, NULL, thread_site, s);
	//ftp_connect(pair->left);
	

	//ftp_connect(pair->right);
}

/*
	command args: close <all>
*/
void cmd_close(char *line, char which) {
	struct site_pair *pair = site_get_current_pair();
	
	if(pair == NULL) {
		return;
	}

	ftp_disconnect(pair->left);
	ftp_disconnect(pair->right);
	site_destroy_pair(pair);
}

//site specific commands
void cmd_ls(char *line, char which) {
	printf("ls %c\n", which);
}

void cmd_ref(char *line, char which) {
	printf("ref %c\n", which);
}

void cmd_cd(char *line, char which) {
	printf("cd %c\n", which);
}

void cmd_put(char *line, char which) {
	printf("put %c\n", which);
}

void cmd_get(char *line, char which) {
	printf("get %c\n", which);
}

void cmd_rm(char *line, char which) {
	printf("rm %c\n", which);
}

void cmd_site(char *line, char which) {
	printf("site %c\n", which);
}

void cmd_quote(char *line, char which) {
	printf("quote %c\n", which);
}
