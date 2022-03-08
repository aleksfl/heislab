#include "elevator.h"


// 0 indicates door is closed, 1 that it is open
int DoorState = 0;
int matQueue[N_FLOORS][N_BUTTONS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};

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

// Only used by wait state
void CheckButtons(void) {
CheckButtonsWithFloor(UNDEFINED_FLOOR);
}

void CheckButtonsWithFloor(int currFloor) {
	if (!elevio_stopButton()) {   
        for(int f = 0; f<N_FLOORS; f++) {
			if (f == currFloor) {
				continue;
			}
            for(int b = 0; b<N_BUTTONS; b++){
				if (!matQueue[f][b]) {
					int btnPressed = elevio_callButton(f, b);
					if (btnPressed) {
						matQueue[f][b] = 1;
						printf("Button being pressed at floor %d, type %d\n",f,b);
						elevio_buttonLamp(f, b, btnPressed);
					}
				}
            }
        }
    }
}

void ClearQueue(void) {
	for(int i = 0; i<N_FLOORS; i++) {
		for(int j = 0; j<N_BUTTONS; j++){			
			elevio_buttonLamp(i, j, 0);			
			matQueue[i][j] = 0;
		}
	}	
}

void RemoveFromQueue(int floor) {
	 for(int j = 0; j<N_BUTTONS; j++){        
        elevio_buttonLamp(floor, j, 0);			        
        matQueue[floor][j] = 0;
    }
}


            
    
