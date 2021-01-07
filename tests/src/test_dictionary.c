#include "test_dictionary.h"

void test_dict_add_item() {
	struct dict_node **root = dict_create();
	int to_save = 55;

	dict_set(root, "testkey", &to_save);
	
	TEST_ASSERT_TRUE(*((int *)dict_get(root, "testkey")) == 55);
}

void test_dict_has_item() {
	struct dict_node **root = dict_create();
	int to_save = 66;

	dict_set(root, "somekey", &to_save);

	TEST_ASSERT_TRUE(dict_has_key(root, "somekey"));
	TEST_ASSERT_FALSE(dict_has_key(root, "badkey"));
}

void test_dict_clear() {
	struct dict_node **root = dict_create();
	int to_save = 77;

	dict_set(root, "otherkey", &to_save);
	/* can only test "false", since its not possible in C to reliably
	 * verify if data has been free()'d
	 */
	dict_clear(root, false);

	TEST_ASSERT_FALSE(dict_has_key(root, "otherkey"));
}

void test_run_dictionary() {
	RUN_TEST(test_dict_add_item);
	RUN_TEST(test_dict_has_item);
	RUN_TEST(test_dict_clear);
}
