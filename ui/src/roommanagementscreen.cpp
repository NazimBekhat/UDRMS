#include "roommanagementscreen.h"
#include "ui_roommanagementscreen.h"
#include "University.h"
#include "Dormitory.h"
#include "Room.h"
#include "Student.h"
#include "User.h"
#include "Exceptions.h"
#include "Restaurant.h"
#include "SystemAdmin.h"
#include "Administrator.h"
#include "Staff.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "SharedRoom.h"
#include <QInputDialog>
#include <QMessageBox>

RoomManagementScreen::RoomManagementScreen(University* university, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RoomManagementScreen)
    , university(university)
    , currentDormitoryIndex(-1)
    , currentRoom(nullptr)
    , currentUser(nullptr)
{
    ui->setupUi(this);

    connect(ui->dormitoryComboBox, &QComboBox::currentIndexChanged, this, &RoomManagementScreen::onDormitoryChanged);
    connect(ui->roomsListWidget, &QListWidget::currentRowChanged, this, &RoomManagementScreen::onRoomSelected);
    connect(ui->assignButton, &QPushButton::clicked, this, &RoomManagementScreen::onAssignClicked);
    connect(ui->removeButton, &QPushButton::clicked, this, &RoomManagementScreen::onRemoveClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &RoomManagementScreen::requestBack);
    connect(ui->saveMenuButton, &QPushButton::clicked, this, &RoomManagementScreen::onSaveMenuClicked);
    connect(ui->addDormitoryButton, &QPushButton::clicked, this, &RoomManagementScreen::onAddDormitoryClicked);
    connect(ui->addRoomButton, &QPushButton::clicked, this, &RoomManagementScreen::onAddRoomClicked);
}

RoomManagementScreen::~RoomManagementScreen() { delete ui; }

void RoomManagementScreen::refresh()
{
    populateDormitoryList();
}

void RoomManagementScreen::populateDormitoryList()
{
    ui->dormitoryComboBox->clear();
    for (const Dormitory& d : university->getDormitories()) {
        ui->dormitoryComboBox->addItem(QString::fromStdString(d.getName()));
    }
    if (!university->getDormitories().empty()) {
        currentDormitoryIndex = 0;
        populateRoomList();
        updateMenuLabels();
    }
}

void RoomManagementScreen::populateRoomList()
{
    ui->roomsListWidget->clear();
    currentRoom = nullptr;

    if (currentDormitoryIndex < 0) return;
    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];

    for (Room* room : dorm.getRooms()) {
        QString label = QString("Room %1 (%2) - %3/%4")
            .arg(room->getRoomNumber()) //in %1
            .arg(QString::fromStdString(room->getType())) //int 2
            .arg(room->getStudents().size()) // %3
            .arg(room->getCapacity()); // %4, all based on what the functions return
        ui->roomsListWidget->addItem(label);
    }
}

void RoomManagementScreen::onDormitoryChanged(int index)
{
    currentDormitoryIndex = index;
    populateRoomList();
     updateMenuLabels();
}

void RoomManagementScreen::onRoomSelected(int row)
{
    if (row < 0 || currentDormitoryIndex < 0) { currentRoom = nullptr; return; }
    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
    currentRoom = dorm.getRooms()[row];
}

void RoomManagementScreen::onAssignClicked()
{
    if (!(dynamic_cast<Administrator*>(currentUser) || dynamic_cast<SystemAdmin*>(currentUser))) {
        QMessageBox::warning(this, "Permission Denied", "You don't have permission to do this.");
        return;
    }

    if (currentRoom == nullptr) {
        QMessageBox::warning(this, "No Room Selected", "Please select a room first.");
        return;
    }
    bool ok = false;
    int studentID = ui->studentIdLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Enter a numeric student ID.");
        return;
    }

    User* user = university->findUserByID(studentID);
    Student* student = dynamic_cast<Student*>(user);
    if (student == nullptr) {
        QMessageBox::warning(this, "Not Found", "No student with that ID.");
        return;
    }

    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
    try {
        dorm.addStudentToRoom(student, currentRoom);
        QMessageBox::information(this, "Success", "Student assigned.");
        populateRoomList();
    } catch (const UDRMSException& e) {
        QMessageBox::warning(this, "Assignment Failed", e.what());
    }
}

void RoomManagementScreen::onRemoveClicked()
{
    if (!(dynamic_cast<Administrator*>(currentUser) || dynamic_cast<SystemAdmin*>(currentUser))) {
        QMessageBox::warning(this, "Permission Denied", "You don't have permission to do this.");
        return;
    }
    
    if (currentRoom == nullptr) {
        QMessageBox::warning(this, "No Room Selected", "Please select a room first.");
        return;
    }
    bool ok = false;
    int studentID = ui->studentIdLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "Enter a numeric student ID.");
        return;
    }

    try {
        currentRoom->removeStudent(studentID);
        QMessageBox::information(this, "Success", "Student removed.");
        populateRoomList();
    } catch (const UDRMSException& e) {
        QMessageBox::warning(this, "Removal Failed", e.what());
    }
}

void RoomManagementScreen::setUser(User* user)
{
    currentUser = user;
    bool canEditRooms = (dynamic_cast<Administrator*>(user) != nullptr)
                      || (dynamic_cast<SystemAdmin*>(user) != nullptr);
    bool canEditTheMenu = canEditMenu();

    ui->studentIdLineEdit->setVisible(canEditRooms);
    ui->assignButton->setVisible(canEditRooms);
    ui->removeButton->setVisible(canEditRooms);

    ui->breakfastMenuLineEdit->setReadOnly(!canEditTheMenu);
    ui->lunchMenuLineEdit->setReadOnly(!canEditTheMenu);
    ui->dinnerMenuLineEdit->setReadOnly(!canEditTheMenu);
    ui->saveMenuButton->setVisible(canEditTheMenu);
}

bool RoomManagementScreen::canEditMenu() const
{
    return (dynamic_cast<Staff*>(currentUser) != nullptr)
        || (dynamic_cast<Administrator*>(currentUser) != nullptr)
        || (dynamic_cast<SystemAdmin*>(currentUser) != nullptr);
}

void RoomManagementScreen::updateMenuLabels()
{
    if (currentDormitoryIndex < 0) return;
    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
    const Restaurant& restaurant = dorm.getRestaurant();

    ui->breakfastMenuLineEdit->setText(QString::fromStdString(restaurant.getBreakFastMenu()));
    ui->lunchMenuLineEdit->setText(QString::fromStdString(restaurant.getLunchMenu()));
    ui->dinnerMenuLineEdit->setText(QString::fromStdString(restaurant.getDinnerMenu()));
}

void RoomManagementScreen::onSaveMenuClicked()
{
    if (!canEditMenu()) {
        QMessageBox::warning(this, "Permission Denied", "You don't have permission to edit the menu.");
        return;
    }
    if (currentDormitoryIndex < 0) return;

    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
    Restaurant& restaurant = dorm.getRestaurant();

    restaurant.setBreakFastMenu(ui->breakfastMenuLineEdit->text().toStdString());
    restaurant.setLunchMenu(ui->lunchMenuLineEdit->text().toStdString());
    restaurant.setDinnerMenu(ui->dinnerMenuLineEdit->text().toStdString());

    QMessageBox::information(this, "Menu Updated", "Restaurant menu saved.");
}

void RoomManagementScreen::onAddDormitoryClicked()
{
    if (!(dynamic_cast<Administrator*>(currentUser) || dynamic_cast<SystemAdmin*>(currentUser))) {
        QMessageBox::warning(this, "Permission Denied", "You don't have permission to do this.");
        return;
    }

    bool ok = false;
    QString name = QInputDialog::getText(this, "Add Dormitory", "Dormitory name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.trimmed().isEmpty()) return;

    university->addDormitory(Dormitory(name.toStdString()));
    populateDormitoryList();
    QMessageBox::information(this, "Success", "Dormitory added.");
}

void RoomManagementScreen::onAddRoomClicked()
{
    if (!(dynamic_cast<Administrator*>(currentUser) || dynamic_cast<SystemAdmin*>(currentUser))) {
        QMessageBox::warning(this, "Permission Denied", "You don't have permission to do this.");
        return;
    }
    if (currentDormitoryIndex < 0) {
        QMessageBox::warning(this, "No Dormitory Selected", "Please select a dormitory first.");
        return;
    }

    QStringList types = {"Single Room", "Double Room", "Shared Room"};
    bool ok = false;
    QString type = QInputDialog::getItem(this, "Add Room", "Room type:", types, 0, false, &ok);
    if (!ok) return;

    bool numOk = false;
    int roomNumber = QInputDialog::getInt(this, "Add Room", "Room number:", 1, 1, 9999, 1, &numOk);
    if (!numOk) return;

    Room* newRoom = nullptr;
    try {
        if (type == "Single Room") newRoom = new SingleRoom(roomNumber);
        else if (type == "Double Room") newRoom = new DoubleRoom(roomNumber);
        else if (type == "Shared Room") {
            bool capOk = false;
            int capacity = QInputDialog::getInt(this, "Add Room", "Capacity (3-4):", 3, 3, 4, 1, &capOk);
            if (!capOk) return;
            newRoom = new SharedRoom(roomNumber, capacity);
        }

        Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
        dorm.addRoom(newRoom);
        populateRoomList();
        QMessageBox::information(this, "Success", "Room added.");
    } catch (const UDRMSException& e) {
        delete newRoom;   // avoid leak if addRoom throws or construction throws after allocation
        QMessageBox::warning(this, "Add Room Failed", e.what());
    }
}