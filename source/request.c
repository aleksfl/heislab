#include "request.h"

void createRequest(int buttonFloor, int buttonType, int elevFloor) {
    struct Request *req = malloc(sizeof(struct Request));
    if(buttonType==2) {
        req->but = buttonFloor;    //Usikker på om dette blir riktig    
        req->loc = buttonFloor;
    }
    else if(buttonType==1) {
        req->but = Down;
        req->loc = buttonFloor;
    }
    else if (buttonType==0) {
        req->but = Up;
        req->loc = buttonFloor;
    }
    EnqueueFront(req);            //lag bedre system etterhvert, husk å dealloker etter bruk.
}