#include "stock.h"

Stock::Stock(QObject *parent)
    : QObject{parent}, inventory("Inventory")
{
    id = -1;
    productId = -1;

    product = NULL;

    qty = 0;

    price = 0.0;

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

void Stock::load()
{
    Q_ASSERT(id != -1);

    inventory.setWhereClause("id = " + QString::number(id));

    inventory.prepareSelect();

    inventory.exec();

    QSqlQuery query = inventory.getQuery();

    if(query.next())
        loadFromRecord(query.record());
}

void Stock::insert()
{
    inventory.prepareInsert();

    inventory.addValue(productId);
    inventory.addValue(qty);
    inventory.addValue(price);
    inventory.addValue(client);
    inventory.addValue(lot);
    inventory.addValue(buyDate.toString("dd/MM/yyyy"));
    inventory.addValue(expDate.toString("dd/MM/yyyy"));
    inventory.addValue(description);

    id = inventory.exec();
}

void Stock::update()
{
    Q_ASSERT(id != -1);

    inventory.setWhereClause("id = " + QString::number(id));

    inventory.prepareUpdate();

    inventory.addValue(productId);
    inventory.addValue(qty);
    inventory.addValue(price);
    inventory.addValue(client);
    inventory.addValue(lot);
    inventory.addValue(buyDate.toString("dd/MM/yyyy"));
    inventory.addValue(expDate.toString("dd/MM/yyyy"));
    inventory.addValue(description);

    id = inventory.exec();
}

void Stock::deletep()
{
    if(id == -1)
        return;

    inventory.setWhereClause("id = " + QString::number(id));

    inventory.prepareDelete();

    inventory.exec();
}

void Stock::store()
{
    if(id != -1)
    {
        update();
    }
    else
    {
        insert();
    }
}

void Stock::loadFromRecord(QSqlRecord rec)
{
    id = rec.value("id").toInt();
    productId = rec.value("productId").toInt();
    price = rec.value("stockPrice").toFloat();
    qty = rec.value("stockQty").toInt();
    client = rec.value("client").toString();
    lot = rec.value("lot").toString();
    buyDate = QDate::fromString(rec.value("buyDate").toString(), "dd/MM/yyyy");
    expDate = QDate::fromString(rec.value("expirationDate").toString(), "dd/MM/yyyy");;
    description = rec.value("description").toString();

    //load the product
    product = new Product();
    product->setId(productId);
    product->load();
}

int Stock::getId() const
{
    return id;
}

void Stock::setId(int newId)
{
    id = newId;
}

int Stock::getProductId() const
{
    return productId;
}

void Stock::setProductId(int newProductId)
{
    if(product && (product->getId() != newProductId))
        delete product;
    productId = newProductId;

    product = new Product();
    product->setId(newProductId);
    product->load();
}

Product *Stock::getProduct()
{
    if(!product)
        product = new Product();
    return product;
}

void Stock::setProduct(Product *newProduct)
{
    product = newProduct;
}

const QString &Stock::getLot() const
{
    return lot;
}

void Stock::setLot(const QString &newLot)
{
    lot = newLot;
}

int Stock::getQty() const
{
    return qty;
}

void Stock::setQty(int newQty)
{
    qty = newQty;
}

float Stock::getPrice() const
{
    return price;
}

void Stock::setPrice(float newPrice)
{
    price = newPrice;
}

const QString &Stock::getClient() const
{
    return client;
}

void Stock::setClient(const QString &newClient)
{
    client = newClient;
}

const QDate &Stock::getBuyDate() const
{
    return buyDate;
}

void Stock::setBuyDate(const QDate &newBuyDate)
{
    buyDate = newBuyDate;
}

const QDate &Stock::getExpDate() const
{
    return expDate;
}

void Stock::setExpDate(const QDate &newExpDate)
{
    expDate = newExpDate;
}

const QString &Stock::getDescription() const
{
    return description;
}

void Stock::setDescription(const QString &newDescription)
{
    description = newDescription;
}



Stock *StockFactory::mergeStock(Stock *newStock, Stock *stock)
{
    if(!stock || !newStock)
        return NULL;

    if(stock->getProductId() != newStock->getProductId())
        return NULL;

    //sum
    newStock->price += stock->price;
    newStock->qty += stock->qty;

    //replace
    newStock->lot = stock->lot;
    newStock->client = stock->client;
    newStock->description = stock->description;

    newStock->buyDate = stock->buyDate;
    newStock->expDate = stock->expDate;
}

Stock *StockFactory::splitStock(Stock *newStock, Stock *stock)
{
    if(!stock || !newStock)
        return NULL;

    if(stock->getProductId() != newStock->getProductId())
        return NULL;

    //sum
    newStock->price -= stock->price;

    newStock->qty -= stock->qty;

    //replace

    //newStock->lot = stock->lot;

    //newStock->client = stock->client;
    //newStock->description = stock->description;

    //newStock->buyDate = stock->buyDate;
    //newStock->expDate = stock->expDate;
}
