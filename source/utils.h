/**
* @file
* @brief Miscellanious functions.
*/

#pragma once
#include <time.h>

/**
* @brief Convenience functions to sleep for a number of miliseconds.
*
* @param[in] miliseconds integer set to numer of milliseconds to wait for.
*/
void milliSleep(int milliseconds);

/**
* @brief Gives a measure of distance between two points, used only for comparison with distances from this function.
*
* @param[in] point1 First point to measure distance from.
* @param[in] point2 Second point to measure distance to.
*
* @return Square of numerical difference between points
*/
int calculateDistance(int point1, int point2);