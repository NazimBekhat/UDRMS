#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "University.h"
#include "User.h"
#include "loginscreen.h"
#include "dashboardscreen.h"

MainWindow::MainWindow(University* university, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , university(university)
{
    ui->setupUi(this);

    loginScreen = new LoginScreen(university, this);
    dashboardScreen = new DashboardScreen(university, this);
    ui->stackedWidget->addWidget(loginScreen);
    ui->stackedWidget->addWidget(dashboardScreen);

    ui->stackedWidget->setCurrentWidget(loginScreen);

    connect(loginScreen, &LoginScreen::loginSucceeded, this, &MainWindow::onLoginSucceeded);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onLoginSucceeded(User* user)
{
    dashboardScreen->setUser(user);
    ui->stackedWidget->setCurrentWidget(dashboardScreen);
}