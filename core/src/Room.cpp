#include "Room.h"
#include "Student.h"
#include <vector>
using  std::vector;
#include <string>
using  std::string;


Room::Room(int roomNumber, int capacity): roomNumber(roomNumber), capacity(capacity){}

void Room::setRoomNumber(int roomNumber){
    this->roomNumber = roomNumber;
}

int Room::getRoomNumber() const{
    return roomNumber;
}

void Room::setCapacity(int capcity){
    this->capacity = capcity;
}

int Room::getCapacity() const{
    return capacity;
}

void Room::addStudent(Student* student){
    if (isFull()) return;
    for (Student* s: students){
        if (s==student) return; //return an exception after this
    }
    students.push_back(student);
} //we add one student at a time then we pass 1 student
        
void Room::removeStudent(int studentID){
    for (auto ptr = students.begin(); ptr != students.end(); ptr++){
        if ((*ptr)->getStudentID() == studentID){
            students.erase(ptr); 
            return;
        }
    }
} //remove by student ID to apply operator overloading
                
void Room::displayStudentsInRoom(){
    for (Student* student: students){
        student->display();
    }
}

bool Room::isFull(){
    return capacity == students.size();
}