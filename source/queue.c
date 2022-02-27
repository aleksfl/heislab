#include "queue.h"

// There can never be more than 10 active requests in normal operation
struct Request *blankQueue[] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

struct Request *mainQueue[] = blankQueue;

// Adds request to first blank element in the main queue
void EnqueueBack(Request* req) {
	for (i = 0; i < 10; i++ ) {
        // Standard behaviour
        if (mainQueue[i] == nullptr) {            
            mainQueue[i] = &req;
            break;
        }        
    }
}

// Adds request to first element in queue by recreating it and moving elements one index forward
void EnqueueFront(Request* req) {
    if (isMainQueueEmpty) {
        // If the queue is empty the behavior of these methods is the same, but this is more performance efficient
        EnqueueBack(req);
    }
    
    if (*mainQueue[9] != nullptr) {
        // This indicates we are trying to add the request to a full queue, which should not happen in normal operation
    } else {
        struct Request *newQueue[] = blankQueue;
        blankQueue[0] = req;
        for (i = 1; i < 10; i++ ) {
            newQueue[i] = mainQueue[i - 1];   
        }
        mainQueue = newQueue;
    }
}


bool isMainQueueEmpty() {
    return mainQueue == blankQueue;
}

// Returns first element and remakes queue moving each element one index back
Request* Dequeue() {       
    Request* retptr = mainQueue[0];

    if (!isMainQueueEmpty()) {
        if (retptr == nullptr) {
            // Something is very wrong here.
        }
        struct Request *newQueue[] = blankQueue;
        for (i = 1; i < 10; i++ ) {
            newQueue[i - 1] = mainQueue[i];   
        }
        mainQueue = newQueue;
    }

    return retptr;
}