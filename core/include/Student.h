#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using std::string;
#include "User.h"

class Student: public User{
    private:
        int studentID;
        string fullName;
        string academicYear;
    public:
        Student(const string& username, const string& password, int studentID, const string& fullName, const string& academicYear);
        
        virtual string getRole() const override;

        void display() const;

        void setStudentID(int studentID);
        int getStudentID() const;

        void setFullName(const string& fullName);
        string getFullName() const;

        void setAcademicYear(const string& academicYear);
        string getAcademicYear() const;
};

#endif