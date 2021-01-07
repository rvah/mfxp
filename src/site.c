#include "site.h"
#include "linked_list.h"

static struct site_pair *current_pair = NULL;
static struct linked_list *all_pairs = NULL;
static struct linked_list *all_sites = NULL;

/*
 * ----------------
 *
 * Private functions
 *
 * ----------------
 */

static void list_add_site(struct site_info *site) {
	if(all_sites == NULL) {
		all_sites = linked_list_create();
	}

	linked_list_add(all_sites, site);
}

static void list_add_pair(struct site_pair *pair) {
	if(all_pairs == NULL) {
		all_pairs = linked_list_create();
		current_pair = pair;
	}

	linked_list_add(all_pairs, pair);
}

/*
 * ----------------
 *
 * Public functions
 *
 * ----------------
 */

struct site_info *site_init(char *name, char *address, char *port,
		char *username, char *password, bool use_tls) {
	struct site_info *site = malloc(sizeof(struct site_info));

	strlcpy(site->name, name, SITE_NAME_MAX);
	strlcpy(site->address, address, SITE_HOST_MAX);
	strlcpy(site->port, port, SITE_PORT_MAX);
	strlcpy(site->username, username, SITE_USER_MAX);
	strlcpy(site->password, password, SITE_PASS_MAX);
	site->use_tls = use_tls;
	site->is_secure = false;
	site->last_recv = NULL;
	site->prot_sent = false;
	site->busy = false;
	site->current_speed = 0.0f;
	site->ls_do_cache = false;
	site->enable_sscn = false;
	site->sscn_on = false;
	site->enforce_sscn_server_mode = false;
	site->is_connecting = false;
	site->xdupe_enabled = false;
	site->xdupe_table = dict_create();
	site->xdupe_empty = true;
	site->cur_dirlist = NULL;

	site_set_cwd(site, "/");

	list_add_site(site);

	return site;
}

void site_busy(struct site_info *site) {
	site->busy = true;
}

void site_idle(struct site_info *site) {
	site->busy = false;
}

void site_set_cwd(struct site_info *site, char *cwd) {
	strlcpy(site->current_working_dir, cwd, MAX_PATH_LEN);
}

struct linked_list *site_get_all() {
	struct linked_list *r = linked_list_create();
	struct site_info *sp;

	linked_list_rewind(all_sites);

	while((sp = linked_list_next(all_sites)) != NULL) {
		linked_list_add(r, sp);
	}

	return r;
}

struct linked_list *site_get_sites_connecting() {
	struct linked_list *r = linked_list_create();
	struct site_info *sp;

	linked_list_rewind(all_sites);

	while((sp = linked_list_next(all_sites)) != NULL) {
		if(sp->is_connecting) {
			linked_list_add(r, sp);
		}
	}

	return r;
}

void site_xdupe_add(struct site_info *site, const char *file) {
	if(file == NULL || strlen(file) == 0) {
		return;
	}

	if(!dict_has_key(site->xdupe_table, file)) {
		dict_set(site->xdupe_table, file, NULL);
		site->xdupe_empty = false;
	}
}

void site_xdupe_clear(struct site_info *site) {
	dict_clear(site->xdupe_table, false);
	site->xdupe_empty = true;
}

bool site_xdupe_has(struct site_info *site, const char *file) {
	return dict_has_key(site->xdupe_table, file);
}

struct site_pair *site_get_current_pair() {
	if(current_pair == NULL) {
		current_pair = malloc(sizeof(struct site_pair));
		current_pair->left = NULL;
		current_pair->right = NULL;
		current_pair->id = site_gen_id();
	}

	return current_pair;
}

struct site_pair *site_create_pair(struct site_info *site_l, struct site_info *site_r) {
	struct site_pair *pair = malloc(sizeof(struct site_pair));

	pair->left = site_l;
	pair->right = site_r;

	list_add_pair(pair);

	return pair;
}

struct linked_list *site_get_all_pairs() {
	struct linked_list *r = linked_list_create();
	struct site_info *sp;

	linked_list_rewind(all_pairs);

	while((sp = linked_list_next(all_pairs)) != NULL) {
		linked_list_add(r, sp);
	}

	return r;
}

void site_destroy_all() {
	linked_list_rewind(all_pairs);
	linked_list_rewind(all_sites);

	struct site_info *si;
	struct site_pair *sp;

	while((si = linked_list_next(all_sites)) != NULL) {
		free(si);
	}

	while((sp = linked_list_next(all_pairs)) != NULL) {
		free(sp);
	}

	linked_list_destroy(all_pairs);
	linked_list_destroy(all_sites);

	all_pairs = NULL;
	all_sites = NULL;
}

uint32_t site_gen_id() {
	static uint32_t id = 0;
	return id++;
}
