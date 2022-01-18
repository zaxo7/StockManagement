#include "history.h"

History::History(QObject *parent)
    : QObject{parent}, history("History")
{
    totalQty = 0;

    totalBuy = 0.0;
    totalSell= 0.0;
    totalDump= 0.0;
    totalReproduce = 0.0;

    //add the columns
    history.addColumn("action");
    history.addColumn("productId");
    history.addColumn("stockQty");
    history.addColumn("stockPrice");
    history.addColumn("client");
    history.addColumn("lot");
    history.addColumn("buyDate");
    history.addColumn("expirationDate");
    history.addColumn("Description");
}

void History::log(InventoryAction action, Stock *stock)
{

    if(!stock)
        return;

    history.prepareInsert();

    history.addValue(action);
    history.addValue(stock->getProductId());
    history.addValue(stock->getQty());
    history.addValue(stock->getPrice());
    history.addValue(stock->getClient());
    history.addValue(stock->getLot());
    history.addValue(stock->getBuyDate().toString("dd/MM/yyyy"));
    history.addValue(stock->getExpDate().toString("dd/MM/yyyy"));
    history.addValue(stock->getDescription());

    history.exec();
}

void History::load()
{
    stocks.clear();

    actions.clear();

    history.prepareSelect();

    history.exec();

    QSqlQuery query = history.getQuery();

    while(query.next())
    {
        Stock* stock = new Stock();
        stock->loadFromRecord(query.record());
        stocks.push_front(stock);

        QString actionStr;
        InventoryAction action = (InventoryAction)query.record().value("action").toUInt();

        switch(action)
        {
        case BUY:
            actionStr = tr("Achat");
            break;
        case SELL:
            actionStr = tr("Vente");
            break;
        case DUMP:
            actionStr = tr("Corbeille");
            break;
        case REPRODUCE:
            actionStr = tr("Reproduire");
            break;
        }

        actions.push_front(actionStr);
        invActions.push_front(action);
    }
}

void History::loadToTable(QTableWidget *table)
{
    load();

//    QTableWidgetItem *buy = new QTableWidgetItem(QIcon("://ressources/icons/box-arrow-down.svg"), tr("Achat"));
//    QTableWidgetItem *sell = new QTableWidgetItem(QIcon("://ressources/icons/box-arrow-up.svg"), tr("Vente"));
//    QTableWidgetItem *dump = new QTableWidgetItem(QIcon("://ressources/icons/trash.svg"), tr("Corbeille"));
//    QTableWidgetItem *reproduce = new QTableWidgetItem(QIcon("://ressources/icons/recycle.svg"), tr("Reproduction"));


    table->setRowCount(0);

    int line = 0;
    foreach(Stock *stock, stocks)
    {
        if(!stock->getProduct())
            continue;


        int column = 0;

        table->insertRow(line);

        switch(invActions.at(line))
        {
        case BUY:
            table->setItem(line, column++, new QTableWidgetItem(QIcon("://ressources/icons/box-arrow-down.svg"), tr("Achat")));

            break;
        case SELL:
            table->setItem(line, column++, new QTableWidgetItem(QIcon("://ressources/icons/box-arrow-up.svg"), tr("Vente")));

            break;
        case DUMP:
            table->setItem(line, column++, new QTableWidgetItem(QIcon("://ressources/icons/trash.svg"), tr("Corbeille")));

            break;
        case REPRODUCE:
            table->setItem(line, column++, new QTableWidgetItem(QIcon("://ressources/icons/recycle.svg"), tr("Reproduction")));

            break;
        }


        table->setItem(line, column++, new QTableWidgetItem(stock->getProduct()->getName()));
        table->setItem(line, column++, new QTableWidgetItem(QString::number(stock->getQty())));
        table->setItem(line, column++, new QTableWidgetItem(QString::number(stock->getPrice())));
        table->setItem(line, column++, new QTableWidgetItem(stock->getBuyDate().toString("dd/MM/yyyy")));
        table->setItem(line, column++, new QTableWidgetItem(stock->getExpDate().toString("dd/MM/yyyy")));
        table->setItem(line, column++, new QTableWidgetItem(stock->getLot()));
        table->setItem(line, column++, new QTableWidgetItem(stock->getClient()));

        line++;
    }
}

const QVector<Stock *> &History::getStocks() const
{
    return stocks;
}

const QStringList &History::getActions() const
{
    return actions;
}
