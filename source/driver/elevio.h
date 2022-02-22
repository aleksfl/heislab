/**
* @file
* @brief Interface between the elevator program and hardware
*/

#pragma once


#define N_FLOORS 4

/** Enum for motor directions */  
typedef enum {
    DIRN_DOWN   = -1,               /** Motor direction is down */
    DIRN_STOP   = 0,                /** Stop the motor */
    DIRN_UP     = 1                 /** Motor direction is down */
} MotorDirection;


#define N_BUTTONS 3

/** Enum for button types */
typedef enum { 
    BUTTON_HALL_UP      = 0,        /** Up button */
    BUTTON_HALL_DOWN    = 1,        /** Down button */
    BUTTON_CAB          = 2         /** Button in cab */
} ButtonType;

/**
* @brief Initializes the interface
*/
void elevio_init(void);

/**
* @brief Sets the motor direction of the elevator .
*
* @param[in] dirn Motor direction enum
*/
void elevio_motorDirection(MotorDirection dirn);

/**
* @brief Turns the button lights on and off. 
*
* @param[in] floor The floor of the button
* @param[in] button ButtonType enum describing the type of button
* @param[in] value binary value deciding if the lamp should be on or off
*/
void elevio_buttonLamp(int floor, ButtonType button, int value);

/**
* @brief Indicator showing the current or latest floor of the elevator cab.
*
* @param[in] floor The current floor of the elevator cab
*/
void elevio_floorIndicator(int floor);

/**
* @brief Lamp indicating the door is open.
*
* @param[in] value integer set to 1 if the door is open
*/
void elevio_doorOpenLamp(int value);

/**
* @brief Lamp indicating that the stop button is being pressed
*
* @param[in] value integer set to 1 if the stop button is being pressed
*/
void elevio_stopLamp(int value);

/**
* @brief Checks if a button is called
*
* @param[in] floor  The floor of the button
* @param[in] button The button type being pressed
*
* @return 1 if call button is being pressed, else return 0
*/
int elevio_callButton(int floor, ButtonType button);

/**
* @brief Checks the current floor of the eelevator cab.
*
* @return floor value if at a valid floor, else return -1
*/
int elevio_floorSensor(void);

/**
* @brief Checks if the stop button is being pressed
*
* @return 1 if stop button is being pressed, else return 0
*/
int elevio_stopButton(void);

/**
* @brief Checks if something is obstructing the elevator doors
*
* @return 1 if there is an obstruction, else return 0
*/
int elevio_obstruction(void);

