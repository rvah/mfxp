#pragma once

#include "general.h"
#include "util.h"
#include "skiplist.h"
#include "priolist.h"
#include "sort.h"
#include "hilight.h"

#define MAX_PATH_LEN 4096
#define MAX_FILENAME_LEN 1024

#define FILE_TYPE_FILE 0
#define FILE_TYPE_DIR 1
#define FILE_TYPE_LINK 2

#define FILE_PERM_LEN 10
#define FILE_USER_LEN 255
#define FILE_GROUP_LEN 255
#define FILE_DATE_LEN 13

struct file_item {
	struct file_item *next;
	uint32_t file_type;
	char permissions[FILE_PERM_LEN];
	char owner_user[FILE_USER_LEN];
	char owner_group[FILE_GROUP_LEN];
	uint64_t size;
	char date[FILE_DATE_LEN];
	char file_name[MAX_FILENAME_LEN];
	bool skip;
	uint32_t priority;
	bool hilight;
};

void file_item_destroy(struct file_item * item);
struct file_item *file_item_cpy(struct file_item * item);
struct file_item *find_local_file(const char *path, const char *filename);
struct file_item *local_ls(char *path, bool prio_sort);
struct file_item *find_file(struct file_item *list, const char *filename);
void print_file_item(struct file_item *item);
struct file_item *parse_list(char *text_list);
struct file_item *parse_line(char *line);
