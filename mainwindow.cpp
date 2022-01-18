#include "mainwindow.h"
#include "ui_mainwindow.h"

Database *db = NULL;

MainWindow* MainWindow::self = NULL;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    self = this;

    //fire the database
    db = Database::getInstance();

    inventory = new Inventory();

    history = new History();

    inventory->setHistory(history);

    //load the default page
    reloadHomePage();
    reloadInventoryPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    return self;
}



void MainWindow::on_actionDashboard_triggered()
{

    ui->stackedPages->setCurrentWidget(ui->homePage);
}


void MainWindow::on_actionInventory_triggered()
{
    ui->stackedPages->setCurrentWidget(ui->inventoryPage);
}


void MainWindow::on_actionParameters_triggered()
{
    ui->stackedPages->setCurrentWidget(ui->settingsPage);
}



void MainWindow::on_stackedPages_currentChanged(int arg1)
{
    if(ui->stackedPages->currentWidget()->objectName() == "homePage")
    {
        reloadHomePage();
    }
    else if(ui->stackedPages->currentWidget()->objectName() == "inventoryPage")
    {
        reloadInventoryPage();
    }
    else if(ui->stackedPages->currentWidget()->objectName() == "settingsPage")
    {

    }
}



