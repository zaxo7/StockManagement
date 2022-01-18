#ifndef DATABASETABLE_H
#define DATABASETABLE_H

#include <QObject>
#include "database.h"

enum Operation{
    SELECT,
    INSERT,
    UPDATE,
    DELETE
};

enum Type{
    TEXT,
    REAL,
    INTEGER,
    BLOB,
    UNKNOWN
};

class DatabaseTable : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseTable(QString tableName, QObject *parent = nullptr);

    const QString &getTableName() const;
    void setTableName(const QString &newTableName);

    const QString &getWhereClause() const;
    void setWhereClause(const QString &newWhereClause);

    const QString &getLimitClause() const;
    void setLimitClause(const QString &newLimitClause);

    void addColumn(Type type, QString name);
    void addColumn(QString name);

    void clearValues();

    void addValue(QString value);
    void addValue(int value);
    void addValue(float value);
    void addValue(QByteArray value);

    void addValueAt(int pos, QString value);
    void addValueAt(int pos, int value);
    void addValueAt(int pos, float value);
    void addValueAt(int pos, QByteArray value);

    Operation getOperation() const;
    void setOperation(Operation newOperation);

    QString prepare(Operation op);

    QString prepareSelect();
    QString prepareInsert();
    QString prepareUpdate();
    QString prepareDelete();


    QString getSelect(QString where = "");
    QString getInsert();
    QString getUpdate(QString where = "");
    QString getDelete(QString where = "");

    int exec();

    QSqlRecord getRecord();
    const QSqlResult* getResult();

    const QSqlQuery &getQuery() const;
    void setQuery(const QSqlQuery &newQuery);

private:
    QString tableName;
    QStringList tableColumns;
    QList<Type> tableColumnTypes;
    QStringList tableValues;
    QString whereClause;
    QString limitClause;

    QString sql;

    QSqlQuery query;

    Operation operation;

    int bindingCursor;


signals:

};

#endif // DATABASETABLE_H
