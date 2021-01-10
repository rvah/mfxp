#include "test_str.h"
#include "str.h"

void test_str_trim() {
	//rtrim
	char *sre = strdup("");
	char *srs = strdup(" ");
	char *sr = strdup("test");
	char *sr1 = strdup("test1 ");
	char *sr2 = strdup("test2 	");
	char *sr3 = strdup("test3   ");

	//ltrim
	char *sle = strdup("");
	char *sls = strdup(" ");
	char *sl = strdup("test");
	char *sl1 = strdup(" test1");
	char *sl2 = strdup("	 test2");
	char *sl3 = strdup("   test3");

	//trim
	char *se = strdup("");
	char *ss = strdup(" ");
	char *s = strdup("test");
	char *s1 = strdup(" test1 ");
	char *s2 = strdup("	 test2 	");
	char *s3 = strdup("   test3   ");

	//rtrim special only
	char *rs1 = strdup("test1");
	char *rs2 = strdup("test2 ");
	char *rs3 = strdup("test3 	");
	char *rs4 = strdup("test4\n");

	//rtrim slash
	char *slash1 = strdup("test1");
	char *slash2 = strdup("test2/");
	char *slash3 = strdup("test3/ ");

	str_rtrim(sre);
	str_rtrim(srs);
	str_rtrim(sr);
	str_rtrim(sr1);
	str_rtrim(sr2);
	str_rtrim(sr3);

	str_ltrim(sle);
	str_ltrim(sls);
	str_ltrim(sl);
	str_ltrim(sl1);
	str_ltrim(sl2);
	str_ltrim(sl3);

	str_trim(se);
	str_trim(ss);
	str_trim(s);
	str_trim(s1);
	str_trim(s2);
	str_trim(s3);

	str_rtrim_special_only(rs1);
	str_rtrim_special_only(rs2);
	str_rtrim_special_only(rs3);
	str_rtrim_special_only(rs4);

	str_rtrim_slash(slash1);
	str_rtrim_slash(slash2);
	str_rtrim_slash(slash3);

	TEST_ASSERT_TRUE(strcmp(sre, "") == 0);
	TEST_ASSERT_TRUE(strcmp(srs, "") == 0);
	TEST_ASSERT_TRUE(strcmp(sr, "test") == 0);
	TEST_ASSERT_TRUE(strcmp(sr1, "test1") == 0);
	TEST_ASSERT_TRUE(strcmp(sr2, "test2") == 0);
	TEST_ASSERT_TRUE(strcmp(sr3, "test3") == 0);

	TEST_ASSERT_TRUE(strcmp(sle, "") == 0);
	TEST_ASSERT_TRUE(strcmp(sls, "") == 0);
	TEST_ASSERT_TRUE(strcmp(sl, "test") == 0);
	TEST_ASSERT_TRUE(strcmp(sl1, "test1") == 0);
	TEST_ASSERT_TRUE(strcmp(sl2, "test2") == 0);
	TEST_ASSERT_TRUE(strcmp(sl3, "test3") == 0);

	TEST_ASSERT_TRUE(strcmp(se, "") == 0);
	TEST_ASSERT_TRUE(strcmp(ss, "") == 0);
	TEST_ASSERT_TRUE(strcmp(s, "test") == 0);
	TEST_ASSERT_TRUE(strcmp(s1, "test1") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "test2") == 0);
	TEST_ASSERT_TRUE(strcmp(s3, "test3") == 0);

	TEST_ASSERT_TRUE(strcmp(rs1, "test1") == 0);
	TEST_ASSERT_TRUE(strcmp(rs2, "test2 ") == 0);
	TEST_ASSERT_TRUE(strcmp(rs3, "test3 ") == 0);
	TEST_ASSERT_TRUE(strcmp(rs4, "test4") == 0);

	TEST_ASSERT_TRUE(strcmp(slash1, "test1") == 0);
	TEST_ASSERT_TRUE(strcmp(slash2, "test2") == 0);
	TEST_ASSERT_TRUE(strcmp(slash3, "test3/") == 0);

	free(sre);
	free(srs);
	free(sr);
	free(sr1);
	free(sr2);
	free(sr3);
	free(sle);
	free(sls);
	free(sl);
	free(sl1);
	free(sl2);
	free(sl3);
	free(se);
	free(ss);
	free(s);
	free(s1);
	free(s2);
	free(s3);
	free(rs1);
	free(rs2);
	free(rs3);
	free(rs4);
	free(slash1);
	free(slash2);
	free(slash3);
}

void test_str_tolower() {
	char *s1 = strdup("test");
	char *s2 = strdup("tESt");
	char *s3 = strdup("TEST123 ");

	str_tolower(s1);
	str_tolower(s2);
	str_tolower(s3);

	TEST_ASSERT_TRUE(strcmp(s1, "test") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "test") == 0);
	TEST_ASSERT_TRUE(strcmp(s3, "test123 ") == 0);

	free(s1);
	free(s2);
	free(s3);
}

void test_str_concat() {
	char *s1 = str_concat("", "end");
	char *s2 = str_concat("start", "");
	char *s3 = str_concat("", "");
	char *s4 = str_concat(NULL, NULL);
	char *s5 = str_concat(NULL, "end");
	char *s6 = str_concat("", NULL);
	char *s7 = str_concat("start", "end");

	TEST_ASSERT_TRUE(strcmp(s1, "end") == 0);
	TEST_ASSERT_TRUE(strcmp(s2, "start") == 0);
	TEST_ASSERT_TRUE(strcmp(s3, "") == 0);
	TEST_ASSERT_NULL(s4);
	TEST_ASSERT_NULL(s5);
	TEST_ASSERT_NULL(s6);
	TEST_ASSERT_TRUE(strcmp(s7, "startend") == 0);

	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(s5);
	free(s6);
	free(s7);
}

void test_run_str() {
	RUN_TEST(test_str_trim);
	RUN_TEST(test_str_tolower);
	RUN_TEST(test_str_concat);
}
