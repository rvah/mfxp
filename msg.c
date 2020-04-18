#include "msg.h"

struct msg *msg_q = NULL;
pthread_mutex_t msg_mtx;

bool msg_init() {
	if(pthread_mutex_init(&msg_mtx, NULL) != 0) {
		return false;
    }

	return true;
}

void msg_print_q() {
	struct msg *p = msg_q;

	while(p != NULL) {
		printf("%d->", p->to_id);
		p = p->next;
	}

	printf("\n");
}

void __msg_send(struct msg *msg) {
	if(msg == NULL) {
		return;
	}

	msg->next = NULL;
	msg->last = NULL;

	if(msg_q == NULL) {
		msg->last = msg;
		msg_q = msg;
		return;
	}

	msg_q->last->next = msg;
	msg_q->last = msg;
}

void msg_send(struct msg *msg) {
	pthread_mutex_lock(&msg_mtx);
	__msg_send(msg);
	pthread_mutex_unlock(&msg_mtx);
}

struct msg *__msg_read(uint32_t id) {
	struct msg *ret = NULL;

	if(msg_q == NULL) {
		return ret;
	}

	if(msg_q->to_id == id) {
		ret = msg_q;
		
		//if last in queue, clear q
		if(msg_q->next == NULL) {
			msg_q = NULL;
		} else { //else remove element and push fwd next
			msg_q->next->last = msg_q->last;
			msg_q = msg_q->next;
		}
	}

	return ret;
}

struct msg *msg_read(uint32_t id) {
	pthread_mutex_lock(&msg_mtx);
	struct msg *ret = __msg_read(id);
	pthread_mutex_unlock(&msg_mtx);

	return ret;
}
