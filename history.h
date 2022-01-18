#ifndef HISTORY_H
#define HISTORY_H


#include <QObject>
#include <QTableWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include "stock.h"


class History : public QObject
{
    Q_OBJECT
public:
    explicit History(QObject *parent = nullptr);

    void log(InventoryAction action, Stock* stock);

    void load();

    void loadToTable(QTableWidget *table);

    const QVector<Stock *> &getStocks() const;

    const QStringList &getActions() const;

private:
    QVector<Stock*> stocks;
    QVector<InventoryAction> invActions;
    QStringList actions;

    int totalQty;

    float totalSell;

    float totalBuy;

    float totalDump;

    float totalReproduce;


    DatabaseTable history;

signals:

};

#endif // HISTORY_H
