#ifndef DOUBLEROOM_H
#define DOUBLEROOM_H

#include <string>
using  std::string;
#include "Room.h"

class DoubleRoom: public Room{
    public:
        DoubleRoom(int roomNumber);
        virtual string getType() const override;
};

#endif