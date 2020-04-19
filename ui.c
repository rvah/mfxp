#include "ui.h"

void ui_init() {

}

/*
ls
ref
cd dir
put dir/file
get dir/file
rm dir/file
site
quote
*/

char *commands[] = {
	"help",
	"open",
	"close",
	"lls","rls",
	"lref","rref",
	"lcd","rcd",
	"lput","rput",
	"lget","rget",
	"lrm","rrm",
	"lsite","rsite",
	"lquote","rquote",
	"lopen","ropen",
	NULL
};


char *command_name_generator(const char *text, int state) {
	static int list_index, len;
	char *name;

	if (!state) {
		list_index = 0;
		len = strlen(text);
	}

	while ((name = commands[list_index++])) {
		if (strncmp(name, text, len) == 0) {
			return strdup(name);
		}
	}

	return NULL;
}

void parse_command_line(char *line) {
	char *save;
	char *item = strtok_r(strdup(line), " \t", &save);
	
	if(item == NULL) {
		return;
	}

	//check normal commands
	if(strcmp(item, "help") == 0) {
		cmd_help(line);
		return;
	} else if(strcmp(item, "open") == 0) {
		cmd_open(line, ' ');
		return;
	} else if(strcmp(item, "close") == 0) {
		cmd_close(line, ' ');
		return;
	}

	char which = item[0];

	item++;

	//check r/l commands
	if (strcmp(item, "ls") == 0) {
		cmd_ls(line, which);
		return;
	} else if(strcmp(item, "ref") == 0) {
		cmd_ref(line, which);
		return;
	} else if(strcmp(item, "cd") == 0) {
		cmd_cd(line, which);
		return;
	} else if(strcmp(item, "put") == 0) {
		cmd_put(line, which);
		return;
	} else if(strcmp(item, "get") == 0) {
		cmd_get(line, which);
		return;
	} else if(strcmp(item, "rm") == 0) {
		cmd_rm(line, which);
		return;
	} else if(strcmp(item, "site") == 0) {
		cmd_site(line, which);
		return;
	} else if(strcmp(item, "quote") == 0) {
		cmd_quote(line, which);
		return;
	} else if(strcmp(item, "open") == 0) {
		cmd_open(line, which);
		return;
	} else if(strcmp(item, "close") == 0) {
		cmd_close(line, which);
		return;
	}

	printf("Err: bad command.\n");


	/*while(line != NULL) {


		line = strtok_r(NULL, "\n", &save);
	}*/
}

char **tab_auto_complete(const char *text, int start, int end) {
	//printf("S: %s\n",text);
	rl_attempted_completion_over = 1;
	return rl_completion_matches(text, command_name_generator);
}

void ui_loop() {
	bool running = true;
	char *str_in; //[UI_INPUT_MAX];
	struct site_pair *current_pair = NULL;
	char s_prefix[255];
	char *s_noname = "--";
	char *s_lname, *s_rname;

	rl_attempted_completion_function = tab_auto_complete;

	pthread_t ui_thread;
	pthread_create(&ui_thread, NULL, thread_ui, NULL);

	while(running) {
		current_pair = site_get_current_pair();

		if(current_pair->left == NULL) {
			s_lname = s_noname;
		} else {
			s_lname = current_pair->left->name;
		}

		if(current_pair->right == NULL) {
			s_rname = s_noname;
		} else {
			s_rname = current_pair->right->name;
		}
		snprintf(s_prefix, 254, "[ %s <> %s ] >> ", s_lname, s_rname);
		//fgets(str_in, UI_INPUT_MAX , stdin);
		str_in = readline(s_prefix);
		add_history(str_in);
		parse_command_line(str_in);
		free(str_in);
	}
}

void ui_close() {

}
