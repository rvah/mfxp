#include "thread.h"

uint32_t thread_gen_id() {
	static uint32_t id = 1;
	return id++;
}

void thread_ui_handle_event(struct msg *m) {
	struct ui_log *d = m->data;

	switch(m->event) {
	case EV_UI_LOG:
		switch(d->type) {
			case LOG_T_E:
				printf(TCOL_RED "[!]" TCOL_RESET);
				break;
			case LOG_T_W:
				printf(TCOL_YELLOW "[w]" TCOL_RESET);
				break;
			case LOG_T_I:
				printf(TCOL_GREEN "[i]" TCOL_RESET);
				break;
		}

		printf(d->str);
		break;
	}
	
	free(m);
}

void *thread_ui(void *ptr) {
	while(1) {
		struct msg *m = msg_read(THREAD_ID_UI);

		if(m != NULL) {
			thread_ui_handle_event(m);
		}
	}
	return 0;
}

void thread_site_handle_event(struct msg *m, struct site_info *s) {
	switch(m->event) {
	case EV_SITE_LS:
		control_send(s, "STAT -la\n");
		control_recv(s);
		log_ui(s->thread_id, LOG_T_I, "testing ui %d msg\n", 666);
		log_ui(s->thread_id, LOG_T_W, "testing ui %d msg\n", 777);
		log_ui(s->thread_id, LOG_T_E, "testing ui %d msg\n", 888);
		break;
	case EV_SITE_CWD:
		if(m->data == NULL) {
			printf("EV_SITE_CWD: bad path\n");
			break;
		}

		int cmd_len = strlen((char *)m->data) + 6;
		char *s_cmd = malloc(cmd_len);

		snprintf(s_cmd, cmd_len, "CWD %s\n", (char *)m->data);
		log_w(s_cmd);

		control_send(s, s_cmd);
		control_recv(s);
		break;
	}
	//rm message since not needed anymore
	free(m);
}

void *thread_site(void *ptr) {
	struct site_info *site = (struct site_info*) ptr;
	site->thread_id = thread_gen_id();
	printf("THREAD STARTED(#%d): %s\n", site->thread_id, site->name);
	ftp_connect(site);

	printf("going into thread loop\n");
	//thread main loop
	while(1) {
		struct msg *m = msg_read(site->thread_id);

		if(m != NULL) {
			thread_site_handle_event(m, site);
		}
	}

	return 0;
}
