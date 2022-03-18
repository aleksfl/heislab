/**
* @file
* @brief Module dealing with collecting and storing of requests.
*/

#pragma once
#include "driver/elevio.h"
#include "utils.h"

/** Global variable int matrix used to keep track of requests made by buttons.
 * Rows represent floors and columns represent types of buttons.
 * Values are set to either 1 or 0, where 1 represents an unprocessed request from a specific button.
  */
extern int RequestMatrix[N_FLOORS][N_BUTTONS];

/**
* @brief Iterates through floor and cab request buttons to store request and turn on button light if they are pressed.
*
* \param[out] RequestMatrix is updated based on pressed buttons.
*/
void CheckButtons(void);


/**
* @brief Same as <CheckButtons>"()", but skips checking buttons requesting the current floor.
*
* @param[in] currFloor Current floor to ignore requests to.
*
* \param[out] RequestMatrix is updated based on pressed buttons.
*/
void CheckButtonsExcludeFloor(int currFloor);

/**
* @brief Sets #RequestMatrix to null matrix and turns off all button lights.
*
* \param[out] RequestMatrix is cleared (set to 0).
*/
void ClearQueue(void);

/**
* @brief Sets all values in a floor corresponding row to 0 in the #RequestMatrix
*
* @param[in] floor Floor to clear corresponding row in #RequestMatrix for.
*
* \param[out] RequestMatrix has a specific row cleared (set to 0).
*/
void RemoveFromQueue(int floor);