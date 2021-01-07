#include "test_filesystem.h"
#include "dictionary.h"
#include "filesystem.h"

static char *text_local = "-rwxrwxrwx 1 1001 1001 1390 Jan 11 2019 locfile.one\n"
		"drwxrwxrwx 1 1001 1001 4096 Dec 31 2018 locfile.two\n"
		"lrwxrwxrwx 1 1001 1001 57495 Jan 11 2019 locfile.three\n";

static char *text_local2 = "-rwxrwxrwx 1 1001 1001 1390 Jan 4 14:44 afile.one\n"
		"-rwxrwxrwx 1 1001 1001 1390 Jan 4 14:40 bfile.one\n"
		"drwxrwxrwx 1 1001 1001 4096 Dec 31 2018 cfile.two\n"
		"lrwxrwxrwx 1 1001 1001 57495 Jan 11 2019 dfile.three\n";

static char *text_glftpd = "total 81\n"
		"-rw-r--r--   1 user      NoGroup      5334 Dec 23 15:23 rem.one\n"
		"lrw-r--r--   1 user      NoGroup     14300 Jun 13 2001 rem.two\n"
		"drwxrwxrwx   3 user      NoGroup      4096 Feb 23 1998 rem.three\n";

bool check_file_parsing(struct file_item *f, uint32_t file_type,
		const char *user, const char *grp, uint64_t size, const char *date) {
	if(f->file_type != file_type) {
		return false;
	}

	if(strcmp(f->owner_user, user) != 0) {
		return false;
	}

	if(strcmp(f->owner_group, grp) != 0) {
		return false;
	}

	if(f->size != size) {
		return false;
	}

	if(strcmp(f->date, date) != 0) {
		return false;
	}

	return true;
}

void test_filesystem_convert_text_to_list() {
	struct file_item *local_files = filesystem_parse_list(text_local, LOCAL);
	struct file_item *gl_files = filesystem_parse_list(text_glftpd, GLFTPD);

	TEST_ASSERT_TRUE(local_files != NULL);
	TEST_ASSERT_TRUE(gl_files != NULL);

	struct dict_node **local_dict = dict_create();
	struct dict_node **gl_dict = dict_create();

	struct file_item *p = local_files;

	while(p != NULL) {
		dict_set(local_dict, p->file_name, p);
		p = p->next;
	}

	p = gl_files;

	while(p != NULL) {
		dict_set(gl_dict, p->file_name, p);
		p = p->next;
	}

	struct file_item *loc_file1 = dict_get(local_dict, "locfile.one");
	struct file_item *loc_file2 = dict_get(local_dict, "locfile.two");
	struct file_item *loc_file3 = dict_get(local_dict, "locfile.three");

	struct file_item *gl_file1 = dict_get(gl_dict, "rem.one");
	struct file_item *gl_file2 = dict_get(gl_dict, "rem.two");
	struct file_item *gl_file3 = dict_get(gl_dict, "rem.three");

	TEST_ASSERT_TRUE(loc_file1 != NULL && loc_file2 != NULL
			&& loc_file3 != NULL);
	TEST_ASSERT_TRUE(gl_file1 != NULL && gl_file2 != NULL && gl_file3 != NULL);

	//test parsing was correct
	TEST_ASSERT_TRUE(check_file_parsing(loc_file1, FILE_TYPE_FILE,
			"1001", "1001", 1390, "Jan 11 2019"));
	TEST_ASSERT_TRUE(check_file_parsing(loc_file2, FILE_TYPE_DIR,
			"1001", "1001", 4096, "Dec 31 2018"));
	TEST_ASSERT_TRUE(check_file_parsing(loc_file3, FILE_TYPE_LINK,
			"1001", "1001", 57495, "Jan 11 2019"));

	TEST_ASSERT_TRUE(check_file_parsing(gl_file1, FILE_TYPE_FILE,
			"user", "NoGroup", 5334, "Dec 23 15:23"));
	TEST_ASSERT_TRUE(check_file_parsing(gl_file2, FILE_TYPE_LINK,
			"user", "NoGroup", 14300, "Jun 13 2001"));
	TEST_ASSERT_TRUE(check_file_parsing(gl_file3, FILE_TYPE_DIR,
			"user", "NoGroup", 4096, "Feb 23 1998"));
}

void test_filesystem_find_file() {
	struct file_item *files = filesystem_parse_list(text_glftpd, GLFTPD);

	struct file_item *good = filesystem_find_file(files, "rem.two");
	struct file_item *bad = filesystem_find_file(files, "");
	struct file_item *bad2 = filesystem_find_file(files, "bad.txt");
	struct file_item *bad3 = filesystem_find_file(files, NULL);

	TEST_ASSERT_TRUE(good != NULL);
	TEST_ASSERT_TRUE(bad == NULL);
	TEST_ASSERT_TRUE(bad2 == NULL);
	TEST_ASSERT_TRUE(bad3 == NULL);

	filesystem_file_item_destroy(good);
	filesystem_file_item_destroy(bad);
	filesystem_file_item_destroy(bad2);
	filesystem_file_item_destroy(bad3);
}

void test_filesystem_file_item_copy() {
	struct file_item *files = filesystem_parse_list(text_glftpd, GLFTPD);

	struct file_item *copy = filesystem_file_item_cpy(files);

	TEST_ASSERT(files != NULL);
	TEST_ASSERT(files != copy);

	TEST_ASSERT(copy->file_type == files->file_type);
	TEST_ASSERT(strcmp(copy->permissions, files->permissions) == 0);
	TEST_ASSERT(strcmp(copy->owner_user, files->owner_user) == 0);
	TEST_ASSERT(strcmp(copy->owner_group, files->owner_group) == 0);
	TEST_ASSERT(copy->size == files->size);
	TEST_ASSERT(strcmp(copy->date, files->date) == 0);
	TEST_ASSERT(strcmp(copy->file_name, files->file_name) == 0);
	TEST_ASSERT(copy->skip == files->skip);
	TEST_ASSERT(copy->priority == files->priority);
	TEST_ASSERT(copy->hilight == files->hilight);

	filesystem_file_item_destroy(files);
	filesystem_file_item_destroy(copy);
}

void test_filesystem_filter_list() {
	struct file_item *files1 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files2 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files3 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files4 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files5 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files6 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct file_item *files7 = filesystem_parse_list(text_glftpd, GLFTPD);
	struct dict_node **d = dict_create();

	struct file_item *f1 = filesystem_filter_list(files1, "*.one");
	struct file_item *f2 = filesystem_filter_list(files2, "rem.?wo");
	struct file_item *f3 = filesystem_filter_list(files3, "rem.three");
	struct file_item *f4 = filesystem_filter_list(files4, "rem.*");

	struct file_item *b1 = filesystem_filter_list(files5, NULL);
	struct file_item *b2 = filesystem_filter_list(files6, "dontexist");
	struct file_item *b3 = filesystem_filter_list(files7, "");

	int n1, n2, n3, n4;
	n1 = n2 = n3 = n4 = 0;

	struct file_item *p;

	p = f1;

	while(p != NULL) {
		n1++;
		p = p->next;
	}

	p = f2;

	while(p != NULL) {
		n2++;
		p = p->next;
	}

	p = f3;

	while(p != NULL) {
		n3++;
		p = p->next;
	}

	p = f4;

	while(p != NULL) {
		n4++;
		dict_set(d, p->file_name, p);
		p = p->next;
	}

	TEST_ASSERT_TRUE(n1 == 1);
	TEST_ASSERT_TRUE(n2 == 1);
	TEST_ASSERT_TRUE(n3 == 1);
	TEST_ASSERT_TRUE(n4 == 3);

	TEST_ASSERT(strcmp(f1->file_name, "rem.one") == 0);
	TEST_ASSERT(strcmp(f2->file_name, "rem.two") == 0);
	TEST_ASSERT(strcmp(f3->file_name, "rem.three") == 0);

	TEST_ASSERT(dict_has_key(d, "rem.one") && dict_has_key(d, "rem.two") &&
		dict_has_key(d, "rem.three"));

	TEST_ASSERT_TRUE(b1 == NULL);
	TEST_ASSERT_TRUE(b2 == NULL);
	TEST_ASSERT_TRUE(b3 == NULL);

	dict_destroy(d, false);
	filesystem_file_item_destroy(f1);
	filesystem_file_item_destroy(f2);
	filesystem_file_item_destroy(f3);
	filesystem_file_item_destroy(f4);
}

void test_filesystem_sort() {
	struct file_item *cur, *prev;

	//test get sort
	filesystem_set_sort(SORT_TYPE_NAME_ASC);
	TEST_ASSERT_TRUE(filesystem_get_sort() == SORT_TYPE_NAME_ASC);
	filesystem_set_sort(SORT_TYPE_NAME_DESC);
	TEST_ASSERT_TRUE(filesystem_get_sort() == SORT_TYPE_NAME_DESC);

	//test sorting
	filesystem_set_sort(SORT_TYPE_NAME_ASC);
	struct file_item *sort_1 = filesystem_parse_list(text_local2, LOCAL);

	filesystem_set_sort(SORT_TYPE_NAME_DESC);
	struct file_item *sort_2 = filesystem_parse_list(text_local2, LOCAL);

	filesystem_set_sort(SORT_TYPE_TIME_ASC);
	struct file_item *sort_3 = filesystem_parse_list(text_local2, LOCAL);

	filesystem_set_sort(SORT_TYPE_TIME_DESC);
	struct file_item *sort_4 = filesystem_parse_list(text_local2, LOCAL);

	filesystem_set_sort(SORT_TYPE_SIZE_ASC);
	struct file_item *sort_5 = filesystem_parse_list(text_local2, LOCAL);

	filesystem_set_sort(SORT_TYPE_SIZE_DESC);
	struct file_item *sort_6 = filesystem_parse_list(text_local2, LOCAL);

	for(cur = sort_1, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		TEST_ASSERT_TRUE(prev->file_name[0] <= cur->file_name[0]);
	}

	for(cur = sort_2, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		TEST_ASSERT_TRUE(prev->file_name[0] >= cur->file_name[0]);
	}

	for(cur = sort_3, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		struct date_info *d_a = parse_date(prev->date);
		struct date_info *d_b = parse_date(cur->date);

		time_t t_a = date_to_unixtime(d_a);
		time_t t_b = date_to_unixtime(d_b);

		free(d_a);
		free(d_b);

		TEST_ASSERT_TRUE(t_a <= t_b);
	}

	for(cur = sort_4, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		struct date_info *d_a = parse_date(prev->date);
		struct date_info *d_b = parse_date(cur->date);

		time_t t_a = date_to_unixtime(d_a);
		time_t t_b = date_to_unixtime(d_b);

		free(d_a);
		free(d_b);

		TEST_ASSERT_TRUE(t_a >= t_b);
	}

	for(cur = sort_5, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		TEST_ASSERT_TRUE(prev->size <= cur->size);
	}

	for(cur = sort_6, prev = NULL; cur != NULL; prev = cur, cur = cur->next) {
		if(prev == NULL) {
			continue;
		}

		TEST_ASSERT_TRUE(prev->size >= cur->size);
	}

	filesystem_file_item_destroy(sort_1);
	filesystem_file_item_destroy(sort_2);
	filesystem_file_item_destroy(sort_3);
	filesystem_file_item_destroy(sort_4);
	filesystem_file_item_destroy(sort_5);
	filesystem_file_item_destroy(sort_6);
}

void test_run_filesystem() {
	RUN_TEST(test_filesystem_convert_text_to_list);
	RUN_TEST(test_filesystem_find_file);
	RUN_TEST(test_filesystem_file_item_copy);
	RUN_TEST(test_filesystem_filter_list);
	RUN_TEST(test_filesystem_sort);
}


/*
char *filesystem_local_list(const char *path);
*/
