#include "SharedRoom.h"

SharedRoom::SharedRoom(int roomNumber, int capacity): Room(roomNumber, capacity){}

string SharedRoom::getType() const{
        return "Shared Room";
}
