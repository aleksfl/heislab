#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "queue.h"
#include "door.h"
#include "utils.h"


int main(){    
    int CurrDir = DIRN_STOP;
    int StopDir = DIRN_STOP;
    int CurrState = INIT;
    int PrevFloor = UNDEFINED_FLOOR;
    while(1){
        int currFloor = elevio_floorSensor();
        switch (CurrState){
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
            PrevFloor = floor;   
            CurrState = STANDBY;
            CurrDir = DIRN_STOP;
            break;
        }
        case STANDBY: {    
            elevio_motorDirection(DIRN_STOP);    
            if (elevio_stopButton()) {                       
                CurrState = STOP;                
                break;
            }
            CheckButtons();           
            if(CurrDir==DIRN_UP) {
                CurrDir = DIRN_STOP;
                for(int f = (currFloor+1); f<N_FLOORS; f++) {
                    if(RequestMatrix[f][BUTTON_HALL_UP] || RequestMatrix[f][BUTTON_CAB]) {
                        CurrDir = DIRN_UP;
                        CurrState = UP;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            } 
            else if(CurrDir==DIRN_DOWN) {
                CurrDir = DIRN_STOP;
                for(int f = (currFloor-1); f>=0; f--) {
                    if(RequestMatrix[f][BUTTON_HALL_DOWN] || RequestMatrix[f][BUTTON_CAB]) {
                        CurrDir = DIRN_DOWN;
                        CurrState = DOWN;
                        elevio_motorDirection(DIRN_DOWN);
                    }
                }
            } 
            else {
                int floor = currFloor;
                if (currFloor == UNDEFINED_FLOOR) {
                    floor = PrevFloor;
                } 
                else {
                    for(int b = 0; b<N_BUTTONS; b++){
                        if (RequestMatrix[currFloor][b]) {
                            RemoveFromQueue(currFloor);
                            CurrState = WAIT;
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
                        switch (StopDir) {
                            case DIRN_DOWN: {
                                CurrDir = DIRN_UP;
                                CurrState = UP;
                                elevio_motorDirection(DIRN_UP);
                                break;
                            }
                            case DIRN_UP: {
                                CurrDir = DIRN_DOWN;
                                CurrState = DOWN;
                                elevio_motorDirection(DIRN_DOWN);
                                break;
                            }
                        }
                    } 
                    else if(floor>lowestDistanceFloor) {
                        CurrDir = DIRN_DOWN;
                        CurrState = DOWN;
                        elevio_motorDirection(DIRN_DOWN);
                    } 
                    else if(floor<lowestDistanceFloor) {
                        CurrDir = DIRN_UP;
                        CurrState = UP;
                        elevio_motorDirection(DIRN_UP);
                    }
                }
            }            
            break;
        }
        case UP: {
            if (elevio_stopButton()) {                       
                CurrState = STOP;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(PrevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                if(RequestMatrix[currFloor][BUTTON_HALL_UP] || RequestMatrix[currFloor][BUTTON_CAB]){
                    RemoveFromQueue(currFloor);
                    elevio_motorDirection(DIRN_STOP); 
                    CurrState = WAIT;
                }
                if(currFloor == (N_FLOORS-1)) {                    
                    CurrState = WAIT;
                    CurrDir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP); 
                }
                PrevFloor = currFloor;
            }            
            break;
        }
        case DOWN: {
            if (elevio_stopButton()) {                       
                CurrState = STOP;                
                break;
            }
            CheckButtons();
            if(currFloor>=0 && currFloor<N_FLOORS) {
                if(PrevFloor!=currFloor){
                    elevio_floorIndicator(currFloor);
                }
                if(RequestMatrix[currFloor][BUTTON_HALL_DOWN] || RequestMatrix[currFloor][BUTTON_CAB]){
                    RemoveFromQueue(currFloor);                    
                    elevio_motorDirection(DIRN_STOP); 
                    CurrState = WAIT;
                }
                if(currFloor == 0) {                    
                    CurrState = WAIT;
                    CurrDir = DIRN_STOP; 
                    elevio_motorDirection(DIRN_STOP); 
                }
                PrevFloor = currFloor;
            }            
            break;
        }
        case STOP: {
            StopDir = CurrDir;
            CurrDir = DIRN_STOP;
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
            CurrState = STANDBY;
            }  else {
            CurrState = WAIT;      
            }             
            break;       
        }
        case OBSTRUCTED: {
            if (elevio_stopButton()) {                       
                CurrState = STOP;
                CurrDir = DIRN_STOP;
                break;
            }
            CheckButtons();      
            if (!elevio_obstruction()) {                
                CurrState = WAIT;
            }
        }

        case WAIT: {          
            elevio_motorDirection(DIRN_STOP);                          
            TryOpenDoor();
            for (int i=1; i<=300; i++) {                
                if (elevio_stopButton()) {                       
                    CurrState = STOP;
                    break;                                
                }                
                CheckButtonsWithFloor(currFloor);                                
                milliSleep(10);
            }                        
            
            TryCloseDoor();            
            if (DoorState == 1 && elevio_obstruction()) {
                CurrState = OBSTRUCTED;
            } 
            else {
                CurrState = STANDBY;
            }
            break;
        }

        default:
            CurrState = INIT;
            break;
        }
    milliSleep(10);
    }
}

