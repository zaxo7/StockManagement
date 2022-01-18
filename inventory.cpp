#include "inventory.h"

#include "mainwindow.h"


Inventory::Inventory(QObject *parent)
    : QObject{parent}, inventory("inventory")
{

    totalQty = 0;

    totalPrice = 0.0;

    //add the columns
    inventory.addColumn("productId");
    inventory.addColumn("stockQty");
    inventory.addColumn("stockPrice");
    inventory.addColumn("client");
    inventory.addColumn("lot");
    inventory.addColumn("buyDate");
    inventory.addColumn("expirationDate");
    inventory.addColumn("Description");
}

void Inventory::load()
{
    stocks.clear();

    inventory.prepareSelect();

    inventory.exec();

    QSqlQuery query = inventory.getQuery();

    while(query.next())
    {
        Stock* stock = new Stock();
        stock->loadFromRecord(query.record());
        stocks.push_back(stock);
    }
}

void Inventory::insert()
{

}

void Inventory::update()
{

}

void Inventory::deletep()
{

}

int Inventory::buy(Stock *stock)
{
    if(!stock)
        return -1;
    //find the product if exists
    int productIndex = findProduct(stock->getProductId());

    Stock *newStock = NULL;

    if(productIndex != -1)
    {
        newStock = stocks.at(productIndex);
        StockFactory::mergeStock(newStock, stock);
    }
    else
    {
        newStock = stock;
    }

    newStock->store();
    history->log(BUY, stock);
    return 0;
}

int Inventory::sell(Stock *stock)
{
    if(!stock)
        return -1;
    //find the product if exists
    int productIndex = findProduct(stock->getProductId());

    Stock *newStock = NULL;

    if(productIndex != -1)
    {
        newStock = stocks.at(productIndex);
        //verifier la quantité existante
        if(newStock->getQty() < stock->getQty())
        {
            QMessageBox::critical(MainWindow::getInstance(),tr("La quantité selectioné est supérieure a celle du stock"),QString(tr("la quantité du (%1) selectioné n'existe pas dans votre stock !")).arg(stock->getProduct()->getName()));
            return -3;
        }
        StockFactory::splitStock(newStock, stock);

    }
    else
    {
        QMessageBox::critical(MainWindow::getInstance(),tr("Produit n'existe pas"),QString(tr("le produit (%1) n'existe pas dans votre stock !")).arg(stock->getProduct()->getName()));
        return -2;
    }

    if(newStock->getQty() > 0)
        newStock->store();
    else
        newStock->deletep();

    history->log(SELL, stock);

    return 0;
}

int Inventory::dump(Stock *stock)
{
    if(!stock)
        return -1;
    //find the product if exists
    int productIndex = findProduct(stock->getProductId());

    Stock *newStock = NULL;

    if(productIndex != -1)
    {
        newStock = stocks.at(productIndex);
        //verifier la quantité existante
        if(newStock->getQty() < stock->getQty())
        {
            QMessageBox::critical(MainWindow::getInstance(),tr("La quantité selectioné est supérieure a celle du stock"),QString(tr("la quantité du (%1) selectioné n'existe pas dans votre stock !")).arg(stock->getProduct()->getName()));
            return -3;
        }
        StockFactory::splitStock(newStock, stock);
    }
    else
    {
        QMessageBox::critical(MainWindow::getInstance(),tr("Produit n'existe pas"),QString(tr("le produit (%1) n'existe pas dans votre stock !")).arg(stock->getProduct()->getName()));
        return -2;
    }

    newStock->store();

    history->log(DUMP, stock);

    return 0;
}

int Inventory::reproduce(Stock *stock)
{
    if(!stock)
        return -1;
    //find the product if exists
    int productIndex = findProduct(stock->getProductId());

    Stock *newStock = NULL;

    if(productIndex != -1)
    {
        newStock = stocks.at(productIndex);
        StockFactory::mergeStock(newStock, stock);
    }
    else
    {
        newStock = stock;
    }

    newStock->store();

    history->log(REPRODUCE, stock);
    return 0;
}

void Inventory::store()
{

}

void Inventory::loadFromRecord(QSqlRecord rec)
{

}

void Inventory::loadToTable(QTableWidget *table)
{
    load();

    table->setRowCount(0);

    int line = 0;
    foreach(Stock *stock, stocks)
    {
        if(!stock->getProduct())
            continue;

        int column = 0;

        table->insertRow(line);

        table->setItem(line, column++, new QTableWidgetItem(stock->getProduct()->getName()));
        table->setItem(line, column++, new QTableWidgetItem(QString::number(stock->getQty())));
        table->setItem(line, column++, new QTableWidgetItem(QString::number(stock->getPrice())));
        table->setItem(line, column++, new QTableWidgetItem(stock->getBuyDate().toString("dd/MM/yyyy")));
        table->setItem(line, column++, new QTableWidgetItem(stock->getExpDate().toString("dd/MM/yyyy")));
        table->setItem(line, column++, new QTableWidgetItem(stock->getClient()));

        line++;
    }
}

int Inventory::findProduct(int id)
{
    int index = 0;
    foreach(Stock* stock, stocks)
    {
        if(stock && (stock->getProductId() == id))
            return index;
        index++;
    }

    return -1;
}


const QVector<Stock *> &Inventory::getStocks() const
{
    return stocks;
}

void Inventory::setStocks(const QVector<Stock *> &newStocks)
{
    stocks = newStocks;
}

int Inventory::getTotalQty() const
{
    return totalQty;
}

void Inventory::setTotalQty(int newTotalQty)
{
    totalQty = newTotalQty;
}

History *Inventory::getHistory() const
{
    return history;
}

void Inventory::setHistory(History *newHistory)
{
    history = newHistory;
}


