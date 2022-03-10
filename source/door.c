#include "door.h"

// 0 indicates door is closed, 1 that it is open
int DoorState = 0;

void TryOpenDoor(void) {
	// Only open door if it is closed
	if (DoorState == 0) {
		// Only open door when in valid floor
		if (elevio_floorSensor() != UNDEFINED_FLOOR) {
			DoorState = 1;
		}

	}
	elevio_doorOpenLamp(DoorState);
}
void TryCloseDoor(void) {
	// Only close door if it is open
	if (DoorState == 1) {
		// Do not close door when it is obstructed
		if (!elevio_obstruction()) {
			DoorState = 0;
		}

	}		
	elevio_doorOpenLamp(DoorState);
}
