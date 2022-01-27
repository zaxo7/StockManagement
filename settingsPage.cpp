#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_buttonEditProducts_clicked()
{
    ProductEditor productEditor;

    productEditor.exec();
}
