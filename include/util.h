#pragma once
#include "general.h"
#include <ctype.h>
#include <libgen.h>
#include "transfer_result.h"
#include "filesystem.h"
#include "dictionary.h"

bool match_rule(const char *rule, const char *str);
bool file_exists (char *filename);
