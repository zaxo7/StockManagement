#ifndef PRODUCT_H
#define PRODUCT_H

#include <QObject>
#include <QPixmap>
#include <QBuffer>
#include <QDate>
#include <QLabel>
#include "databasetable.h"


class Product : public QObject
{
    Q_OBJECT

    friend class ProductFactory;
public:
    Product();

    void load();
    void insert();
    void update();
    void deletep();

    void store();

    void loadFromRecord(QSqlRecord rec);

    int getId() const;
    void setId(int newId);

    const QString &getName() const;
    void setName(const QString &newName);

    float getPrice() const;
    void setPrice(float newPrice);

    const QByteArray &getLogo() const;
    void setLogo(const QByteArray &newLogo);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

    const QString &getBrand() const;
    void setBrand(const QString &newBrand);

    const QString &getBarcode() const;
    void setBarcode(const QString &newBarcode);

    bool loadLogoToLabel(QLabel *label);

private:
    int id;
    QString name;
    float price;
    QByteArray logo;
    QString description;
    QString brand;
    QString barcode;

    DatabaseTable product;

signals:

};


class ProductFactory
{
public:
    static bool compare(Product* p1, Product* p2);
};

#endif // PRODUCT_H
