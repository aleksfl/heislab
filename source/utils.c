#include "utils.h"

void sleep(int seconds) {
	nanosleep(&(struct timespec){0, seconds*1000*1000*1000}, NULL);
}

void milliSleep(int milliseconds) {
	nanosleep(&(struct timespec){0, milliseconds*1000*1000}, NULL);
}