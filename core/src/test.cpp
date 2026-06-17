#include "Administrator.h"
#include "Dormitory.h"
#include "DoubleRoom.h"
#include "Restaurant.h"
#include "Room.h"
#include "SharedRoom.h"
#include "SingleRoom.h"
#include "Staff.h"
#include "Student.h"
#include "SystemAdmin.h"
#include "User.h"

#include <iostream>
using namespace std;

int main(){
    Restaurant restau1;
    restau1.displayMenu();

    Restaurant restau2("Eggs","Steack","Orange");
    restau2.displayMenu();

    Dormitory dorm1("Dorm1");
    dorm1.addRoom(new SingleRoom(1));
    dorm1.addRoom(new DoubleRoom(2));
    dorm1.addRoom(new SharedRoom(3,3));
    dorm1.addRoom(new SingleRoom(1));//duplicate of 1
    cout<<"Room 1 of dorm 1: "<<dorm1.getRoom(1)<<endl;
    cout<<"Room 4 of dorm 1: "<<dorm1.getRoom(4)<<endl;

//Student(const string& username, const string& password, int studentID, const string& fullName, const string& academicYear);

    Student s1("nazim_bekhat","nazim",1,"Nazim Bekhat","25/26"), s2("abdou_bekhat","abdou",1,"Abdou Bekhat","24/25");
    dorm1.addStudentToRoom(&s1,dorm1.getRoom(1));
    dorm1.addStudentToRoom(&s2,dorm1.getRoom(1)); //should fail
    dorm1.getRoom(1)->displayStudentsInRoom(); // should show only s1

    dorm1.addStudentToRoom(&s1,dorm1.getRoom(2));
    dorm1.addStudentToRoom(&s2,dorm1.getRoom(2)); //will work

    dorm1.getRoom(2)->displayStudentsInRoom(); // should show s1 and s2
    
    dorm1.removeRoom(1); //should fail
    cout << "Room 1 exists: " << (dorm1.getRoom(1) == nullptr ? "no" : "yes") << endl; // still exists → occupied
    dorm1.removeRoom(3);
    cout << "Room 3 exists: " << (dorm1.getRoom(3) == nullptr ? "no" : "yes") << endl; // removed → nullptr
    


    return 0;
}


