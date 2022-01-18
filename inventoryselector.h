#ifndef INVENTORYSELECTOR_H
#define INVENTORYSELECTOR_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include <databasetable.h>
#include "product.h"
#include "inventory.h"

namespace Ui {
class InventorySelector;
}

class InventorySelector : public QDialog
{
    Q_OBJECT

public:
    explicit InventorySelector(Inventory *inv, InventoryAction action, QWidget *parent = nullptr);
    ~InventorySelector();

    void setIcon();

    int exec();

    void reloadList(bool reset = true);

    void StockToUI(Inventory inv);

    Stock* UIToStock();

private slots:
    void on_listViewProducts_clicked(const QModelIndex &index);

    void on_lineEditSearch_textChanged(const QString &arg1);

    void on_pushButtonValidate_clicked();

private:
    Ui::InventorySelector *ui;

    QVector<Product*> products;

    Product *currentProduct;
    int selectedRow;

    Inventory *inventory;

    InventoryAction action;

    QByteArray currentLogo;
};

#endif // INVENTORYSELECTOR_H
