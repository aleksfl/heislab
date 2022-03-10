/**
* @file
* @brief Module dealing with the operation of the elevator doors.
*/

#pragma once

//Inkluderer queue.h for å få tilgang til makroer
#include "queue.h"                   
#include "driver/elevio.h"

/** Global variable to keep track of whether door is open or not.  */
extern int DoorState;


/**
* @brief Opens elevator door and turns on door lamp if recquired conditions are met.
*
* \param[out] DoorState is updated with updated state of door.
*/
void TryOpenDoor(void);

/**
* @brief Closes elevator door and turns off door lamp if recquired conditions are met.
*
* \param[out] DoorState is updated with updated state of door.
*/
void TryCloseDoor(void);

