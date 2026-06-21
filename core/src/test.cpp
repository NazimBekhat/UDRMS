// #include "Administrator.h"
// #include "Dormitory.h"
// #include "DoubleRoom.h"
// #include "Restaurant.h"
// #include "Room.h"
// #include "SharedRoom.h"
// #include "SingleRoom.h"
// #include "Staff.h"
// #include "Student.h"
// #include "SystemAdmin.h"
// #include "User.h"
// #include "University.h"
// #include "Exceptions.h"

// #include <iostream>
// using namespace std;

// int main(){
//     Restaurant restau1;
//     restau1.displayMenu();

//     Restaurant restau2("Eggs","Steack","Orange");
//     restau2.displayMenu();

//     Dormitory dorm1("Dorm1");
//     dorm1.addRoom(new SingleRoom(1));
//     dorm1.addRoom(new DoubleRoom(2));
//     dorm1.addRoom(new SharedRoom(3,3));
//     dorm1.addRoom(new SingleRoom(1));//duplicate of 1
//     cout<<"Room 1 of dorm 1: "<<dorm1.getRoom(1)<<endl;
//     cout<<"Room 4 of dorm 1: "<<dorm1.getRoom(4)<<endl;

// //Student(const string& username, const string& password, int studentID, const string& fullName, const string& academicYear);

//     Student* s1 = new Student("nazim_bekhat","nazim",1,"Nazim Bekhat","25/26");
//     Student* s2 = new Student("abdou_bekhat","abdou",2,"Abdou Bekhat","24/25");
//     dorm1.addStudentToRoom(s1,dorm1.getRoom(1));
//     dorm1.addStudentToRoom(s2,dorm1.getRoom(1)); //should fail
//     dorm1.getRoom(1)->displayStudentsInRoom(); // should show only s1

//     // dorm1.addStudentToRoom(s1,dorm1.getRoom(2));
//     // dorm1.addStudentToRoom(s2,dorm1.getRoom(2)); //will work
//     try {
//         dorm1.addStudentToRoom(s1,dorm1.getRoom(2)); //should fail — s1 still in Room 1
//     } catch (const UDRMSException& e) {
//         cout << "Expected failure: " << e.what() << endl;
//     }
//     dorm1.addStudentToRoom(s2,dorm1.getRoom(2)); //will work — s2 not in any room yet

//     dorm1.getRoom(2)->displayStudentsInRoom(); // should show s1 and s2
    
//     dorm1.removeRoom(1); //should fail
//     cout << "Room 1 exists: " << (dorm1.getRoom(1) == nullptr ? "no" : "yes") << endl; // still exists → occupied
//     dorm1.removeRoom(3);
//     cout << "Room 3 exists: " << (dorm1.getRoom(3) == nullptr ? "no" : "yes") << endl; // removed → nullptr
    
//     // dorm1.getRoom(1)->removeStudent(1); // s1 leaves Room 1, inRoom → false
//     // dorm1.addStudentToRoom(s1, dorm1.getRoom(2)); // now should succeed
//     // dorm1.getRoom(2)->displayStudentsInRoom(); // should show s1 and s2

//     try {
//         dorm1.addStudentToRoom(s1,dorm1.getRoom(1));
//     } catch (const UDRMSException& e) {
//         cout << "Unexpected failure: " << e.what() << endl;
//     }

//     try {
//         dorm1.addStudentToRoom(s2,dorm1.getRoom(1)); //should fail
//     } catch (const UDRMSException& e) {
//         cout << "Expected failure: " << e.what() << endl;
//     }
//     dorm1.getRoom(1)->displayStudentsInRoom(); // should show only s1

//     try {
//         dorm1.addStudentToRoom(s1,dorm1.getRoom(2)); //should fail now — s1 still in Room 1
//     } catch (const UDRMSException& e) {
//         cout << "Expected failure: " << e.what() << endl;
//     }

//     try {
//         dorm1.addStudentToRoom(s2,dorm1.getRoom(2)); //should fail — s2 already in Room 2
//     } catch (const UDRMSException& e) {
//         cout << "Expected failure: " << e.what() << endl;
//     }

//     dorm1.getRoom(2)->displayStudentsInRoom(); // should show only s2 at this point

//     cout << "\n--- University + File Handling Test ---\n";

//     University uni1("ENSIA");

//     Staff* staff1 = new Staff("john_doe", "pass", 10, "Maintenance","John Foe");
//     Administrator* admin1 = new Administrator("jane_doe", "pass", 20, "Jane Doe");
//     SystemAdmin* sysadmin1 = new SystemAdmin("root", "pass", "Root User");

//     uni1.addUser(s1);
//     uni1.addUser(s2);
//     uni1.addUser(staff1);
//     uni1.addUser(admin1);
//     uni1.addUser(sysadmin1);

//     dorm1.getRestaurant().setBreakFastMenu("Eggs");
//     dorm1.getRestaurant().setLunchMenu("Steack");
//     dorm1.getRestaurant().setDinnerMenu("Orange");

//     uni1.addDormitory(move(dorm1));

//     try {
//         sysadmin1->backup(uni1, "../../data/test_backup.txt");
//         cout << "Backup written.\n";
//     } catch (const UDRMSException& e) {
//         cout << "Backup failed: " << e.what() << endl;
//     }

//     University uni2("Placeholder");

//     try {
//         sysadmin1->loadData(uni2, "../../data/test_backup.txt");
//     } catch (const UDRMSException& e) {
//         cout << "Load failed: " << e.what() << endl;
//     }

//     cout<<"Loaded university name: "<<uni2.getName()<<endl;
//     cout<<"Loaded user count: "<<uni2.getUsers().size()<<endl;
//     cout<<"Loaded dormitory count: "<<uni2.getDormitories().size()<<endl;

//     for (User* u: uni2.getUsers()){
//         cout<<u->getRole()<<" - "<<u->getFullName()<<endl;
//     }

//     for (const Dormitory& d: uni2.getDormitories()){
//         cout<<"Dormitory: "<<d.getName()<<", rooms: "<<d.getRooms().size()<<endl;
//         for (Room* r: d.getRooms()){
//             cout<<" Room "<<r->getRoomNumber()<<" ("<<r->getType()<<"), students: "<<r->getStudents().size()<<endl;
//         }
//         cout<<" Restaurant: "<<d.getRestaurant().getBreakFastMenu()<<", "
//         <<d.getRestaurant().getLunchMenu()<<", "<<d.getRestaurant().getDinnerMenu()<<endl;
//     }

//     return 0;
// }