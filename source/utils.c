#include "utils.h"

void milliSleep(int milliseconds) {
	nanosleep(&(struct timespec){0, milliseconds*1000*1000}, NULL);
}

int calculateDistance(int point1, int point2) {
	return (point1 - point2) * (point1 - point2);
}