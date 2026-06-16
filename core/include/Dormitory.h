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
        vector<Room*> rooms; //Since Room is abstract we used a pointer so when we want to access any type of room we just use this one

    public:
        Dormitory(const string& name);

        string getName() const;
        void setName(const string& name);

        Restaurant& getRestaurant(); //no need for setters since we can modify by using class setters, it is noncosnt since we will modify it later

        void addRoom(Room* room); //cannot add const in the beggining since we are going to push back then it is considered as modifying the object
        void removeRoom(int roomNumber);
        void addStudentToRoom(Student* student,Room* room);

        Room* getRoom(int roomNumber); //returns a pointer to the desired room


        //we have vector<Room*> rooms; so we need a destructor in which we delete those oen by oe
        ~Dormitory();
        
        //since dorm will never be copied in our system, then we delete copy and assignment operators from this class
        Dormitory(const Dormitory&)=delete;
        Dormitory& operator=(const Dormitory&)=delete;
    };

//Since when we do Dorm d2 = d1, the compileer will create a shallow copy then it will delete the same pointer twice

#endif