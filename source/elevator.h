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
<<<<<<< HEAD
void checkButtonInput(int *matrixQueue[N_FLOORS][N_BUTTONS]);
=======

void CheckButtons(void);
>>>>>>> 48ad3713effdcbe8e140f9e80f68b7ef8c235424
