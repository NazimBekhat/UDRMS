#ifndef DORMITORY_H
#define DORMITORY_H

#include <string>
using std::string;
#include <vector>
using std::vector;
#include "Restaurant.h"

class Room;
class Student;

class Dormitory{
    private:
        string name;
        Restaurant restaurant;
        vector<Room*> rooms;

    public:
        Dormitory(const string& name);

        string getName() const;
        void setName(const string& name);

        const Restaurant& getRestaurant() const; 
        Restaurant& getRestaurant(); 

        void addRoom(Room* room); 
        void removeRoom(int roomNumber);
        void addStudentToRoom(Student* student,Room* room);

        Room* getRoom(int roomNumber);
        const vector<Room*>& getRooms() const;

        ~Dormitory();
        
        Dormitory(const Dormitory&)=delete;
        Dormitory& operator=(const Dormitory&)=delete;
        Dormitory(Dormitory&&) = default;
        Dormitory& operator=(Dormitory&&) = default;
    };


#endif