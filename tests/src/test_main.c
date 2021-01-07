#include <stdio.h>
#include "unity.h"
#include "unity_internals.h"
#include "test_dictionary.h"
#include "test_filesystem.h"
#include "test_linked_list.h"
#include "test_site.h"
#include "test_path.h"
#include "test_str.h"

void setUp() {
}

void tearDown() {
}

int main() {
	UNITY_BEGIN();
	test_run_dictionary();
	test_run_filesystem();
	test_run_linked_list();
	test_run_site();
	test_run_path();
	test_run_str();
	return UNITY_END();
}
