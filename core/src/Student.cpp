#include "Student.h"
#include <iostream>
using namespace std;

Student::Student(const string& username, const string& password, int studentID, const string& fullName, const string& academicYear, bool inRoom): User(username,password), studentID(studentID), fullName(fullName), academicYear(academicYear), inRoom(inRoom){}

string Student::getRole() const{
    return "Student";
}

void Student::display() const{
    cout<<"Student: "<<fullName<<endl;
    cout<<"ID: "<<studentID<<endl;
    cout<<"Academic Year: "<<academicYear<<endl;
}

void Student::setStudentID(int studentID){
    this->studentID = studentID;
}

int Student::getStudentID() const{
    return studentID;
}

void Student::setFullName(const string& fullName){
    this->fullName = fullName;
}

string Student::getFullName() const{
    return fullName;
}

void Student::setAcademicYear(const string& academicYear){
    this->academicYear = academicYear;
}

string Student::getAcademicYear() const{
    return academicYear;
}

void Student::setInRoom(bool inRoom){
    this->inRoom = inRoom;
}  

bool Student::getInRoom() const{
    return inRoom;
}

// bool Student::operator==(const Student& other) const{
//     return (studentID == other.studentID);
// }