#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::reloadHomePage()
{
    inventory->loadToTable(ui->tableActualStock);
}

void MainWindow::on_tableActualStock_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    //load the image
    if(currentRow >= 0 && (currentRow < inventory->getStocks().count()))
    {
        inventory->getStocks().at(currentRow)->getProduct()->loadLogoToLabel(ui->labelLogo);
    }
}

