#include "elevator.h"

bool isRequestServed(*Request reqPtr, int currentFloor) {	
	if (((int)(reqPtr->Button)) == currentFloor) {
		return true;
	} else if ((reqPtr->Button == Up || reqPtr->Button == Down) && reqPtr->Location == currentFloor) {
		return true;
	}
	return false;
}

void ServeRequest(*Request reqPtr) {
	if (reqPtr != nullptr) {
	while (1) {
		int currentFloor = elevio_floorSensor(); 
		if (isRequestServed(reqPtr, currentFloor)) {
			elevio_motorDirection(DIRN_STOP)
			break;
		}
		if (reqPtr->Button == Up || reqPtr->Button == Down) {
				if ((int)(reqPtr->Location) < currentFloor) {
				elevio_motorDirection(DIRN_DOWN);
			} else {
				elevio_motorDirection(DIRN_UP);
			}
		} else {
			if ((int)(reqPtr->Button) < currentFloor) {
				elevio_motorDirection(DIRN_DOWN);
			} else {
				elevio_motorDirection(DIRN_UP);
			}
		}
	}

	Dequeue();
	}
}