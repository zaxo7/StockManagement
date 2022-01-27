#include "database.h"

Database *Database::database = NULL;

Database::Database(const QString& path, QObject *parent)
    : QObject{parent}
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open())
    {
      qDebug() << "Error: connection with database failed";
    }
    else
    {
      qDebug() << "Database: connection ok";
      initDatabase();
    }
}

Database *Database::getInstance(const QString& path)
{
    if(!database)
        database = new Database(path);

    return database;
}

void Database::initDatabase()
{
    QStringList sqlL;

    bool resetDatabase = false;

    sqlL << (resetDatabase? "drop table if exists Product;":"")
         << "Create table if not exists Product (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, price REAL DEFAULT 0.0, description TEXT, brand TEXT, barcode TEXT, logo BLOB);"
         << (resetDatabase? "drop table if exists Inventory;":"")
         << "Create table if not exists Inventory (id INTEGER PRIMARY KEY AUTOINCREMENT, productId INTEGER NOT NULL, stockQty INTEGER DEFAULT 0, stockPrice FLOAT DEFAULT 0.0, client TEXT DEFAULT '', lot TEXT, buyDate TEXT, expirationDate TEXT, Description TEXT);"
         << (resetDatabase? "drop table if exists History;":"")
         << "Create table if not exists History (id INTEGER PRIMARY KEY AUTOINCREMENT, action TEXT, productId INTEGER NOT NULL, stockQty INTEGER DEFAULT 0, stockPrice FLOAT DEFAULT 0.0, client TEXT DEFAULT '', lot TEXT, buyDate TEXT, expirationDate TEXT, Description TEXT);";

    QSqlQuery query;

    foreach(QString sql, sqlL)
    {
        if(!sql.isEmpty())
        {
            query.prepare(sql);

            if(!query.exec())
            {
                qDebug() << "database init error:"
                         << query.executedQuery() << "\n"
                         << query.lastError();
            }
        }
    }


}


bool Database::addProduct(QString name, float price, QString desc, QString brand, QString barcode, QByteArray *logo)
{
    bool success = false;
    // you should check if args are ok first...
    QSqlQuery query;
    query.prepare("INSERT INTO Product(name, price, description, brand, barcode, logo) VALUES(:name, :price, :description, :brand, :barcode, :logo)");

    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":description", desc);
    query.bindValue(":brand", brand);
    query.bindValue(":barcode", barcode);
    if(logo)
        query.bindValue(":logo", *logo);
    else
        query.bindValue(":logo", QByteArray());

    if(query.exec())
    {
       success = true;
    }
    else
    {
        qDebug() << "addProduct error:"
                 << query.executedQuery() << "\n"
                 << query.lastError();
    }

    return success;
}

bool Database::updateProduct(QString name, float price, QString desc, QString brand, QString barcode, QByteArray *logo, QString where)
{
    bool success = false;
    // you should check if args are ok first...
    QString sql = "UPDATE Product SET name = :name, price = :price, description = :description, brand = :brand, barcode = :barcode, logo = :logo";

    if(!where.isEmpty())
        sql.append(" WHERE " + where);

    QSqlQuery query;
    query.prepare(sql);

    query.bindValue(":name", name);
    query.bindValue(":price", price);
    query.bindValue(":description", desc);
    query.bindValue(":brand", brand);
    query.bindValue(":barcode", barcode);
    if(logo)
        query.bindValue(":logo", *logo);
    else
        query.bindValue(":logo", QByteArray());

    if(query.exec())
    {
       success = true;
    }
    else
    {
        qDebug() << "addProduct error:"
                 << query.executedQuery() << "\n"
                 << query.lastError();
    }

    return success;
}

const QSqlDatabase &Database::getDb() const
{
    return db;
}

void Database::setDb(const QSqlDatabase &newDb)
{
    db = newDb;
}
