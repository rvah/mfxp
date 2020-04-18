#pragma once
#include <pthread.h>
#include "general.h"

#define EV_SITE_LS 1

struct msg {
	uint32_t to_id;
	uint32_t event;
	void *data;
	struct msg *next;
	struct msg *last;
};

bool msg_init();
void msg_print_q();
void msg_send(struct msg *msg);
struct msg *msg_read(uint32_t id);
