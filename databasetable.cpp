#include "databasetable.h"

DatabaseTable::DatabaseTable(QString tableName, QObject *parent)
    : QObject{parent}, query(Database::getInstance()->getDb())
{
    this->tableName = tableName;
}

const QString &DatabaseTable::getTableName() const
{
    return tableName;
}

void DatabaseTable::setTableName(const QString &newTableName)
{
    tableName = newTableName;
}


const QString &DatabaseTable::getWhereClause() const
{
    return whereClause;
}

void DatabaseTable::setWhereClause(const QString &newWhereClause)
{
    whereClause = newWhereClause;
}

const QString &DatabaseTable::getLimitClause() const
{
    return limitClause;
}

void DatabaseTable::setLimitClause(const QString &newLimitClause)
{
    limitClause = newLimitClause;
}

void DatabaseTable::addColumn(Type type, QString name)
{
    tableColumns << name;
    tableColumnTypes << type;
}

void DatabaseTable::addColumn(QString name)
{
    tableColumns << name;
}

void DatabaseTable::clearValues()
{

}

void DatabaseTable::addValue(QString value)
{
    query.bindValue(":" + tableColumns[bindingCursor++], value);
}

void DatabaseTable::addValue(int value)
{
    query.bindValue(":" + tableColumns[bindingCursor++], value);
}

void DatabaseTable::addValue(float value)
{
    query.bindValue(":" + tableColumns[bindingCursor++], value);
}

void DatabaseTable::addValue(QByteArray value)
{
    query.bindValue(":" + tableColumns[bindingCursor++], value);
}

void DatabaseTable::addValueAt(int pos, QString value)
{
    Q_ASSERT(tableColumns.size() > pos);
    query.bindValue(":" + tableColumns[pos], value);
}

void DatabaseTable::addValueAt(int pos, int value)
{
    Q_ASSERT(tableColumns.size() > pos);
    query.bindValue(":" + tableColumns[pos], value);
}

void DatabaseTable::addValueAt(int pos, float value)
{
    Q_ASSERT(tableColumns.size() > pos);
    query.bindValue(":" + tableColumns[pos], value);
}

void DatabaseTable::addValueAt(int pos, QByteArray value)
{
    Q_ASSERT(tableColumns.size() > pos);
    query.bindValue(":" + tableColumns[pos], value);
}

Operation DatabaseTable::getOperation() const
{
    return operation;
}

void DatabaseTable::setOperation(Operation newOperation)
{
    operation = newOperation;
}

QString DatabaseTable::prepare(Operation op)
{
    switch(op)
    {
        case SELECT:
            return prepareSelect();
        case INSERT:
            return prepareInsert();
        case UPDATE:
            return prepareUpdate();
        case DELETE:
            return prepareDelete();
        default:
            return QString();
    }
}

QString DatabaseTable::prepareSelect()
{
    bindingCursor = 0;
    sql = getSelect();
    query.prepare(sql);
    return sql;
}

QString DatabaseTable::prepareInsert()
{
    bindingCursor = 0;
    sql = getInsert();
    query.prepare(sql);
    return sql;
}

QString DatabaseTable::prepareUpdate()
{
    bindingCursor = 0;
    sql = getUpdate();
    query.prepare(sql);
    return sql;
}

QString DatabaseTable::prepareDelete()
{
    bindingCursor = 0;
    sql = getDelete();
    query.prepare(sql);
    return sql;
}

QString DatabaseTable::getSelect(QString where)
{
    QString query = "SELECT * FROM  " + tableName + " ";

    if(!where.isEmpty())
        query.append(" WHERE " + where);
    else
        if(!whereClause.isEmpty())
            query.append(" WHERE " + whereClause);

    return query;
}

QString DatabaseTable::getInsert()
{
//    if(tableValues.size() != tableColumns.size())
//        return QString("the values and columns are not the same size ") + QString::number(tableValues.size()) + " != " + QString::number(tableColumns.size());

    QString query = "INSERT INTO " + tableName + "(" + tableColumns.join(", ") + ") VALUES(:" + tableColumns.join(", :") + ")";

    return query;
}

QString DatabaseTable::getUpdate(QString where)
{
//    if(tableValues.size() != tableColumns.size())
//        return QString("the values and columns are not the same size ") + QString::number(tableValues.size()) + " != " + QString::number(tableColumns.size());

    QString query = "UPDATE  " + tableName + " ";

    bool firstSet = true;
    foreach(QString column, tableColumns)
    {
        if(firstSet)
        {
            query.append("SET " + column + " = :" + column);
            firstSet = false;
        }
        else
            query.append(", " + column + " = :" + column);
    }

    Q_ASSERT(!where.isEmpty() || !whereClause.isEmpty());

    if(!where.isEmpty())
        query.append(" WHERE " + where);
    else
        if(!whereClause.isEmpty())
            query.append(" WHERE " + whereClause);

    return query;
}

QString DatabaseTable::getDelete(QString where)
{
    QString query = "DELETE FROM  " + tableName + " ";

    Q_ASSERT(!where.isEmpty() || !whereClause.isEmpty());

    if(!where.isEmpty())
        query.append(" WHERE " + where);
    else
        if(!whereClause.isEmpty())
            query.append(" WHERE " + whereClause);

    return query;
}

int DatabaseTable::exec()
{

    if(query.exec())
    {
        qDebug() << "executed:"
                 << query.executedQuery();
        return query.lastInsertId().toInt();
    }
    else
    {
        qDebug() << "error:"
                 << query.executedQuery() << "\n"
                 << query.lastError();
        return -1;
    }
}

QSqlRecord DatabaseTable::getRecord()
{
    return query.record();
}

const QSqlResult* DatabaseTable::getResult()
{
    return query.result();
}

const QSqlQuery &DatabaseTable::getQuery() const
{
    return query;
}

void DatabaseTable::setQuery(const QSqlQuery &newQuery)
{
    query = newQuery;
}
