#include "queue.h"

int RequestMatrix[N_FLOORS][N_BUTTONS] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};


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
				if (!RequestMatrix[f][b]) {
					int btnPressed = elevio_callButton(f, b);
					if (btnPressed) {
						RequestMatrix[f][b] = 1;
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
			RequestMatrix[i][j] = 0;
		}
	}	
}

void RemoveFromQueue(int floor) {
	 for(int j = 0; j<N_BUTTONS; j++){        
        elevio_buttonLamp(floor, j, 0);			        
        RequestMatrix[floor][j] = 0;
    }
}
