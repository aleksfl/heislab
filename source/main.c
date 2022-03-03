#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "elevator.h"
#include "utils.h"


int main(){
    int currDir = DIRN_STOP;
    int currState = Init;
    int currFloor = elevio_floorSensor();
    int prevFloor = -1;
    while(1){
        switch (currState){
        case Init: {
            printf("Initializing");
            elevio_init();
            ClearQueue();
            if (elevio_floorSensor() == -1) {
                elevio_motorDirection(DIRN_DOWN);
                while (elevio_floorSensor() == -1) {
                    // Do nothing, floor is not defined
                    milliSleep(10);
                }                 
                elevio_motorDirection(DIRN_STOP);  
                elevio_floorIndicator(elevio_floorSensor());                     
            }
            currState = Standby;
            currDir = DIRN_STOP;
            break;
        }
        case Standby: {    
            elevio_motorDirection(DIRN_STOP);    
            CheckButtons();
            if (elevio_stopButton()) {                       
                currState = Stop;
                currDir = DIRN_STOP;
            }
            if(currDir==DIRN_UP) {
                currDir = DIRN_STOP;
                for(int f = currFloor+1; f<=N_FLOORS; f++) {
                    if(matQueue[currFloor-1][BUTTON_HALL_UP] && matQueue[currFloor-1][BUTTON_CAB]) {
                        currDir = DIRN_UP;
                        currState = Up;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            }
            else if(currDir==DIRN_DOWN) {
                currDir = DIRN_STOP;
                for(int f = currFloor-1; f>0; f--) {
                    if(matQueue[currFloor-1][BUTTON_HALL_DOWN] && matQueue[currFloor-1][BUTTON_CAB]) {
                        currDir = DIRN_DOWN;
                        currState = Down;
                        elevio_motorDirection(DIRN_DOWN);
                    }
                }
            }
            else {
                for(int f = 0; f<N_FLOORS; f++) {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if(matQueue[f][b]) {
                            if(currFloor>(f+1)) {
                                currDir = DIRN_DOWN;
                                currState = Down;
                                elevio_motorDirection(DIRN_DOWN);
                            }
                            else if(currFloor<(f+1)) {
                                currDir = DIRN_UP;
                                currState = Up;
                                elevio_motorDirection(DIRN_UP);
                            }
                            else {
                                RemoveFromQueue(f);
                                currState = Wait;
                            }
                        }
                    }
                }
            }
            break;
        }
        case Up: {
            CheckButtons();
            if(currFloor>0 && currFloor<=N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                if(matQueue[currFloor-1][BUTTON_HALL_UP] && matQueue[currFloor-1][BUTTON_CAB]){
                    RemoveFromQueue(currFloor-1);
                    currState = Wait;
                }
                if(currFloor = N_FLOORS) {
                    currState = Wait;
                    currDir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP); 
                }
            }
            prevFloor = currFloor;
            break;

        }
        case Down: {
            CheckButtons();
            if(currFloor>0 && currFloor<=N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                if(matQueue[currFloor-1][BUTTON_HALL_DOWN] && matQueue[currFloor-1][BUTTON_CAB]){
                    RemoveFromQueue(currFloor-1);
                    currState = Wait;
                }
                if(currFloor = 1) {
                    currState = Wait;
                    currDir = DIRN_STOP; 
                    elevio_motorDirection(DIRN_STOP); 
                }
            }
            prevFloor = currFloor;
            break;
        }
        case Stop: {
             elevio_stopLamp(1);             
             currState = Wait;      
             break;       
        }

        case Wait: {          
            elevio_motorDirection(DIRN_STOP);
            TryOpenDoor();  
            int turnedOffStop = 0;
            for (int i=1; i<=300; i++) {
                CheckButtons();
                if (elevio_stopButton()) {                       
                currState = Stop;
                } else if (!turnedOffStop) {
                    elevio_stopLamp(0);
                    // Done for performance benefit
                    turnedOffStop = 1;
                }
                milliSleep(10);
            }
            // return to previous course
            TryCloseDoor();
            if (DoorState == 1 && elevio_obstruction()) {
                currState = Wait;
            } else {
                currState = Standby;
            }
            
            break;
        }

        default:
            currState = Init;
            break;
        }
    milliSleep(10);
    }
}

