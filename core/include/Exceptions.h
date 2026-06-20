#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "UDRMSException.h"
#include <string>

class RoomFullException: public UDRMSException{
    public:
        RoomFullException(int roomNumber, int capacity);
};

class InvalidCapacityException: public UDRMSException{
    public:
        InvalidCapacityException(int attemptedCapacity);
};

class InvalidRoomNumberException: public UDRMSException{
    public:
        InvalidRoomNumberException(int attemptedRoomNumber);
};

class StudentAlreadyInRoomException: public UDRMSException{
    public:
        StudentAlreadyInRoomException(int studentID);
};

class StudentNotInRoomException: public UDRMSException{
    public:
        StudentNotInRoomException(int studentID);
};


class FileException: public UDRMSException{
    public:
        FileException(std::string filename);
};



#endif