#include "util.h"

bool __match_rule(const char *rule, const char *str, int ri, int si) {
	if(rule == NULL) {
		return false;
	}

	if(rule[ri]  == '\0') {
		return str[si] == '\0';
	}

	if(rule[ri] == '*') {
		while(str[si] != '\0') {
			if(__match_rule(rule, str, ri+1, si)) {
				return true;
			}
			si++;
		}

		return __match_rule(rule, str, ri+1, si);
	}

	if((rule[ri] != str[si]) && (rule[ri] != '?')) {
		return false;
	}

	return __match_rule(rule, str, ri+1, si+1);
}

bool match_rule(const char * rule, const char *str) {
	return __match_rule(rule, str, 0, 0);
}

bool file_exists(char *filename) {
	struct stat buffer;   
	return (stat(filename, &buffer) == 0);
}


