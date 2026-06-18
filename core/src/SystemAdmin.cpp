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
#include <fstream>
using std::ofstream;
using std::ifstream;

SystemAdmin::SystemAdmin(const string& username, const string& password, const string& fullName): User(username, password, fullName){}

string SystemAdmin::getRole() const{
    return "SystemAdmin";
};

void SystemAdmin::backup(University& uni, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

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

void SystemAdmin::loadData(University& uni, const string& filename){
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    Dormitory* currentDorm = nullptr;

    while (getline(file,line)){

    }
    
}

