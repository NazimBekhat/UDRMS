#ifndef SHAREDROOM_H
#define SHAREDROOM_H

#include <string>
using  std::string;
#include "Room.h"

class SharedRoom: public Room{
    public:
        SharedRoom(int roomNumber, int capacity);
        virtual string getType() const override;
};

#endif