#include "path.h"



char *path_expand_home(const char *in) {
	char *d = strdup(in);
	str_trim(d);

	if(d[0] == '~') {
		wordexp_t exp_result;
		wordexp(d, &exp_result, 0);

		free(d);
		d = strdup(exp_result.we_wordv[0]);
	}

	return d;
}

char *path_expand_full_remote(const char *in, const char *cwd) {
	char *d = strdup(in);
	str_trim(d);

	char *p = d;

	if( (p[0] == '.') && (p[1] == '/')) {
		p += 2;
	} else if(p[0] == '/') {
		return d;
	}

	int n_len = strlen(p) + strlen(cwd) + 2;

	char *s_new = malloc(n_len);

	if(strcmp(cwd, "/") == 0) {
		snprintf(s_new, n_len, "/%s", p);
	} else {
		snprintf(s_new, n_len, "%s/%s", cwd, p);
	}

	free(d);
	return s_new;
}

char *path_expand_full_local(const char *in) {
	char *d = path_expand_home(in);

	if(d == NULL) {
		return d;
	}

	char *dir = dirname(strdup(d));
	char *file = basename(strdup(d));

	free(d);

	char *real = realpath(dir, NULL);

	if(real == NULL) {
		return NULL;
	}

	int flen = strlen(real)+strlen(file)+2;
	char *ret = malloc(flen);

	//prevent /// and // paths

	if(strcmp(real, "/") == 0) {
		if(strcmp(file, "/") == 0) {
			snprintf(ret, flen, "%s", real);
		} else {
			snprintf(ret, flen, "%s%s", real, file);
		}
	} else {
		snprintf(ret, flen, "%s/%s", real, file);
	}

	return ret;
}

char *path_get_dir_path(char *s) {
	char *d = strdup(s);
	char *r = strdup(dirname(d));
	free(d);

	size_t len = strlen(r);

	if(r[len-1] != '/') {
		char *nr = malloc(len+2);
		snprintf(nr, len+2, "%s/", r);
		free(r);
		r = nr;
	}

	//if unix shell ~ path, expand it
	if(r[0] == '~') {
		wordexp_t exp_result;
		wordexp(r, &exp_result, 0);

		free(r);
		r = strdup(exp_result.we_wordv[0]);
	}

	return r;
}

char *path_get_filename(char *s) {
	char *d = strdup(s);
	char *r = strdup(basename(d));
	free(d);

	return r;
}

char *path_append_file(const char *path, const char *file) {
	int p_len = strlen(path);
	int f_len = strlen(file);
	int new_len = p_len + f_len + 2;
	char *ret = malloc(new_len);
	strlcpy(ret, path, new_len);

	if(path[p_len-1] != '/') {
		strlcat(ret, "/", new_len);
	}

	strlcat(ret, file, new_len);
	return ret;
}

char *path_append_dir(const char *path, const char *dir) {
	char *t = path_append_file(path, dir);
	int t_len = strlen(t);

	if(t[t_len-1] != '/') {
		int new_len = t_len+2;
		char *r = malloc(new_len);
		strlcpy(r, t, new_len);
		strlcat(r, "/", new_len);
		free(t);
		return r;
	}
	return t; 
}
