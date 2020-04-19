#include "log.h"

FILE *logfd;

bool log_init() {
	logfd = fopen("rfxp.log", "a");
	return logfd != NULL;
}

void log_cleanup() {
	fclose(logfd);
}

void log_w(char *format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(logfd, format, args);
	va_end(args);
	fflush(logfd);
}

void log_ui(uint32_t from_id, uint32_t type, char *format, ...) {
	va_list args;
	va_start(args, format);
	char *s_data = malloc(LOG_UI_MAX_LEN);
	vsnprintf(s_data, LOG_UI_MAX_LEN, format, args);
	va_end(args);

	struct msg *m = malloc(sizeof(struct msg));
	struct ui_log *data = malloc(sizeof(struct ui_log));

	data->str = s_data;
	data->type = type;

	m->to_id = THREAD_ID_UI;
	m->from_id = from_id;
	m->event = EV_UI_LOG;
	m->data = (void *)data;
	msg_send(m);
}
