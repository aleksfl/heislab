#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "elevator.h"
#include "utils.h"


int main(){    
    int currDir = DIRN_STOP;
    int stopDir = DIRN_STOP;
    int currState = INIT;
    int prevFloor = UNDEFINED_FLOOR;
    while(1){
        int currFloor = elevio_floorSensor();
        switch (currState){
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
        case STANDBY: {    
            elevio_motorDirection(DIRN_STOP);    
            if (elevio_stopButton()) {                       
                currState = STOP;                
                break;
            }
            CheckButtons();           
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
                
                if (lowestDistanceFloor != UNDEFINED_FLOOR && lowestDistance != UNDEFINED_DISTANCE) {
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

        case WAIT: {          
            elevio_motorDirection(DIRN_STOP);                          
            TryOpenDoor();
            for (int i=1; i<=300; i++) {                
                if (elevio_stopButton()) {                       
                    currState = STOP;
                    break;                                
                }                
                CheckButtonsWithFloor(currFloor);                                
                milliSleep(10);
            }                        
            
            TryCloseDoor();            
            if (DoorState == 1 && elevio_obstruction()) {
                currState = OBSTRUCTED;
            } 
            else {
                currState = STANDBY;
            }
            break;
        }

        default:
            currState = INIT;
            break;
        }
    milliSleep(10);
    }
}

