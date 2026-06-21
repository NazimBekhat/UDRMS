#include "Dormitory.h"
#include "Room.h"
#include "Exceptions.h"
#include <algorithm>
using std::remove;

Dormitory::Dormitory(const string& name): name(name){}

string Dormitory::getName() const{
    return name;
}

void Dormitory::setName(const string& name){
    this->name = name;
}

const Restaurant&  Dormitory::getRestaurant() const{
    return restaurant;
}

Restaurant& Dormitory::getRestaurant(){
    return restaurant;
}

void Dormitory::addRoom(Room* room){
    if (room == nullptr) return; //throw later
    if (getRoom(room->getRoomNumber()) != nullptr) InvalidRoomNumberException(room->getRoomNumber()); //have to be non duplicate, thow
    rooms.push_back(room);
}

void Dormitory::removeRoom(int roomNumber){
    Room* room = getRoom(roomNumber);
    if (room == nullptr) return; //throw
    if (!room->isEmpty()) throw RoomNotEmptyException(roomNumber); //throw
    rooms.erase(remove(rooms.begin(), rooms.end(), room), rooms.end());
    delete room;

    //std::remove(rooms.begin(), rooms.end(), room)
    //Scans the vector, moves all elements that are NOT equal to room to the front, leaves the rest as garbage at the end. Returns an iterator pointing to where the garbage starts. The vector size hasn't changed yet.
    // Before: [A, B, room, C, D]
    // After remove: [A, B, C, D, garbage]  ← iterator points here

    //.erase(iterator, rooms.end())
    //Cuts off everything from that iterator to the end, actually shrinking the vector.
    //After erase: [A, B, C, D]
}

void Dormitory::addStudentToRoom(Student* student,Room* room){
    if (student == nullptr) return; //throw
    if (room == nullptr) return; //throw
    if (getRoom(room->getRoomNumber()) == nullptr) throw InvalidRoomNumberException(room->getRoomNumber()); //throw
    if (room->isFull()) throw RoomFullException(room->getRoomNumber(), room->getCapacity()); //throw
    room->addStudent(student);
}

Room* Dormitory::getRoom(int roomNumber){
    for (Room* room: rooms){
        if (room->getRoomNumber() == roomNumber) return room;
    }
    return nullptr;
}

const vector<Room*>& Dormitory::getRooms() const{
    return rooms;
}

Dormitory::~Dormitory(){
    for(Room* room: rooms){
        delete room; //enough since ~Room is virtual
    }
}