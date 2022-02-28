#pragma once

enum Button { FirstFloor = 1, SecondFloor = 2, ThirdFloor = 3, FourthFloor = 4, Up = 5, Down = 6}
enum Location { FirstFloor = 1, SecondFloor = 2, ThirdFloor = 3, FourthFloor = 4}

// Macros for simpler code
#define nullptr ((void*)0)

struct Request {
    enum Button but;
    enum Location loc;
}

void EnqueueFront(Request* req);

void EnqueueBack(Request* req);

Request* Dequeue(void);

