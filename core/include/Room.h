#ifndef ROOM_H
#define ROOM_H

#include <vector>
using  std::vector;
#include <string>
using  std::string;

class Student;

class Room{
    private:
        int roomNumber;
        int capacity;
        vector<Student*> students; //use by ref since if we use by value then the room owns the students.
    public:
        Room(int roomNumber, int capacity);
        virtual ~Room()=default;

        void setRoomNumber(int roomNumber);
        int getRoomNumber() const;
        void setCapacity(int capcity);
        int getCapacity() const;


        void addStudent(Student* student); //we add one student at a time then we pass 1 student
        void removeStudent(int studentID); //remove by student ID to apply operator overloading
        virtual string getType()=0;
        void displayStudentsInRoom();
        bool isFull();
};

#endif