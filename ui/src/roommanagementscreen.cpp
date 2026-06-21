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

void RoomManagementScreen::updateMenuLabels()
{
    if (currentDormitoryIndex < 0) return;
    Dormitory& dorm = university->getDormitories()[currentDormitoryIndex];
    const Restaurant& restaurant = dorm.getRestaurant();

    ui->breakfastMenuLabel->setText("Breakfast: " + QString::fromStdString(restaurant.getBreakFastMenu()));
    ui->lunchMenuLabel->setText("Lunch: " + QString::fromStdString(restaurant.getLunchMenu()));
    ui->dinnerMenuLabel->setText("Dinner: " + QString::fromStdString(restaurant.getDinnerMenu()));
}

void RoomManagementScreen::setUser(User* user)
{
    currentUser = user;
    bool canEdit = (dynamic_cast<Administrator*>(user) != nullptr)
                || (dynamic_cast<SystemAdmin*>(user) != nullptr);

    ui->studentIdLineEdit->setVisible(canEdit);
    ui->assignButton->setVisible(canEdit);
    ui->removeButton->setVisible(canEdit);
}