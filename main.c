#include "general.h"
#include "conn.h"
#include "crypto.h"
#include "filesystem.h"
#include "site.h"
#include "ui.h"
#include "config.h"

void init() {
	ssl_init();
	config_read("config.ini");
}

void destroy() {
	ssl_cleanup();
	config_cleanup();
}

int32_t main( int32_t argc, char **argv ) {
	init();
	ui_init();
	ui_loop();
	ui_close();
	destroy();

	return 0;
}
