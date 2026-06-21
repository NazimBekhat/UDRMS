#include "createuserscreen.h"
#include "ui_createuserscreen.h"
#include "University.h"
#include "User.h"
#include "Student.h"
#include "Staff.h"
#include "Administrator.h"
#include "SystemAdmin.h"
#include <QMessageBox>

CreateUserScreen::CreateUserScreen(University* university, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateUserScreen)
    , university(university)
    , currentUser(nullptr)
{
    ui->setupUi(this);
    connect(ui->roleComboBox, &QComboBox::currentIndexChanged, this, &CreateUserScreen::onRoleChanged);
    connect(ui->createButton, &QPushButton::clicked, this, &CreateUserScreen::onCreateClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &CreateUserScreen::requestBack);
}

CreateUserScreen::~CreateUserScreen() { delete ui; }

void CreateUserScreen::setUser(User* user)
{
    currentUser = user;
    ui->roleComboBox->clear();

    if (dynamic_cast<SystemAdmin*>(user) != nullptr) {
        ui->roleComboBox->addItem("Administrator");
        ui->roleComboBox->addItem("Staff");
        ui->roleComboBox->addItem("Student");
    } else if (dynamic_cast<Administrator*>(user) != nullptr) {
        ui->roleComboBox->addItem("Staff");
        ui->roleComboBox->addItem("Student");
    }
    // Staff/Student: combo box stays empty since they have no roles in creating users

    onRoleChanged(0);
}

void CreateUserScreen::onRoleChanged(int index)
{
    QString role = ui->roleComboBox->currentText();
    bool needsExtraField = (role == "Student" || role == "Staff");
    ui->extraFieldLineEdit->setVisible(needsExtraField);
}

void CreateUserScreen::onCreateClicked()
{
    QString role = ui->roleComboBox->currentText();
    std::string username = ui->usernameLineEdit->text().toStdString();
    std::string password = ui->passwordLineEdit->text().toStdString();
    std::string fullName = ui->fullNameLineEdit->text().toStdString();

    if (username.empty() || password.empty() || fullName.empty()) {
        QMessageBox::warning(this, "Missing Fields", "Please fill in all required fields.");
        return;
    }

    bool ok = false;
    int id = ui->idLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Invalid Input", "ID must be a number.");
        return;
    }

    if (university->findUserByUsername(username) != nullptr) {
        QMessageBox::warning(this, "Username Taken", "That username is already in use.");
        return;
    }

    User* newUser = nullptr;

    if (role == "Student") {
        std::string academicYear = ui->extraFieldLineEdit->text().toStdString();
        newUser = new Student(username, password, id, fullName, academicYear);
    } else if (role == "Staff") {
        std::string department = ui->extraFieldLineEdit->text().toStdString();
        newUser = new Staff(username, password, id, department, fullName);
    } else if (role == "Administrator") {
        newUser = new Administrator(username, password, id, fullName);
    }

    if (newUser == nullptr) {
        QMessageBox::warning(this, "Error", "No role selected or insufficient permission.");
        return;
    }

    university->addUser(newUser);
    QMessageBox::information(this, "Success", "User created successfully.");

    ui->usernameLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->fullNameLineEdit->clear();
    ui->idLineEdit->clear();
    ui->extraFieldLineEdit->clear();
}