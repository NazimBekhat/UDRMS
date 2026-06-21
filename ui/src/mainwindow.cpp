#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "University.h"
#include "User.h"
#include "loginscreen.h"
#include "dashboardscreen.h"
#include "roommanagementscreen.h"
#include "createuserscreen.h"

MainWindow::MainWindow(University* university, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , university(university)
{
    ui->setupUi(this);

    loginScreen = new LoginScreen(university, this);
    dashboardScreen = new DashboardScreen(university, this);
    roomManagementScreen = new RoomManagementScreen(university, this);
    createUserScreen = new CreateUserScreen(university, this); 

    ui->stackedWidget->addWidget(loginScreen);
    ui->stackedWidget->addWidget(dashboardScreen);
    ui->stackedWidget->addWidget(roomManagementScreen);
    ui->stackedWidget->addWidget(createUserScreen);

    ui->stackedWidget->setCurrentWidget(loginScreen);

    connect(loginScreen, &LoginScreen::loginSucceeded, this, &MainWindow::onLoginSucceeded);
    connect(dashboardScreen, &DashboardScreen::requestRoomManagement, this, &MainWindow::onRequestRoomManagement);
    connect(dashboardScreen, &DashboardScreen::requestRestaurantMenu, this, &MainWindow::onRequestRoomManagement);
    connect(roomManagementScreen, &RoomManagementScreen::requestBack, this, &MainWindow::onRequestBackToDashboard);
    connect(dashboardScreen, &DashboardScreen::requestLogout, this, &MainWindow::onLogout);
    connect(dashboardScreen, &DashboardScreen::requestCreateUser, this, &MainWindow::onRequestCreateUser);
    connect(createUserScreen, &CreateUserScreen::requestBack, this, &MainWindow::onRequestBackToDashboard);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onLoginSucceeded(User* user)
{
    loggedInUser = user;  
    dashboardScreen->setUser(user);
    roomManagementScreen->setUser(user);
    ui->stackedWidget->setCurrentWidget(dashboardScreen);
}

void MainWindow::onRequestCreateUser()
{
    createUserScreen->setUser(loggedInUser); 
    ui->stackedWidget->setCurrentWidget(createUserScreen);
}

void MainWindow::onRequestRoomManagement()
{
    roomManagementScreen->refresh(); 
    ui->stackedWidget->setCurrentWidget(roomManagementScreen);
}

void MainWindow::onRequestBackToDashboard()
{
    ui->stackedWidget->setCurrentWidget(dashboardScreen);
}

void MainWindow::onLogout()
{
    loginScreen->clearFields();
    ui->stackedWidget->setCurrentWidget(loginScreen);
}