#include "command.h"

//general commands
void cmd_help(char *line) {
	printf("\nGeneral commands:\n\n");

	printf("cd <dir>\t\t\topen local directory\n");
	printf("close\t\t\t\tclose both sites in pair\n");
	printf("exit/quit\t\t\tclose application\n");
	printf("help\t\t\t\tshow this help(!)\n");
	printf("log <num lines>\t\t\tshow last n lines from log\n");
	printf("ls\t\t\t\tlist current local working directory\n");
	printf("mkdir\t\t\t\tcreate local directory\n");
	printf("open <site1,site2>\t\topen site pair\n");
	printf("rm <dir/file>\t\t\tdelete local file or directory\n\n");

	printf("Site specific commands:\n");
	printf("- prefix command with either 'l' or 'r' to run on left or right site.\n");
	printf("- example: lcd Test_Rls-BLAH/\n\n");

	printf("cd <dir>\t\t\topen directory on site\n");
	printf("close\t\t\t\tclose site connection\n");
	printf("fxp <dir/file>\t\t\tfxp file or directory to the other site in the pair\n");
	printf("get <dir/file>\t\t\tdownload file or directory\n");
	printf("ls\t\t\t\tlist current working directory on site\n");
	printf("mkdir <dir>\t\t\tcreate dir on site\n");
	printf("nfo <file>\t\t\tview remote nfo/sfv/txt/diz\n");
	printf("open <site name>\t\topen connection to site\n");
	printf("put <dir/file>\t\t\tupload file or directory\n");
	printf("quote <raw cmd>\t\t\tsend raw command string to site\n");
	printf("rm <file/dir>\t\t\tdelete file or directory on site\n");
	printf("site <cmd>\t\t\tsend SITE command\n\n");
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

char *get_arg_full(char *line, int i) {
	char *save;
	strtok_r(strdup(line), " \t", &save);

	for(int j = 0; j < (i-1); j++) {
		strtok_r(NULL, " \t", &save);
	}

	//trim
	str_trim(save);

	return save;
}

void bad_arg(char *cmd) {
	printf("bad arg\n");
}

struct site_info *cmd_get_site(char which) {
	struct site_pair *pair = site_get_current_pair();

	if(which == 'l') {
		return pair->left;
	} else if (which == 'r') {
		return pair->right;
	}

	return NULL;
}

void cmd_execute(uint32_t thread_id, uint32_t event, void *data) {
	struct msg *m = malloc(sizeof(struct msg));
	m->to_id = thread_id;
	m->event = event;
	m->data = data;
	msg_send(m);
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
}

/*
	command args: close <all>
*/
void cmd_close(char *line, char which) {
	if(which == ' ') {
		printf("not implemented.\n");
		return;
	}

	struct site_pair *pair = site_get_current_pair();

	struct site_info *s;

	if(which == 'l') {
		s = pair->left;
	} else if (which == 'r') {
		s = pair->right;
	} else {
		// not implemented
	}

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	cmd_execute(s->thread_id, EV_SITE_CLOSE, NULL);

	if(which == 'l') {
		pair->left = NULL;
	} else if(which == 'r') {
		pair->right = NULL;
	}
}

//site specific commands
void cmd_ls(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	cmd_execute(s->thread_id, EV_SITE_LS, NULL);
}

void cmd_ref(char *line, char which) {
	printf("ref %c\n", which);
}

void cmd_cd(char *line, char which) {
	struct site_info *s = cmd_get_site(which);
	
	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);

	if(arg_path == NULL) {
		bad_arg("cd");
		return;
	}

	cmd_execute(s->thread_id, EV_SITE_CWD, (void *)arg_path);	
}

void cmd_put(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);

	if(arg_path == NULL) {
		bad_arg("put");
		return;
	}

	cmd_execute(s->thread_id, EV_SITE_PUT, (void *)arg_path);
}

void cmd_get(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);

	if(arg_path == NULL) {
		bad_arg("get");
		return;
	}
	cmd_execute(s->thread_id, EV_SITE_GET, (void *)arg_path);
}

void cmd_rm(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);

	if(arg_path == NULL) {
		bad_arg("rm");
		return;
	}

	cmd_execute(s->thread_id, EV_SITE_RM, (void *)arg_path);
}

void cmd_site(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg = get_arg_full(line, 1);
	cmd_execute(s->thread_id, EV_SITE_SITE, (void *)arg);
}

void cmd_quote(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg = get_arg_full(line, 1);
	cmd_execute(s->thread_id, EV_SITE_QUOTE, (void *)arg);
}

void cmd_fxp(char *line, char which) {
	struct site_info *s = NULL;
	struct site_info *d = NULL; 

	struct site_pair *pair = site_get_current_pair();

	if(which == 'l') {
		s = pair->left;
		d = pair->right;
	} else {
		s = pair->right;
		d = pair->left;
	}


	if(s == NULL) {
		printf("src site not connected.\n");
		return;
	}

	if(d == NULL) {
		printf("dst site not connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);
	
	if(arg_path == NULL) {
		bad_arg("fxp");
		return;
	}

	struct fxp_arg *fa = malloc(sizeof(struct fxp_arg));
	fa->filename = arg_path;
	fa->dst = d;

	cmd_execute(s->thread_id, EV_SITE_FXP, (void*)fa);
}

void cmd_mkdir(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg = get_arg_full(line, 1);
	cmd_execute(s->thread_id, EV_SITE_MKDIR, (void *)arg);
}

void cmd_quit(char *line, char which) {
	//TODO: proper cleanup needed :)
	exit(0);
}

void cmd_log(char *line, char which) {
	char *arg = get_arg(line, 1);

	uint32_t n_lines = 50;

	if((arg != NULL) && (strlen(arg) > 0)) {
		str_trim(arg);

		n_lines = atoi(arg);

		//put a reasonable limit..
		if(n_lines > 10000) {
			n_lines = 10000;
		}

		if(n_lines == 0) {
			n_lines = 50;
		}
	}

	log_print(n_lines);
}

void cmd_nfo(char *line, char which) {
	struct site_info *s = cmd_get_site(which);

	if(s == NULL) {
		printf("no site connected.\n");
		return;
	}

	char *arg_path = get_arg_full(line, 1);

	if(arg_path == NULL) {
		bad_arg("nfo");
		return;
	}

	char *d = strdup(arg_path);
	str_tolower(d);

	const char *pos = strrchr(d, '.');

	if(!pos || pos == d) {
		printf("unable to read file type\n");
		return;
	}

	if( (strcmp(pos+1, "nfo") != 0) && (strcmp(pos+1, "sfv") != 0) && (strcmp(pos+1, "diz") != 0) && (strcmp(pos+1, "txt") != 0) ) {
		printf("bad filetype, supported filetypes: nfo, sfv, diz, txt\n");
		return;
	}

	free(d);

	cmd_execute(s->thread_id, EV_SITE_VIEW_NFO, (void *)arg_path);
}

void cmd_local_ls(char *line) {
	struct file_item *fl = local_ls("./", false);
	struct file_item *prev = NULL;

	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	log_ui(THREAD_ID_UI, LOG_T_I, TCOL_GREEN "[%s]:\n" TCOL_RESET, cwd);

	while(fl != NULL) {
		//TODO: clean up
		char *f_size = parse_file_size(fl->size);

		if(fl->skip) {
			log_ui(THREAD_ID_UI, LOG_T_I, TCOL_RED "%5s %s\n" TCOL_RESET, f_size, fl->file_name);
		} else if(fl->hilight) {
			log_ui(THREAD_ID_UI, LOG_T_I, TCOL_YELLOW "%5s %s\n" TCOL_RESET, f_size, fl->file_name);
		} else {
			switch(fl->file_type) {
			case FILE_TYPE_FILE:
				log_ui(THREAD_ID_UI, LOG_T_I, TCOL_PINK "%5s %s\n" TCOL_RESET, f_size, fl->file_name);
				break;
			case FILE_TYPE_DIR:
				log_ui(THREAD_ID_UI, LOG_T_I, TCOL_GREEN "%5s %s/\n" TCOL_RESET, f_size, fl->file_name);
				break;
			case FILE_TYPE_LINK:
				log_ui(THREAD_ID_UI, LOG_T_I, TCOL_CYAN "%5s %s\n" TCOL_RESET, f_size, fl->file_name);
				break;
			}
		}

		free(f_size);
		prev = fl;
		fl = fl->next;
		free(prev);
	}
}

void cmd_local_cd(char *line) {
	char *arg_path = get_arg_full(line, 1);

	if( (arg_path == NULL) || (strlen(arg_path) == 0)) {
		printf("bad path\n");
	}

	str_trim(arg_path);

	if(chdir(arg_path) != 0) {
		log_ui(THREAD_ID_UI, LOG_T_E,"%s: error CWD\n", arg_path);
		return;
	}

	log_ui(THREAD_ID_UI, LOG_T_I,"cwd successful.\n");

	return;
}

void cmd_local_rm(char *line) {
	printf("not implemented.\n");
	return;
}

void cmd_local_mkdir(char *line) {
	printf("not implemented.\n");
	return;
}
