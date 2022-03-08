#include "driver/elevio.h"

extern int DoorState;
extern int matQueue[N_FLOORS][N_BUTTONS];

#define UNDEFINED_FLOOR -1
#define UNDEFINED_DISTANCE -1

typedef enum {
    Init,            
    Standby,               
    Up,
    Down,
    Stop,
    Obstructed,
    Wait                    
} ElevState;


void TryOpenDoor(void);
void TryCloseDoor(void);

void CheckButtons(void);
void CheckButtonsWithFloor(int currFloor);

void ClearQueue(void);

void RemoveFromQueue(int floor);