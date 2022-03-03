#include "driver/elevio.h"


typedef enum {
    Init,            
    Standby,               
    Up,
    Down,
    Stop                    
} ElevState;


void TryOpenDoor(void);
void TryCloseDoor(void);

