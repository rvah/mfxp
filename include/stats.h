#pragma once
#include <sys/time.h>
#include "general.h"
#include <ctype.h>
#include "transfer_result.h"
#include "filesystem.h"
#include "parse.h"

#define STATS_UPDATE_FREQ_USEC 100000 /* 0.1s */

enum stats_transfer_status { sts_PROGRESS, sts_DONE };

struct stats_transfer {
	enum stats_transfer_status status;
	struct timeval t_start;
	struct timeval t_end;
	double tot_avg_speed;
	double cur_avg_speed;
	size_t tot_bytes_sent;
};

double calc_transfer_speed(struct timeval *start, struct timeval *end, uint64_t size);
char *s_get_speed(double speed);
char *s_calc_transfer_speed(struct timeval *start, struct timeval *end, uint64_t size);
char *s_gen_stats(struct transfer_result *tr, uint64_t seconds);
struct stats_transfer *stats_transfer_create();
void stats_transfer_update(struct stats_transfer *s, size_t bytes_sent);
double stats_transfer_duration(struct stats_transfer *s);
void stats_transfer_destroy(struct stats_transfer *stats);
