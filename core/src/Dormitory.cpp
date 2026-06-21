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
    if (room == nullptr) return;
    if (getRoom(room->getRoomNumber()) != nullptr) throw InvalidRoomNumberException(room->getRoomNumber());
    rooms.push_back(room);
}

void Dormitory::removeRoom(int roomNumber){
    Room* room = getRoom(roomNumber);
    if (room == nullptr) return; //throw
    if (!room->isEmpty()) throw RoomNotEmptyException(roomNumber); 
    rooms.erase(remove(rooms.begin(), rooms.end(), room), rooms.end());
    delete room;
}

void Dormitory::addStudentToRoom(Student* student,Room* room){
    if (student == nullptr) return; 
    if (room == nullptr) return; 
    if (getRoom(room->getRoomNumber()) == nullptr) throw InvalidRoomNumberException(room->getRoomNumber()); 
    if (room->isFull()) throw RoomFullException(room->getRoomNumber(), room->getCapacity());
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
        delete room;
    }
}