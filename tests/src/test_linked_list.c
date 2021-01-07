#include "test_linked_list.h"
#include "linked_list.h"

void test_list_add() {
	struct linked_list *list = linked_list_create();
	int one = 11;
	int two = 22;

	linked_list_add(list, &one);
	linked_list_add(list, &two);

	TEST_ASSERT_TRUE(*(int *)linked_list_next(list) == 11);
	TEST_ASSERT_TRUE(*(int *)linked_list_next(list) == 22);
	TEST_ASSERT_TRUE(linked_list_next(list) == NULL);

	linked_list_destroy(list);
}

void test_list_rewind() {
	struct linked_list *list = linked_list_create();
	int one = 11;
	int two = 22;

	linked_list_add(list, &one);
	linked_list_add(list, &two);

	TEST_ASSERT_TRUE(*(int *)linked_list_next(list) == 11);
	linked_list_rewind(list);
	TEST_ASSERT_TRUE(*(int *)linked_list_next(list) == 11);

	linked_list_destroy(list);
}

void test_list_count() {
	struct linked_list *list = linked_list_create();
	int one = 11;
	int two = 22;

	TEST_ASSERT_TRUE(linked_list_count(list) == 0);
	linked_list_add(list, &one);
	TEST_ASSERT_TRUE(linked_list_count(list) == 1);
	linked_list_add(list, &two);
	TEST_ASSERT_TRUE(linked_list_count(list) == 2);

	linked_list_destroy(list);
}

void test_run_linked_list() {
	RUN_TEST(test_list_add);
	RUN_TEST(test_list_rewind);
	RUN_TEST(test_list_count);
}
