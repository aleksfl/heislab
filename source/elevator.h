#include "driver/elevio.h"

extern int DoorState;
extern int matrixQueue[N_FLOORS][N_BUTTONS];

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


void TryOpenDoor(void);
void TryCloseDoor(void);

void CheckButtons(void);
void CheckButtonsWithFloor(int currFloor);

void ClearQueue(void);

void RemoveFromQueue(int floor);