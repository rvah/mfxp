#include "test_path.h"
#include "path.h"

/*
char *path_expand_home(const char *in);
char *path_expand_full_remote(const char *in, const char *cwd);
char *path_expand_full_local(const char *in);
char *path_get_dir_path(char *s);
char *path_get_filename(char *s);
char *path_append_file(const char *path, const char *file);
char *path_append_dir(const char *path, const char *dir);
*/

void test_path_expand_home() {
	struct passwd *pw = getpwuid(getuid());
	char *home_dir = pw->pw_dir;
	str_rtrim_slash(home_dir);

	char *s1 = path_expand_home("file1");
	char *s2 = path_expand_home("");
	char *s3 = path_expand_home(NULL);
	char *s4 = path_expand_home("/some/file");
	char *s5 = path_expand_home("~");
	char *s6 = path_expand_home("~/");
	char *s7 = path_expand_home("~/random/");
	char *s8 = path_expand_home("/");
	char *s9 = path_expand_home(".");

	char *c6 = str_concat(home_dir, "/");
	char *c7 = str_concat(home_dir, "/random/");

	TEST_ASSERT_TRUE(strcmp(s1, "file1") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s3, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s4, "/some/file") == 0);
	TEST_ASSERT_TRUE(strcmp(s5, home_dir) == 0);
	TEST_ASSERT_TRUE(strcmp(s6, c6) == 0);
	TEST_ASSERT_TRUE(strcmp(s7, c7) == 0);
	TEST_ASSERT_TRUE(strcmp(s8, "/") == 0);
	TEST_ASSERT_TRUE(strcmp(s9, ".") == 0);

	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(s5);
	free(s6);
	free(s7);
	free(s8);
	free(s9);
	free(c6);
	free(c7);
}

void test_path_expand_full_remote() {
	char *s1 = path_expand_full_remote(NULL, "/");
	char *s2 = path_expand_full_remote("/", NULL);
	char *s3 = path_expand_full_remote(NULL, NULL);
	char *s4 = path_expand_full_remote("/f", "/d");
	char *s5 = path_expand_full_remote("./f", "/d");
	char *s6 = path_expand_full_remote("./d/d/", "/d/d/");
	char *s7 = path_expand_full_remote("./f", "/");
	char *s8 = path_expand_full_remote("f", "");
	char *s9 = path_expand_full_remote("", "d");
	char *s10 = path_expand_full_remote("f", "/d");
	char *s11 = path_expand_full_remote("d/", "/d/");
	
	TEST_ASSERT_TRUE(strcmp(s1, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s3, "") == 0);

	TEST_ASSERT_TRUE(strcmp(s4, "/f") == 0);
	TEST_ASSERT_TRUE(strcmp(s5, "/d/f") == 0);
	TEST_ASSERT_TRUE(strcmp(s6, "/d/d/d/d/") == 0);
	TEST_ASSERT_TRUE(strcmp(s7, "/f") == 0);
	TEST_ASSERT_TRUE(strcmp(s8, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s9, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s10, "/d/f") == 0);
	TEST_ASSERT_TRUE(strcmp(s11, "/d/d/") == 0);

	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(s5);
	free(s6);
	free(s7);
	free(s8);
	free(s9);
	free(s10);
	free(s11);
}

void test_path_expand_full_local() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	struct passwd *pw = getpwuid(getuid());
	char *home_dir = pw->pw_dir;
	str_rtrim_slash(home_dir);

	chdir(home_dir);

	char *s1 = path_expand_full_local("");
	char *s2 = path_expand_full_local(NULL);
	char *s3 = path_expand_full_local("~");
	char *s4 = path_expand_full_local("/");
	char *s5 = path_expand_full_local("./f");
	char *s6 = path_expand_full_local("./");

	char *c5 = str_concat(home_dir, "/f");
	char *c6 = str_concat(home_dir, "/");

	TEST_ASSERT_TRUE(strcmp(s1, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "") == 0);

	TEST_ASSERT_TRUE(strcmp(s3, home_dir) == 0);
	TEST_ASSERT_TRUE(strcmp(s4, "/") == 0);
	TEST_ASSERT_TRUE(strcmp(s5, c5) == 0);
	TEST_ASSERT_TRUE(strcmp(s6, c6) == 0);

	chdir(cwd);

	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(s5);
	free(s6);
	free(c5);
	free(c6);

}

void test_path_parse_dir_and_filename() {
/*
char *path_get_dir_path(char *s);
char *path_get_filename(char *s);
*/
	char *p1 = path_get_dir_path("/");
	char *p2 = path_get_dir_path("/d/f");

	char *f1 = path_get_filename("/");
	char *f2 = path_get_filename("/d/f");

	char *n1 = path_get_filename(NULL);
	char *n2 = path_get_dir_path(NULL);

	char *e1 = path_get_filename("");
	char *e2 = path_get_dir_path("");

	TEST_ASSERT_TRUE(strcmp(p1, "/") == 0);
	TEST_ASSERT_TRUE(strcmp(p2, "/d/") == 0);

	TEST_ASSERT_TRUE(strcmp(f1, "") == 0);
	TEST_ASSERT_TRUE(strcmp(f2, "f") == 0);

	TEST_ASSERT_TRUE(strcmp(n1, "") == 0);
	TEST_ASSERT_TRUE(strcmp(n2, "") == 0);
	TEST_ASSERT_TRUE(strcmp(e1, "") == 0);
	TEST_ASSERT_TRUE(strcmp(e2, "") == 0);

	free(p1);
	free(p2);
	free(f1);
	free(f2);
	free(e1);
	free(e2);
	free(n1);
	free(n2);
}

void test_path_append_file() {
}

void test_path_append_dir() {
}

void test_run_path() {
	RUN_TEST(test_path_expand_home);
	RUN_TEST(test_path_expand_full_remote);
	RUN_TEST(test_path_expand_full_local);
	RUN_TEST(test_path_parse_dir_and_filename);
	RUN_TEST(test_path_append_file);
	RUN_TEST(test_path_append_dir);
}
