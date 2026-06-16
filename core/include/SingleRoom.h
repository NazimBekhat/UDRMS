#ifndef SINGLEROOM_H
#define SINGLEROOM_H

#include <string>
using  std::string;
#include "Room.h"

class SingleRoom: public Room{
    public:
        SingleRoom(int roomNumber);
        virtual string getType() const override;
};

#endif
