#pragma once

#include "general.h"
#include "str.h"

char *path_expand_home(const char *in);
char *path_expand_full_remote(const char *in, const char *cwd);
char *path_expand_full_local(const char *in);
char *path_get_dir_path(char *s);
char *path_get_filename(char *s);
char *path_append_file(const char *path, const char *file);
char *path_append_dir(const char *path, const char *dir);
