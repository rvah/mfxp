#include "thread.h"

void *thread_site(void *ptr) {
	struct site_info *site = (struct site_info*) ptr;
	printf("THREAD STARTED: %s\n", site->name);
	ftp_connect(site);
	return 0;
}
