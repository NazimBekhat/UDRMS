#include "SharedRoom.h"
#include "Exceptions.h"

SharedRoom::SharedRoom(int roomNumber, int capacity): Room(roomNumber, capacity){
    if (capacity <= 2 || capacity >= 5) throw InvalidCapacityException(capacity); //can do this since the base is constructed before derived
}

std::string SharedRoom::getType() const{
    return "Shared Room";
}