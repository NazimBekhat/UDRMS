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
        vector<Student*> students; 
    public:
        Room(int roomNumber, int capacity);
        virtual ~Room()=default;

        void setRoomNumber(int roomNumber);
        int getRoomNumber() const;
        void setCapacity(int capcity);
        int getCapacity() const;

        const std::vector<Student*>& getStudents() const;


        void addStudent(Student* student); 
        void removeStudent(int studentID); 
        virtual string getType()const=0;
        void displayStudentsInRoom();
        bool isFull() const;
        bool isEmpty() const;
};

#endif