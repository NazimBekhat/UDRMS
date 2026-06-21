#include "dashboardscreen.h"
#include "ui_dashboardscreen.h"
#include "University.h"
#include "User.h"
#include "Administrator.h"
#include "SystemAdmin.h"
#include "Exceptions.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QDir>

// Add this helper function near the top of the file, outside the class methods
static QString getBackupFilePath()
{
    QString dirPath = QCoreApplication::applicationDirPath() + "/data";
    QDir dir(dirPath);
    if (!dir.exists()) {
        dir.mkpath(".");   // creates the folder if it doesn't exist yet
    }
    return dirPath + "/backup.txt";
}

DashboardScreen::DashboardScreen(University* university, QWidget *parent)
    : QWidget(parent), ui(new Ui::DashboardScreen), university(university), currentUser(nullptr)
{
    ui->setupUi(this);
    connect(ui->roomManagementButton, &QPushButton::clicked, this, &DashboardScreen::requestRoomManagement);
    connect(ui->restaurantMenuButton, &QPushButton::clicked, this, &DashboardScreen::requestRestaurantMenu);
    connect(ui->backupButton, &QPushButton::clicked, this, &DashboardScreen::onBackupClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &DashboardScreen::onLoadClicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &DashboardScreen::requestLogout); 
    connect(ui->createUserButton, &QPushButton::clicked, this, &DashboardScreen::requestCreateUser);
}

DashboardScreen::~DashboardScreen()
 { delete ui; }

void DashboardScreen::setUser(User* user)
{
    currentUser = user;
    QString text = QString::fromStdString(user->getFullName()) + " (" + QString::fromStdString(user->getRole()) + ")";
    ui->welcomeLabel->setText(text);

    bool isSysAdmin = (dynamic_cast<SystemAdmin*>(user) != nullptr);
    ui->backupButton->setVisible(isSysAdmin);
    ui->loadButton->setVisible(isSysAdmin);

    bool canCreateUsers = isSysAdmin || (dynamic_cast<Administrator*>(user) != nullptr);
    ui->createUserButton->setVisible(canCreateUsers);
}

void DashboardScreen::onBackupClicked()
{
    SystemAdmin* admin = dynamic_cast<SystemAdmin*>(currentUser);
    try {
        admin->backup(*university, getBackupFilePath().toStdString());
        QMessageBox::information(this, "Backup", "Backup saved successfully.");
    } catch (const UDRMSException& e) {
        QMessageBox::warning(this, "Backup Failed", e.what());
    }
}

void DashboardScreen::onLoadClicked()
{
    SystemAdmin* admin = dynamic_cast<SystemAdmin*>(currentUser);
    try {
        admin->loadData(*university, getBackupFilePath().toStdString());
        QMessageBox::information(this, "Load", "Data loaded successfully.");
    } catch (const UDRMSException& e) {
        QMessageBox::warning(this, "Load Failed", e.what());
    }
}