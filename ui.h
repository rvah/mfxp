#pragma once
#include "general.h"
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"


#define UI_INPUT_MAX 2048

void ui_init();
void ui_loop();
void ui_close();