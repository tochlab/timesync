#include <stdio.h>

#include "tslib.h"

void timecmd_print(timecmd_t *cmd) {
	printf("ver %d id %d cmd %d sec %d usec %d\n", cmd->version, cmd->id, cmd->cmd, cmd->sec, cmd->usec);
}

int deltausec(struct timeval from_time, struct timeval to_time) {
	int delta_sec = to_time.tv_sec - from_time.tv_sec;
	int delta_usec = to_time.tv_usec - from_time.tv_usec;
	int result = (delta_sec * 1000000) + delta_usec;
	return result;
}
