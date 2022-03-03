#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "elevator.h"
#include "utils.h"


int main(){
    int currDir = DIRN_STOP;
    int currState = Init;
    while(1){
        switch (currState){
        case Init: {
            printf("Initializing");
            elevio_init();
            for(int i = 0; i<N_FLOORS; i++) {
                for(int j = 0; j<N_BUTTONS; j++){
                    matQueue[i][j] = 0;
                }
            }
            currState = Standby;
            break;
        }
        case Standby: {        
            CheckButtons();
            if (elevio_stopButton()) {                       
                currState = Stop;
            }
            break;
        }
        case Up: {
            CheckButtons();
            int currFloor = elevio_floorSensor();
            if(currFloor>0 && currFloor<=N_FLOORS) {
                if(matQueue[currFloor][BUTTON_HALL_UP] && matQueue[currFloor][BUTTON_HALL_UP]){
                    currState = Standby;
                }
                if(currFloor = N_FLOORS) {
                    currDir = DIRN_STOP; 
                    currState = Standby;
                }
                else if(currFloor = 1){
                    currState = Init;
                }
            }
            break;

        }
        case Down: {
            CheckButtons();
            int currFloor = elevio_floorSensor();
            if(currFloor>0 && currFloor<=N_FLOORS) {
                if(matQueue[currFloor][BUTTON_HALL_UP]){
                    currState = Standby;
                }
                if(currFloor = N_FLOORS) {
                    currDir = DIRN_STOP; 
                    currState = Standby;
                }
            }
        }
        case Stop: {
             elevio_stopLamp(1);
        }

        case Wait: {

        }

        default:
            currState = Init;
            break;
        }

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