#include "SingleRoom.h"

SingleRoom::SingleRoom(int roomNumber): Room(roomNumber, 1){}

string SingleRoom::getType() const{
        return "Single Room";
}
