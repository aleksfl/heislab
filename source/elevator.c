#include "elevator.h"


// 0 indicates door is closed, 1 that it is open
int DoorState = 0;

void TryOpenDoor(void) {
	// Only open door if it is closed
	if (DoorState == 0) {
		// Only open door when in valid floor
		if (elevio_floorSensor() != -1) {
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

void checkButtonInput(int *matrixQueue[N_FLOORS][N_BUTTONS]) {
    for(int f = 0; f<N_FLOORS; f++) {
        for(int b = 0; b<N_BUTTONS; b++){
            int btnPressed = elevio_callButton(f+1, b+1);
            if (btnPressed) {
                *matrixQueue[f][b] = 1;
                elevio_buttonLamp(f, b, btnPressed);
            }
        }
    }
}

