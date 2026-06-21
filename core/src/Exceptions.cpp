#include "Exceptions.h"

RoomFullException::RoomFullException(int roomNumber, int capacity)
:UDRMSException("Room: " + std::to_string(roomNumber) + " is full (capacity: " + std::to_string(capacity) + ")")
{}

InvalidCapacityException::InvalidCapacityException(int attemptedCapacity)
:UDRMSException("Invalid capacity: " + std::to_string(attemptedCapacity))
{}

InvalidRoomNumberException::InvalidRoomNumberException(int attemptedRoomNumber)
:UDRMSException("Invalid room number: " + std::to_string(attemptedRoomNumber))
{}

StudentAlreadyInRoomException::StudentAlreadyInRoomException(int studentID)
:UDRMSException("Student: " + std::to_string(studentID) + " is already in a room")
{}

RoomNotEmptyException::RoomNotEmptyException(int roomNumber)
:UDRMSException("Room " + std::to_string(roomNumber) + " is not empty")
{}

StudentNotInRoomException::StudentNotInRoomException(int studentID)
:UDRMSException("Student: " + std::to_string(studentID) + " is not in the room")
{}

FileException::FileException(std::string filename)
:UDRMSException("File error: " + filename)
{}
