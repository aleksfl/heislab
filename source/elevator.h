#include "driver/elevio.h"

extern int DoorState;
extern int matQueue[N_FLOORS][N_BUTTONS];

typedef enum {
    Init,            
    Standby,               
    Up,
    Down,
    Stop,
    Wait                    
} ElevState;


void TryOpenDoor(void);
void TryCloseDoor(void);
void CheckButtons(void);

void ClearQueue(void);

void RemoveFromQueue(int floor);