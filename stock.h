#ifndef STOCK_H
#define STOCK_H

#include <QObject>
#include "product.h"

enum InventoryAction
{
    SELL,
    BUY,
    DUMP,
    REPRODUCE
};

class Stock : public QObject
{
    Q_OBJECT

    friend class StockFactory;
public:
    explicit Stock(QObject *parent = nullptr);

    void load();
    void insert();
    void update();
    void deletep();

    void store();

    void loadFromRecord(QSqlRecord rec);

    int getId() const;
    void setId(int newId);

    int getProductId() const;
    void setProductId(int newProductId);

    Product *getProduct();
    void setProduct(Product *newProduct);

    int getQty() const;
    void setQty(int newQty);

    float getPrice() const;
    void setPrice(float newPrice);

    const QString &getLot() const;
    void setLot(const QString &newLot);

    const QString &getClient() const;
    void setClient(const QString &newClient);

    const QDate &getBuyDate() const;
    void setBuyDate(const QDate &newBuyDate);

    const QDate &getExpDate() const;
    void setExpDate(const QDate &newExpDate);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);


private:

    int id;

    int productId;

    Product *product;

    int qty;

    float price;

    QString lot;

    QString client;

    QDate buyDate;

    QDate expDate;

    QString description;

    DatabaseTable inventory;

signals:

};


class StockFactory
{
public:
    static Stock* mergeStock(Stock* newStock, Stock* stock);
    static Stock* splitStock(Stock* newStock, Stock* stock);
};

#endif // STOCK_H
