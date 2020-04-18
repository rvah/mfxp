#include "thread.h"

uint32_t thread_gen_id() {
	static uint32_t id = 1;
	return id++;
}

void thread_handle_event(struct msg *m, struct site_info *s) {
	switch(m->event) {
	case EV_SITE_LS:
		control_send(s, "STAT -la\n");
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
			thread_handle_event(m, site);
		}
	}

	return 0;
}
