#include "parse.h"

//227 Entering Passive Mode (127,0,0,1,219,3)
bool parse_pasv(const char *in, char *out_addr, uint32_t *out_port) {
	if((in == NULL) || (strlen(in) == 0)) {
		return false;
	}

	//find start
	bool found_start = false;

	while(*in != '\0') {
		if(*in == '(') {
			found_start = true;
			in++;
			break;
		}
		in++;
	}
	if(!found_start) {
		return false;
	}

	//split into array
	uint32_t pasv_a[6];
	char *part, *save;

	for(int i = 0; i < 6; i++) {
		if(i == 0) {
			part = strtok_r(strdup(in), ",", &save);
		} else {
			part = strtok_r(NULL, ",", &save);
		}

		if(part == NULL) {
			return false;
		}

		pasv_a[i] = atoi(part);
	}

	sprintf(out_addr, "%d.%d.%d.%d", pasv_a[0], pasv_a[1], pasv_a[2], pasv_a[3]);
	*out_port = (pasv_a[4]*256)+pasv_a[5];

	return true;
}

char *parse_pwd(const char *s) {
	if(s == NULL) {
		return NULL;
	}

	//find start
	bool found_start = false;
	bool found_end = false;

	while(*s != '\0') {
		if(*s == '"') {
			found_start = true;
			break;
		}
		s++;
	}

	if(!found_start) {
		return NULL;
	}

	const char *start_p = s+1;
	int len = 0;

	//find end
	s++;
	while(*s != '\0') {
		if( (*s == '"') && (*(s-1) != '"') && (*(s-1) != '\\')) {
			found_end = true;
			break;
		}		
		s++;
		len++;
	}

	if(!found_end) {
		return NULL;
	}

	char *path = malloc(len+1);
	strlcpy(path, start_p, len+1);

	return path;
}
