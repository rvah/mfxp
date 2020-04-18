#pragma once
#include <pthread.h>
#include "general.h"
#include "site.h"
#include "conn.h"
#include "msg.h"

void *thread_site(void *ptr);
