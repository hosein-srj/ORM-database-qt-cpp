#include "DbContext.h"


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
        qDebug() << "Failed to create table:" << query.lastError().text() << queryString;
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

bool DbContext::isNumber(const std::string& str)
{
    std::istringstream iss(str);
    double value;
    return (iss >> value) && (iss.eof());
}
