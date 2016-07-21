#include <sys/time.h>
#include <stdint.h>


#define CMD_PING 1
#define CMD_GETTIME 2

typedef struct {
	int32_t version;
	int32_t cmd;
	int32_t id;
	int32_t sec;
	int32_t usec;
	int32_t checksum;
} timecmd_t;


void timecmd_print(timecmd_t *cmd);

int deltausec(struct timeval end, struct timeval start); 
