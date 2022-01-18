#ifndef INVENTORY_H
#define INVENTORY_H

#include <QObject>
#include <QTableWidget>
#include <QMessageBox>
#include "stock.h"
#include "history.h"

class Inventory : public QObject
{
    Q_OBJECT
public:
    explicit Inventory(QObject *parent = nullptr);

    void load();
    void insert();
    void update();
    void deletep();

    int buy(Stock *stock);
    int sell(Stock *stock);
    int dump(Stock *stock);
    int reproduce(Stock *stock);


    void store();

    void loadFromRecord(QSqlRecord rec);

    void loadToTable(QTableWidget *table);

    int findProduct(int id);

    const QVector<Stock *> &getStocks() const;
    void setStocks(const QVector<Stock *> &newStocks);

    int getTotalQty() const;
    void setTotalQty(int newTotalQty);

    float getTotalPrice() const;
    void setTotalPrice(float newTotalPrice);

    History *getHistory() const;
    void setHistory(History *newHistory);

private:

    QVector<Stock*> stocks;

    int totalQty;

    float totalPrice;

    History *history;

    DatabaseTable inventory;

signals:

};

#endif // INVENTORY_H
