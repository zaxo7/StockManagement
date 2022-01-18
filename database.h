#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>

#include <QDebug>

#include <QString>
#include <QStringList>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString& path = "./db.db", QObject *parent = NULL);

    static Database* getInstance(const QString& path = "./db.db");

    void initDatabase();

    //add product
    bool addProduct(QString name, float price, QString desc, QString brand, QString barcode, QByteArray *logo = NULL);
    bool updateProduct(QString name, float price, QString desc, QString brand, QString barcode, QByteArray *logo = NULL, QString where = "");

    const QSqlDatabase &getDb() const;
    void setDb(const QSqlDatabase &newDb);

private:
    QSqlDatabase db;


    static Database *database;

signals:

};

#endif // DATABASE_H
