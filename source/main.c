#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "elevator.h"
#include "utils.h"


int main(){    
    int currDir = DIRN_STOP;
    int stopDir = DIRN_STOP;
    int currState = Init;
    int prevFloor = -1;
    int k = 0;
    while(1){
        int currFloor = elevio_floorSensor();
         //if(!k%20) {
         //    printf("Current floor: %d \n",currFloor);
         //}
         if(k==100) {
             for(int f = 3; f>=0; f--) {
                 printf("\n");
                 for(int b = 0; b<N_BUTTONS; b++){
                     printf(" %d",matQueue[f][b]);
                 }
             }
             printf("\n");
             k=0;
        }
        switch (currState){
        case Init: {
            printf("Initializing\n");
            elevio_init();
            elevio_stopLamp(0);             
            elevio_doorOpenLamp(0);
            ClearQueue();
            if (elevio_floorSensor() == -1) {
                elevio_motorDirection(DIRN_DOWN);
                while (elevio_floorSensor() == -1) {
                    // Do nothing, floor is not defined
                    milliSleep(10);
                }                              
            }
            int floor = elevio_floorSensor();
            elevio_motorDirection(DIRN_STOP);  
            elevio_floorIndicator(floor);                              
            prevFloor = floor;   
            currState = Standby;
            currDir = DIRN_STOP;
            break;
        }
        case Standby: {    
            elevio_motorDirection(DIRN_STOP);    
            if (elevio_stopButton()) {                       
                currState = Stop;                
                break;
            }
            CheckButtons();           
            if(currDir==DIRN_UP) {
                currDir = DIRN_STOP;
                for(int f = (currFloor+1); f<N_FLOORS; f++) {
                    if(matQueue[f][BUTTON_HALL_UP] || matQueue[f][BUTTON_CAB]) {
                        currDir = DIRN_UP;
                        currState = Up;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            } else if(currDir==DIRN_DOWN) {
                currDir = DIRN_STOP;
                for(int f = (currFloor-1); f>=0; f--) {
                    if(matQueue[f][BUTTON_HALL_DOWN] || matQueue[f][BUTTON_CAB]) {
                        currDir = DIRN_DOWN;
                        currState = Down;
                        elevio_motorDirection(DIRN_DOWN);
                    }
                }
            } else {
                int floor = currFloor;
                if (currFloor == -1) {
                    floor = prevFloor;
                } else {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if (matQueue[currFloor][b]) {
                            RemoveFromQueue(currFloor);
                            printf("wait 1");
                            currState = Wait;
                            break;
                        }
                    }       
                }         
                int lowestDistance = -1;
                int lowestDistanceFloor = -1;
                for(int f = 0; f<N_FLOORS; f++) {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if (matQueue[f][b]) {
                            int dist = calculateDistance(floor, f);                            
                            if (lowestDistance == -1 || dist < lowestDistance) {
                            lowestDistance = dist;
                            lowestDistanceFloor = f;
                            }
                        }
                    }
                }
                
                if (lowestDistanceFloor != -1 && lowestDistance != -1) {
                        if (lowestDistance == 0) {
                                switch (stopDir) {
                                    case DIRN_DOWN:
                                        currDir = DIRN_UP;
                                        currState = Up;
                                        elevio_motorDirection(DIRN_UP);
                                        break;
                                    case DIRN_UP:
                                        currDir = DIRN_DOWN;
                                        currState = Down;
                                        elevio_motorDirection(DIRN_DOWN);
                                        break;
                                }                                
                        } else if(floor>lowestDistanceFloor) {
                                currDir = DIRN_DOWN;
                                currState = Down;
                                elevio_motorDirection(DIRN_DOWN);
                            } else if(floor<lowestDistanceFloor) {
                                currDir = DIRN_UP;
                                currState = Up;
                                elevio_motorDirection(DIRN_UP);
                            }
                }
            }
            break;
        }
        case Up: {
            if (elevio_stopButton()) {                       
                currState = Stop;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                for(int b = 0; b<N_BUTTONS; b++){
                    if (matQueue[currFloor][b]) {
                        RemoveFromQueue(currFloor);
                        printf("wait 2");
                        currState = Wait;
                        break;
                    }
                }
                if(currFloor == (N_FLOORS-1)) {
                    printf("wait 3");
                    currState = Wait;
                    currDir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP); 
                }
                prevFloor = currFloor;
            }            
            break;
        }
        case Down: {
            if (elevio_stopButton()) {                       
                currState = Stop;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(prevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                for(int b = 0; b<N_BUTTONS; b++){
                    if (matQueue[currFloor][b]) {
                        RemoveFromQueue(currFloor);
                        printf("wait 4");
                        currState = Wait;
                        break;
                    }
                }
                if(currFloor == 0) {
                    printf("wait 5");
                    currState = Wait;
                    currDir = DIRN_STOP; 
                    elevio_motorDirection(DIRN_STOP); 
                }
                prevFloor = currFloor;
            }            
            break;
        }
        case Stop: {
            stopDir = currDir;
            currDir = DIRN_STOP;
            elevio_motorDirection(DIRN_STOP);                          
            ClearQueue();             
            elevio_stopLamp(1);             
             while (elevio_stopButton()) {
                 milliSleep(10);
             }
             elevio_stopLamp(0);             
             if (elevio_floorSensor() == -1) {
                currState = Standby;
             }  else {
                 printf("wait 6");
                currState = Wait;      
             }             
             break;       
        }
        case Obstructed: {
            if (elevio_stopButton()) {                       
                currState = Stop;
                currDir = DIRN_STOP;
                break;
            }
            CheckButtons();      
            if (!elevio_obstruction()) {
                printf("wait 7");
                currState = Wait;
            }
        }

        case Wait: {          
            elevio_motorDirection(DIRN_STOP);                          
            TryOpenDoor();
            for (int i=1; i<=300; i++) {                
                if (elevio_stopButton()) {                       
                currState = Stop;
                break;                                
                }                
                CheckButtons();                                
                milliSleep(10);
            }                        
            
            TryCloseDoor();            
            if (DoorState == 1 && elevio_obstruction()) {
                currState = Obstructed;
            } else {
                currState = Standby;
            }
            
            break;
        }

        default:
            currState = Init;
            break;
        }
        k++;
    milliSleep(10);
    }
}

