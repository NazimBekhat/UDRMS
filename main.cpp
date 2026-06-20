#include "mainwindow.h"
#include "University.h"
#include "Student.h"
#include "Staff.h"
#include "Administrator.h"
#include "SystemAdmin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    University university("ENSIA");
    university.addUser(new Student("nazim_bekhat", "nazim", 1, "Nazim Bekhat", "25/26"));
    university.addUser(new Staff("john_doe", "pass", 10, "Maintenance", "John Doe"));
    university.addUser(new Administrator("jane_doe", "pass", 20, "Jane Doe"));
    university.addUser(new SystemAdmin("root", "pass", "Root User"));

    MainWindow w(&university);
    w.show();
    return a.exec();
}

