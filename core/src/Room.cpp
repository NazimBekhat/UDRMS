#include "Room.h"
#include "Student.h"
#include "Exceptions.h"
#include <vector>
using  std::vector;
#include <string>
using  std::string;

static int validateRoomNumber(int roomNumber){
    if (roomNumber <= 0) throw InvalidRoomNumberException(roomNumber);
    return roomNumber;
}

static int validateCapacity(int capacity){
    if (capacity <= 0) throw InvalidCapacityException(capacity);
    return capacity;
}

Room::Room(int roomNumber, int capacity): roomNumber(validateRoomNumber(roomNumber)), capacity(validateCapacity(capacity)){}

void Room::setRoomNumber(int roomNumber){
    if (roomNumber<=0) throw InvalidRoomNumberException(roomNumber);
    this->roomNumber = roomNumber;
}

int Room::getRoomNumber() const{
    return roomNumber;
}

void Room::setCapacity(int capacity){
    if (capacity<=0) throw InvalidCapacityException(capacity);
    this->capacity = capacity;
}

int Room::getCapacity() const{
    return capacity;
}

void Room::addStudent(Student* student){
    if (student->getInRoom()) throw StudentAlreadyInRoomException(student->getStudentID());
    for (Student* s: students){
        if (s==student) throw StudentAlreadyInRoomException(student->getStudentID());
    }
    if (isFull()) throw RoomFullException(roomNumber, capacity);
    students.push_back(student);
    student->setInRoom(true);
} 
        
void Room::removeStudent(int studentID){
    for (auto ptr = students.begin(); ptr != students.end(); ptr++){
        if ((*ptr)->getStudentID() == studentID){
            Student* s = *ptr;
            students.erase(ptr); 
            s->setInRoom(false);
            return;
        }
    }
    throw StudentNotInRoomException(studentID);
}
                
void Room::displayStudentsInRoom(){
    for (Student* student: students){
        student->display();
    }
}

bool Room::isFull() const{
    return capacity == students.size();
}

bool Room::isEmpty() const{
    return students.size() == 0;
}

const std::vector<Student*>& Room::getStudents() const {
    return students;
}