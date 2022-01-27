#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inventory.h"
#include "producteditor.h"
#include "inventoryselector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* getInstance();

    //HomePage functions
    void reloadHomePage();

    //InventoryPage functions
    void reloadInventoryPage();

    //SettingsPage functions
    void reloadSettingsPage();

private slots:
    void on_actionDashboard_triggered();

    void on_actionInventory_triggered();

    void on_actionParameters_triggered();

    void on_invBuy_clicked();

    void on_buttonEditProducts_clicked();

    void on_stackedPages_currentChanged(int arg1);

    void on_tableActualStock_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_invSell_clicked();

    void on_invWasteBasket_clicked();

    void on_invReproduction_clicked();

    void on_buttonEmptyDatabase_clicked();

private:
    Ui::MainWindow *ui;

    static MainWindow *self;

    Inventory *inventory;
    History *history;
};
#endif // MAINWINDOW_H
