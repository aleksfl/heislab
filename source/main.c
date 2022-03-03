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
                    }
                }
            }
            else if(currDir==DIRN_DOWN) {
                currDir = DIRN_STOP;
                for(int f = currFloor+1; f<=N_FLOORS; f++) {
                    if(matQueue[currFloor-1][BUTTON_HALL_UP] && matQueue[currFloor-1][BUTTON_CAB]) {
                        currDir = DIRN_UP;
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
                    currState = Standby;
                }
                if(currFloor = N_FLOORS) {
                    currState = Standby;
                    currDir = DIRN_STOP; 
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
                    currState = Standby;
                }
                if(currFloor = 1) {
                    currState = Standby;
                    currDir = DIRN_STOP; 
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




/*
int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        int floor = elevio_floorSensor();
        printf("floor: %d \n",floor);

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if(btnPressed) {
                    createRequest(f, b, floor);
                }
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
*/