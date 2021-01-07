#include "test_site.h"
#include "linked_list.h"
#include "site.h"

void test_site_create() {
	struct site_info *site = site_init("TestSite", "google.com", "21",
			"myuser", "mypass", true);

	TEST_ASSERT_TRUE(strcmp(site->name, "TestSite") == 0);
	TEST_ASSERT_TRUE(strcmp(site->address, "google.com") == 0);
	TEST_ASSERT_TRUE(strcmp(site->port, "21") == 0);
	TEST_ASSERT_TRUE(strcmp(site->username, "myuser") == 0);
	TEST_ASSERT_TRUE(strcmp(site->password, "mypass") == 0);
	TEST_ASSERT_TRUE(site->use_tls);

	site_destroy_all();
}

void test_site_set_busy_and_idle() {
	struct site_info *site = site_init("TestSite", "google.com", "21",
			"myuser", "mypass", true);

	site_busy(site);
	TEST_ASSERT_TRUE(site->busy);

	site_idle(site);
	TEST_ASSERT_FALSE(site->busy);

	site_destroy_all();
}

void test_site_set_cwd() {
	struct site_info *site = site_init("TestSite", "google.com", "21",
			"myuser", "mypass", true);

	site_set_cwd(site, "/some/path");
	TEST_ASSERT_TRUE(strcmp(site->current_working_dir, "/some/path") == 0);
	site_destroy_all();
}

void test_site_get_all() {
	site_destroy_all();
	struct linked_list *empty_list = site_get_all();

	site_init("TestSite1", "google.com", "21", "myuser", "mypass", true);
	site_init("TestSite2", "google.com", "21", "myuser", "mypass", true);

	struct linked_list *all = site_get_all();

	TEST_ASSERT_TRUE(linked_list_count(empty_list) == 0);
	TEST_ASSERT_TRUE(linked_list_count(all) == 2);
	TEST_ASSERT_TRUE(strcmp(((struct site_info *)linked_list_next(all))->name,
				"TestSite1") == 0);
	TEST_ASSERT_TRUE(strcmp(((struct site_info *)linked_list_next(all))->name,
				"TestSite2") == 0);
	TEST_ASSERT_NULL(linked_list_next(all));

	site_destroy_all();
	linked_list_destroy(empty_list);
	linked_list_destroy(all);
}

void test_site_get_sites_connecting() {
	site_destroy_all();
	struct linked_list *empty_list = site_get_sites_connecting();

	site_init("TestSite1", "google.com", "21", "myuser", "mypass", true);
	struct site_info *s2 = site_init("TestSite2", "google.com", "21", "myuser",
			"mypass", true);
	struct site_info *s3 = site_init("TestSite3", "google.com", "21", "myuser",
			"mypass", true);

	s2->is_connecting = true;
	s3->is_connecting = true;

	struct linked_list *all = site_get_sites_connecting();

	TEST_ASSERT_TRUE(linked_list_count(empty_list) == 0);
	TEST_ASSERT_TRUE(linked_list_count(all) == 2);

	site_destroy_all();
	linked_list_destroy(empty_list);
	linked_list_destroy(all);
}

void test_site_xdupe() {
	struct site_info *site = site_init("TestSite", "google.com", "21",
			"myuser", "mypass", true);

	TEST_ASSERT_FALSE(site_xdupe_has(site, NULL));
	TEST_ASSERT_FALSE(site_xdupe_has(site, ""));
	TEST_ASSERT_FALSE(site_xdupe_has(site, "something"));

	site_xdupe_add(site, "somefile");
	site_xdupe_add(site, "otherfile");
	site_xdupe_add(site, "");

	TEST_ASSERT_TRUE(site_xdupe_has(site, "somefile"));
	TEST_ASSERT_TRUE(site_xdupe_has(site, "otherfile"));
	TEST_ASSERT_FALSE(site_xdupe_has(site, "badfile"));

	TEST_ASSERT_FALSE(site_xdupe_has(site, ""));

	site_xdupe_clear(site);

	TEST_ASSERT_FALSE(site_xdupe_has(site, "somefile"));
	TEST_ASSERT_FALSE(site_xdupe_has(site, "otherfile"));

	site_destroy_all();
}

void test_site_get_current_pairs() {
	site_destroy_all();

	struct site_info *l = site_init("TestSite1", "google.com", "21", "myuser",
			"mypass", true);
	struct site_info *r = site_init("TestSite2", "google.com", "21", "myuser",
			"mypass", true);

	site_create_pair(l, r);

	struct site_pair *current = site_get_current_pair();

	TEST_ASSERT(strcmp(current->left->name, "TestSite1") == 0);
	TEST_ASSERT(strcmp(current->right->name, "TestSite2") == 0);

	site_destroy_all();
}

void test_site_get_all_pairs() {
	site_destroy_all();

	struct site_info *l1 = site_init("TestSite1", "google.com", "21", "myuser",
			"mypass", true);
	struct site_info *r1 = site_init("TestSite2", "google.com", "21", "myuser",
			"mypass", true);
	struct site_info *l2 = site_init("TestSite3", "google.com", "21", "myuser",
			"mypass", true);
	struct site_info *r2 = site_init("TestSite4", "google.com", "21", "myuser",
			"mypass", true);

	site_create_pair(l1, r1);
	site_create_pair(l2, r2);

	struct linked_list *pairs = site_get_all_pairs();

	struct site_pair *p1 = linked_list_next(pairs);
	struct site_pair *p2 = linked_list_next(pairs);
	struct site_pair *p3 = linked_list_next(pairs);

	TEST_ASSERT_TRUE(strcmp(p1->left->name, "TestSite1") == 0);
	TEST_ASSERT_TRUE(strcmp(p1->right->name, "TestSite2") == 0);
	TEST_ASSERT_TRUE(strcmp(p2->left->name, "TestSite3") == 0);
	TEST_ASSERT_TRUE(strcmp(p2->right->name, "TestSite4") == 0);

	TEST_ASSERT_NULL(p3);

}

void test_site_gen_id() {
	for(int i = 0, old_id = 0, new_id = site_gen_id(); i < 10; i++, old_id = new_id,
			new_id = site_gen_id()) {
		if(i == 0) {
			continue;
		}
		TEST_ASSERT_TRUE(old_id < new_id);
	}
}

void test_run_site() {
	RUN_TEST(test_site_create);
	RUN_TEST(test_site_set_busy_and_idle);
	RUN_TEST(test_site_set_cwd);
	RUN_TEST(test_site_get_all);
	RUN_TEST(test_site_get_sites_connecting);
	RUN_TEST(test_site_xdupe);
	RUN_TEST(test_site_get_current_pairs);
	RUN_TEST(test_site_gen_id);
	RUN_TEST(test_site_get_all_pairs);
}
