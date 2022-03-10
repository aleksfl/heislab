#include "driver/elevio.h"

extern int DoorState;
extern int RequestMatrix[N_FLOORS][N_BUTTONS];

#define UNDEFINED_FLOOR -1
#define UNDEFINED_DISTANCE -1

typedef enum {
    INIT,            
    STANDBY,               
    UP,
    DOWN,
    STOP,
    OBSTRUCTED,
    WAIT                    
} ElevState;

/**
* @brief Opens elevator door and turns on door lamp if recquired conditions are met.
*/
void TryOpenDoor(void);

/**
* @brief Closes elevator door and turns off door lamp if recquired conditions are met.
*/
void TryCloseDoor(void);

/**
* @brief Iterates through floor and cab request buttons to store request and turn on button light if they are pressed.
*/
void CheckButtons(void);

/**
* @brief Same as <CheckButtons>"()", but skips checking buttons requesting the current floor.
* @param[in] currFloor Current floor to ignore requests to.
*/
void CheckButtonsWithFloor(int currFloor);

/**
* @brief Same as <CheckButtons>"()", but skips checking buttons requesting the current floor.
* @param[in] currFloor Current floor to ignore requests to.
*/
void ClearQueue(void);


void RemoveFromQueue(int floor);