#pragma once

#include <ctype.h>
#include <libgen.h>
#include "general.h"

void str_ltrim(char *s);
void str_rtrim(char *s);
void str_rtrim_special_only(char *s);
void str_trim(char *s);
void str_rtrim_slash(char *s);
void str_tolower(char *s);
