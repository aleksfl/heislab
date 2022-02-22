enum ReqButton { FirstFloor = 1, SecondFloor = 2, ThirdFloor = 3, FourthFloor = 4, Up = 5, Down = 6}
enum Location { FirstFloor = 1, SecondFloor = 2, ThirdFloor = 3, FourthFloor = 4}


struct Request {
    enum ReqButton dest;
    enum Location loc;

}