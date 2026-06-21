#include "mainwindow.h"
#include "University.h"
#include "Student.h"
#include "Staff.h"
#include "Administrator.h"
#include "SystemAdmin.h"
#include "Dormitory.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "SharedRoom.h"
#include "Restaurant.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    University university("ENSIA");
    university.addUser(new Student("nazim_bekhat", "nazim", 1, "Nazim Bekhat", "25/26"));
    university.addUser(new Staff("john_doe", "pass", 10, "Maintenance", "John Doe"));
    university.addUser(new Administrator("jane_doe", "pass", 20, "Jane Doe"));
    university.addUser(new SystemAdmin("root", "pass", "Root User"));

    Dormitory dorm1("Dorm A");
    dorm1.addRoom(new SingleRoom(1));
    dorm1.addRoom(new DoubleRoom(2));
    dorm1.addRoom(new SharedRoom(3, 3));
    dorm1.getRestaurant().setBreakFastMenu("Eggs & Toast");
    dorm1.getRestaurant().setLunchMenu("Grilled Chicken");
    dorm1.getRestaurant().setDinnerMenu("Pasta");
    university.addDormitory(std::move(dorm1));

    Dormitory dorm2("Dorm B");
    dorm2.addRoom(new SingleRoom(1));
    dorm2.addRoom(new SharedRoom(2, 4));
    dorm2.getRestaurant().setBreakFastMenu("Pancakes");
    dorm2.getRestaurant().setLunchMenu("Beef Stew");
    dorm2.getRestaurant().setDinnerMenu("Soup & Bread");
    university.addDormitory(std::move(dorm2));

    MainWindow w(&university);
    w.show();
    return a.exec();
}