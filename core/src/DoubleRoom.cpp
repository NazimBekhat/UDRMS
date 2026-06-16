#include "DoubleRoom.h"

DoubleRoom::DoubleRoom(int roomNumber): Room(roomNumber, 2){}

string DoubleRoom::getType() const{
        return "Double Room";
}
