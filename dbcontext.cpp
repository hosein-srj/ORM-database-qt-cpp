#include "DbContext.h"
#include <QMetaProperty>
#include <QMetaType>

DbContext::DbContext() {
//    m_database = QSqlDatabase::addDatabase("QODBC");
//    m_database.setDatabaseName("test_database");
//    database.setDatabaseName("DRIVER={SQL Server};Server=(local)\\myInstanceName;Database=myDataBase;Trusted_Connection=yes;");

//    // Set your SQL Server connection details here
//    m_database.setHostName("your_server_hostname");
//    m_database.setPort(1433);
//    m_database.setUserName("your_username");
//    m_database.setPassword("your_password");
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("db.db");

    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        //return -1;

    }

    else
        qDebug() << "Database opened successfully";
}

DbContext::~DbContext() {
    close();
}

bool DbContext::open() {
    if (!m_database.isOpen() && !m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    return true;
}

void DbContext::close() {
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DbContext::createRecord(const QString& tableName, const QVariantMap& data) {
    QSqlQuery query(m_database);

    QStringList columnNames;
    QStringList placeholders;
    QList<QVariant> values;

    for (const QString& key : data.keys()) {
        columnNames << key;
        placeholders << ":" + key;
        values << data.value(key);
    }

    QString queryString = QString("INSERT INTO %1 (%2) VALUES (%3)")
        .arg(tableName)
        .arg(columnNames.join(", "))
        .arg(placeholders.join(", "));

    query.prepare(queryString);

    for (int i = 0; i < values.size(); ++i) {
        query.bindValue(":" + columnNames[i], values[i]);
    }

    if (!query.exec()) {
        qDebug() << "Failed to create record:" << query.lastError().text();
        return false;
    }

    return true;
}

QList<QVariantMap> DbContext::readRecords(const QString& tableName) {
    QSqlQuery query(m_database);

    QString queryString = QString("SELECT * FROM %1").arg(tableName);
    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Failed to read records:" << query.lastError().text();
        return QList<QVariantMap>();
    }

    QList<QVariantMap> records;

    while (query.next()) {
        QVariantMap record;
        QSqlRecord sqlRecord = query.record();
        for (int i = 0; i < sqlRecord.count(); ++i) {
            record[sqlRecord.fieldName(i)] = query.value(i);
        }
        records.append(record);
    }

    return records;
}

bool DbContext::updateRecord(const QString& tableName, int id, const QVariantMap& data) {
    QSqlQuery query(m_database);

    QStringList updateStatements;
    QList<QVariant> values;

    for (const QString& key : data.keys()) {
        updateStatements << QString("%1 = :%1").arg(key);
        values << data.value(key);
    }

    QString queryString = QString("UPDATE %1 SET %2 WHERE id = :id")
        .arg(tableName)
        .arg(updateStatements.join(", "));

    query.prepare(queryString);
    query.bindValue(":id", id);

    for (int i = 0; i < values.size(); ++i) {
        query.bindValue(":" + data.keys().at(i), values.at(i));
    }

    if (!query.exec()) {
        qDebug() << "Failed to update record:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DbContext::deleteRecord(const QString& tableName, int id) {
    QSqlQuery query(m_database);

    QString queryString = QString("DELETE FROM %1 WHERE id = :id").arg(tableName);

    query.prepare(queryString);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to delete record:" << query.lastError().text();
        return false;
    }

    return true;
}

bool DbContext::createTable(const QString& tableName, const QObject* object) {
    if (!object) {
        qDebug() << "Invalid object pointer.";
        return false;
    }

    QSqlQuery query(m_database);

    QString queryString = QString("CREATE TABLE %1 (").arg(tableName);

    const QMetaObject* metaObject = const_cast<QObject*>(object)->metaObject();

    if (!metaObject) {
        qDebug() << "Failed to retrieve meta object.";
        return false;
    }

    int propertyCount = metaObject->propertyCount();

    QStringList columnDefinitions;

    for (int i = 0; i < propertyCount; ++i) {
        const QMetaProperty metaProperty = metaObject->property(i);
        const QString propertyName = metaProperty.name();
        const QMetaType::Type propertyType = static_cast<QMetaType::Type>(metaProperty.type());

        QString columnDefinition = QString("%1 %2")
            .arg(propertyName)
            .arg(getSqlType(propertyType));

        columnDefinitions << columnDefinition;
    }

    queryString += columnDefinitions.join(", ") + ")";

    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}


QString DbContext::getSqlType(QMetaType::Type metaType) {
    switch (metaType) {
        case QMetaType::Int:
            return "INT";
        case QMetaType::Double:
            return "DOUBLE";
        case QMetaType::QString:
            return "VARCHAR(255)";
        // Handle other types as needed
        default:
            return "VARCHAR(255)";
    }
}
