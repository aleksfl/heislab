#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "queue.h"
#include "door.h"
#include "utils.h"


int main(){    
    // Current direction of elevator cab
    int currDir = DIRN_STOP;

    // Last direction of cab at point when stop button was pressed.
    int stopDir = DIRN_STOP;

    // Current state of state machine.
    int currState = INIT;

    // Previous floor picked up by floor sensor.
    int prevFloor = UNDEFINED_FLOOR;
    while(1){
        int currFloor = elevio_floorSensor();
        switch (currState){

        // Initialization state for elevator startup and used to reset state machine in case of unexpected behaviour.
        // Initializes connection to elevator, turns off all lights, clears RequestMatrix and moves cab to a defined floor.
        case INIT: {
            printf("Initializing\n");
            elevio_init();
            elevio_stopLamp(0);             
            elevio_doorOpenLamp(0);
            ClearQueue();
            if (elevio_floorSensor() == UNDEFINED_FLOOR) {
                elevio_motorDirection(DIRN_DOWN);
                while (elevio_floorSensor() == UNDEFINED_FLOOR) {
                    // Do nothing, floor is not defined
                    milliSleep(10);
                }                              
            }
            int floor = elevio_floorSensor();
            elevio_motorDirection(DIRN_STOP);  
            elevio_floorIndicator(floor);                              
            prevFloor = floor;   
            currState = STANDBY;
            currDir = DIRN_STOP;
            break;
        }

        // Represents elevator being ready to process requests.
        case STANDBY: {    
            elevio_motorDirection(DIRN_STOP);    
            if (elevio_stopButton()) {                       
                currState = STOP;                
                break;
            }
            CheckButtons(); 
            // Keep going if more requests in same direction.          
            if(currDir==DIRN_UP) {
                currDir = DIRN_STOP;
                for(int f = (currFloor+1); f<N_FLOORS; f++) {
                    if(RequestMatrix[f][BUTTON_HALL_UP] || RequestMatrix[f][BUTTON_CAB]) {
                        currDir = DIRN_UP;
                        currState = UP;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            } 
            else if(currDir==DIRN_DOWN) {
                currDir = DIRN_STOP;
                for(int f = (currFloor-1); f>=0; f--) {
                    if(RequestMatrix[f][BUTTON_HALL_DOWN] || RequestMatrix[f][BUTTON_CAB]) {
                        currDir = DIRN_DOWN;
                        currState = DOWN;
                        elevio_motorDirection(DIRN_DOWN);
                    }
                }
            } 
            else {
                int floor = currFloor;

                // We should only be in Standby in a defined floor, unless the cab is stopped between floors.
                // In that case, using the previous floor lets us perform calculations as if we are on a defined floor.
                // (will not always give the lowest distance, but there is not way to know how close to the destination floor the cab is)
                if (currFloor == UNDEFINED_FLOOR) {
                    floor = prevFloor;
                } 
                else {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if (RequestMatrix[currFloor][b]) {
                            RemoveFromQueue(currFloor);
                            currState = WAIT;
                            break;
                        }
                    }       
                }         
                int lowestDistance = UNDEFINED_DISTANCE;
                int lowestDistanceFloor = UNDEFINED_FLOOR;
                for(int f = 0; f<N_FLOORS; f++) {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if (RequestMatrix[f][b]) {
                            int dist = calculateDistance(floor, f);                            
                            if (lowestDistance == UNDEFINED_DISTANCE || dist < lowestDistance) {
                            lowestDistance = dist;
                            lowestDistanceFloor = f;
                            }
                        }
                    }
                }
                // When not in motion, most efficient to the nearest requested floor.
                // Distance of 0 means the requested floor is the current floor.
                if (lowestDistanceFloor != UNDEFINED_FLOOR && lowestDistance != UNDEFINED_DISTANCE) {

                    // In this case, if stopDir is up or down, the previous floor has been requested while the cab is between floors.
                    // Naturally, going in the opposite directon takes the cab back to the previous floor.
                    if (lowestDistance == 0) {
                        switch (stopDir) {
                            case DIRN_DOWN: {
                                currDir = DIRN_UP;
                                currState = UP;
                                elevio_motorDirection(DIRN_UP);
                                break;
                            }
                            case DIRN_UP: {
                                currDir = DIRN_DOWN;
                                currState = DOWN;
                                elevio_motorDirection(DIRN_DOWN);
                                break;
                            }
                        }
                    } 
                    // Neither floor values should be undefined here,
                    // although in the off chance that it happens the problem should correct itself.
                    else if(floor>lowestDistanceFloor) {
                        currDir = DIRN_DOWN;
                        currState = DOWN;
                        elevio_motorDirection(DIRN_DOWN);
                    } 
                    else if(floor<lowestDistanceFloor) {
                        currDir = DIRN_UP;
                        currState = UP;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            }            
            break;
        }

        // Represents elevator moving upwards to process relevant requests.
        // Stops when there are no more requests above current floor or top floor is reached.
        case UP: {
            if (elevio_stopButton()) {                       
                currState = STOP;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }

                // Do not service requests that want to go downwards when we are going upwards.
                if(RequestMatrix[currFloor][BUTTON_HALL_UP] || RequestMatrix[currFloor][BUTTON_CAB]){
                    RemoveFromQueue(currFloor);
                    elevio_motorDirection(DIRN_STOP); 
                    currState = WAIT;
                }
                if(currFloor == (N_FLOORS-1)) {                    
                    currState = WAIT;
                    currDir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP); 
                }
                prevFloor = currFloor;
            }            
            break;
        }

        // Represents elevator moving downward to process relevant requests.
        // Stops when there are no more requests below current floor or bottom floor is reached.
        case DOWN: {
            if (elevio_stopButton()) {                       
                currState = STOP;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                // Do not service requests that want to go upwards when we are going downwards.
                if(RequestMatrix[currFloor][BUTTON_HALL_DOWN] || RequestMatrix[currFloor][BUTTON_CAB]){
                    RemoveFromQueue(currFloor);                    
                    elevio_motorDirection(DIRN_STOP); 
                    currState = WAIT;
                }
                if(currFloor == 0) {                    
                    currState = WAIT;
                    currDir = DIRN_STOP; 
                    elevio_motorDirection(DIRN_STOP); 
                }
                prevFloor = currFloor;
            }            
            break;
        }

        // Represents the period between the stop button being pressed and when it is released.
        case STOP: {
            stopDir = currDir;
            currDir = DIRN_STOP;
            elevio_motorDirection(DIRN_STOP);                          
            ClearQueue();             
            elevio_stopLamp(1);            
            if (elevio_floorSensor() != UNDEFINED_FLOOR) {
                TryOpenDoor();
            }
            while (elevio_stopButton()) {
                milliSleep(10);
            }
            elevio_stopLamp(0);             
            if (elevio_floorSensor() == UNDEFINED_FLOOR) {
            currState = STANDBY;
            }  else {
            currState = WAIT;      
            }             
            break;       
        }
        // Represents door being open while obstruction switch is on.
        // In this case the elevator is unable to do anything except take requests and wait for obstruction to clear.
        case OBSTRUCTED: {
            if (elevio_stopButton()) {                       
                currState = STOP;
                currDir = DIRN_STOP;
                break;
            }
            CheckButtons();      
            if (!elevio_obstruction()) {                
                currState = WAIT;
            }
        }
        
        // Represents elevator waiting at a defined floor for 3 seconds.
        // Attempt to open the door, keep it open for the duration of the wait, then close it.
        case WAIT: {          
            elevio_motorDirection(DIRN_STOP);                          
            TryOpenDoor();
            for (int i=1; i<=300; i++) {                
                if (elevio_stopButton()) {                       
                    currState = STOP;
                    break;                                
                }                
                // If we check the buttons for the current floor here, we end up with double waits.
                // Since we already are waiting at current floor, we can use this method to avoid checking current floor buttons.    
                // This prevents the double wait issue.
                CheckButtonsExcludeFloor(currFloor);                                
                milliSleep(10);
            }                        
            
            TryCloseDoor();            
            // Important that door is open before we act on potential obstruction.
            if (DoorState == 1 && elevio_obstruction()) {
                currState = OBSTRUCTED;
            } 
            else {
                currState = STANDBY;
            }
            break;
        }

        // This should never happen, but resetting the state machine if it does is important for robustness.
        default:
            currState = INIT;
            break;
        }
    milliSleep(10);
    }
}

