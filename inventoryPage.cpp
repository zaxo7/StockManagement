#include <mainwindow.h>
#include "ui_mainwindow.h"


void MainWindow::on_invBuy_clicked()
{
    InventorySelector selector(inventory, BUY);
    selector.exec();
    reloadInventoryPage();
}

void MainWindow::on_invSell_clicked()
{
    InventorySelector selector(inventory, SELL);
    selector.exec();
    reloadInventoryPage();
}


void MainWindow::on_invWasteBasket_clicked()
{
    InventorySelector selector(inventory, DUMP);
    selector.exec();
    reloadInventoryPage();
}

void MainWindow::on_invReproduction_clicked()
{
    InventorySelector selector(inventory, REPRODUCE);
    selector.exec();
    reloadInventoryPage();
}


void MainWindow::reloadInventoryPage()
{
    history->loadToTable(ui->tableHistory);
}

