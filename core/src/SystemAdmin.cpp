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
#include "University.h"
#include "User.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
using std::ofstream;
using std::ifstream;
using std::stringstream;

#include <iostream>
using namespace std;
SystemAdmin::SystemAdmin(const string& username, const string& password, const string& fullName): User(username, password, fullName){}

string SystemAdmin::getRole() const{
    return "SystemAdmin";
};

void SystemAdmin::backup(University& uni, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) throw FileException(filename);

    // 1. University name
    file << "UNIVERSITY|" << uni.getName() << "\n";

    // 2. Users
    for (User* u : uni.getUsers()) {
        Student* s = dynamic_cast<Student*>(u);
        if (s) {
            file << "STUDENT|" << s->getStudentID() << "|"
                 << s->getFullName() << "|"
                 << s->getAcademicYear() << "|"
                 << s->getInRoom() << "\n";
            continue;
        }
        Staff* st = dynamic_cast<Staff*>(u);
        if (st) {
            file << "STAFF|" << st->getStaffID() << "|"
                 << st->getFullName() << "|"
                 << st->getDepartement() << "\n";
            continue;
        }
        Administrator* a = dynamic_cast<Administrator*>(u);
        if (a) {
            file << "ADMIN|" << a->getAdminID() << "|"
                 << a->getFullName() << "\n";
            continue;
        }
        SystemAdmin* sa = dynamic_cast<SystemAdmin*>(u);
        if (sa) {
            file << "SYSADMIN|" << sa->getUsername() << "|"
                 << sa->getFullName() << "\n";
            continue;
        }
    }

    // 3. Dormitories
    for (const Dormitory& dorm : uni.getDormitories()) {
        file << "DORMITORY|" << dorm.getName() << "\n";

        for (Room* r : dorm.getRooms()) {
            file << "ROOM|" << r->getType() << "|"
                 << r->getRoomNumber() << "|"
                 << r->getCapacity();
            for (Student* s : r->getStudents()) {
                file << "|" << s->getStudentID();
            }
            file << "\n";
        }

        const Restaurant& rest = dorm.getRestaurant();
        file << "RESTAURANT|" << rest.getBreakFastMenu() << "|"
             << rest.getLunchMenu() << "|"
             << rest.getDinnerMenu() << "\n";
    }

    file.close();
}

void SystemAdmin::loadData(University& uni, const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) throw FileException(filename);

    uni.clearAll();
    
    string line;
    Dormitory* currentDorm = nullptr;

    while (getline(file, line)){
        stringstream ss(line); 
        string token;
        getline(ss,token, '|');
    
        //UNI
        if (token == "UNIVERSITY"){
            string uniName;
            getline(ss,uniName, '|');
            uni.setName(uniName);
            //If the line started with UNIVERSITY, the next field is the university's name. Read it, set it on the uni object passed in.
        }

        //STUDENT
        else if (token == "STUDENT"){
            string idStr, fullName, yearStr, inRoomStr;
            getline(ss, idStr, '|');
            getline(ss, fullName, '|');
            getline(ss, yearStr, '|');
            getline(ss, inRoomStr, '|');
            Student* s = new Student("", "", stoi(idStr), fullName,yearStr, false);
        //stoi converts "1" to 1
            uni.addUser(s);
        }

        //Staff
        else if (token == "STAFF") {
            std::string idStr, fullName, dept;
            std::getline(ss, idStr, '|');
            std::getline(ss, fullName, '|');
            std::getline(ss, dept, '|');
            Staff* st = new Staff("", "", std::stoi(idStr), dept, fullName);
            st->setFullName(fullName);
            uni.addUser(st);
        }

        //Admin
        else if (token == "ADMIN") {
            std::string idStr, fullName;
            std::getline(ss, idStr, '|');
            std::getline(ss, fullName, '|');
            Administrator* a = new Administrator("", "", std::stoi(idStr), fullName);
            a->setFullName(fullName);
            uni.addUser(a);
        }

        //System admin
        else if (token == "SYSADMIN") {
            std::string username, fullName;
            std::getline(ss, username, '|');
            std::getline(ss, fullName, '|');
            SystemAdmin* sa = new SystemAdmin(username, "", fullName);
            uni.addUser(sa);
        }

        //DORMITORY
        else if (token == "DORMITORY"){
            string dormName;
            getline(ss, dormName, '|');
            uni.addDormitory(Dormitory(dormName));
            currentDorm = &uni.getDormitories().back(); //uni.getDormitories() returns const std::vector<Dormitory>& — the const version you wrote for backup(). .back() on a const vector gives you a const Dormitory&, so &...back() is a const Dormitory*. But currentDorm is declared as Dormitory* (non-const) — mismatch.You need a non-const overload of getDormitories() for loadData() to use, since loadData() needs to modify dormitories (add rooms, set restaurant).

            //back() returns a reference to the last element in the vector. Since we just added this dormitory, it's the last one. & takes its address, storing a pointer in currentDorm.
            //Why we need currentDorm: the next lines in the file (ROOM, RESTAURANT) belong to this dormitory, but the file format doesn't repeat the dormitory name on every line. So we remember "this is the dormitory we're currently filling in" until the next DORMITORY line appears.
        }

        //ROOM
        else if (token == "ROOM" && currentDorm != nullptr){
            string type, roomNumStr, capcacityStr;
            getline(ss, type, '|');
            getline(ss, roomNumStr, '|');
            getline(ss, capcacityStr, '|');

            Room* r = nullptr;
            if (type == "Single Room") r = new SingleRoom(stoi(roomNumStr));
            else if (type == "Double Room") r = new DoubleRoom(stoi(roomNumStr));
            else if (type == "Shared Room") r = new SharedRoom(stoi(roomNumStr), stoi(capcacityStr));
        
            string studentIDStr;
            while (getline(ss, studentIDStr, '|')){
                User* u = uni.findUserByID(stoi(studentIDStr));
                Student* s = dynamic_cast<Student*>(u);
                if (s && r){
                    try{
                        r->addStudent(s);
                    } catch (const UDRMSException& e){
                        std::cerr << "Skipping student " << studentIDStr << ": " << e.what()<<endl;
                    }
                    
                }
            }

            if (r) currentDorm->addRoom(r);
        }

        //RESTAURANT
        else if (token == "RESTAURANT" && currentDorm != nullptr) {
            string breakfast, lunch, dinner;
            getline(ss, breakfast, '|');
            getline(ss, lunch, '|');
            getline(ss, dinner, '|');

            currentDorm->getRestaurant().setBreakFastMenu(breakfast);
            currentDorm->getRestaurant().setLunchMenu(lunch);
            currentDorm->getRestaurant().setDinnerMenu(dinner);
        }
    }
    
    file.close();
}
