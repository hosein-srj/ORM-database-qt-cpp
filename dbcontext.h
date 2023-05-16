#include <QtCore>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QMetaProperty>
#include <users.h>
#include <QMetaType>
#include <sstream>




class DbContext {
public:
    DbContext();
    ~DbContext();

    bool open();
    void close();

    // CRUD operations
    template<typename T>
    bool insertRecord(const QString& tableName, const T& record)
    {
        QSqlQuery query(m_database);

        QString queryString = QString("INSERT INTO %1 (").arg(tableName);

        const QMetaObject* metaObject = record.metaObject();
        if (!metaObject) {
            qDebug() << "Failed to retrieve meta object.";
            return false;
        }

        int propertyCount = metaObject->propertyCount();
        QStringList columns;
        QStringList placeholders;

        for (int i = 0; i < propertyCount; ++i) {
            const QMetaProperty metaProperty = metaObject->property(i);
            const QString propertyName = QString::fromLatin1(metaProperty.name());
            columns << propertyName;
            placeholders << QString(":%1").arg(propertyName);
        }

        queryString += columns.join(", ") + ") VALUES (" + placeholders.join(", ") + ")";
        query.prepare(queryString);

        for (int i = 0; i < propertyCount; ++i) {
            const QMetaProperty metaProperty = metaObject->property(i);
            const QString propertyName = QString::fromLatin1(metaProperty.name());
            const QVariant value = QVariant::fromValue(metaProperty.read(&record));
            query.bindValue(QString(":%1").arg(propertyName), value);
        }

        if (!query.exec()) {
            qDebug() << "Failed to insert record:" << query.lastError().text();
            return false;
        }

        return true;
    }

    template<typename T>
    bool updateRecord(const QString& tableName, const T& record)
    {
        QSqlQuery query(m_database);

        QString queryString = QString("UPDATE %1 SET ").arg(tableName);

        const QMetaObject* metaObject = record.metaObject();
        if (!metaObject) {
            qDebug() << "Failed to retrieve meta object.";
            return false;
        }

        int propertyCount = metaObject->propertyCount();
        QStringList updateFields;

        for (int i = 0; i < propertyCount; ++i) {
            const QMetaProperty metaProperty = metaObject->property(i);
            const QString propertyName = QString::fromLatin1(metaProperty.name());
            if(propertyName == "id")
                continue;
            const QVariant value = metaProperty.read(&record);

            //qDebug() << value;
            //updateFields << QString("%1 = '%2'").arg(propertyName).arg(value.toString());
            //query.bindValue(QString(":%1").arg(propertyName), value);
            updateFields << QString("%1 = '%2'").arg(propertyName).arg(value.toString());
            query.bindValue(QString(":%1").arg(propertyName), value);
        }

        queryString += updateFields.join(", ");
        queryString += " WHERE id = " + QString::number(record.getId()); // Adjust the WHERE condition based on your requirements
       // query.bindValue(":id", + record.getId()); // Assuming the ID property is named "id"

        qDebug() << queryString << record.getId();
        query.prepare(queryString);

        if (!query.exec()) {
            qDebug() << "Failed to update record:" << query.lastError().text();
            return false;
        }

        return true;
    }


    template <typename T>
    bool deleteRecord(const QString& tableName, T& id) {
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

    template<typename T>
    std::vector<T*> selectAllRecords(const QString& tableName)
    {
        std::vector<T*> records;

        QSqlQuery query(m_database);
        QString queryString = QString("SELECT * FROM %1").arg(tableName);

        //qDebug() << queryString;
        query.prepare(queryString);

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return records;
        }

        const QMetaObject* metaObject = &T::staticMetaObject;
        if (!metaObject) {
            qDebug() << "Failed to retrieve meta object.";
            return records;
        }

        while (query.next()) {
            T* record = new T;
            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                const QMetaProperty metaProperty = metaObject->property(i);
                const QString propertyName = QString::fromLatin1(metaProperty.name());
                const QVariant value = query.value(propertyName);
                metaProperty.write(record, value);
            }

            records.push_back(record);
        }

        return records;
    }

    template<typename T>
    std::vector<T*> selectRecords(const QString& tableName, const QString& whereCondition)
    {
        std::vector<T*> records;

        QSqlQuery query(m_database);
        QString queryString = QString("SELECT * FROM %1").arg(tableName);

        if (!whereCondition.isEmpty()) {
            queryString += " WHERE " + whereCondition;
        }

        query.prepare(queryString);

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return records;
        }

        const QMetaObject* metaObject = &T::staticMetaObject;
        if (!metaObject) {
            qDebug() << "Failed to retrieve meta object.";
            return records;
        }

        while (query.next()) {
            T* record = new T;
            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                const QMetaProperty metaProperty = metaObject->property(i);
                const QString propertyName = QString::fromLatin1(metaProperty.name());
                const QVariant value = query.value(propertyName);
                metaProperty.write(record, value);
            }

            records.push_back(record);
        }

        return records;
    }

    template<typename T>
    std::vector<T*> selectRecords(const QString& tableName, const QMap<QString, QVariant>& whereConditions)
    {
        std::vector<T*> records;

        QSqlQuery query(m_database);
        QString queryString = QString("SELECT * FROM %1").arg(tableName);

        if (!whereConditions.isEmpty()) {
            queryString += " WHERE ";
            //QStringList whereClauses;
            int counter = 0;
            for (const auto& key : whereConditions.keys()) {
                if(counter != 0)
                    queryString += " AND ";
                const QVariant& value = whereConditions.value(key);
                QString whereClause;
                if(isNumber(value.toString().toStdString()))
                    queryString += " " + key + " = " + value.toString(); //whereClause = QString("%1 = :%2").arg(key).arg(key);
                else
                    queryString += " " + key + " = '" + value.toString() + "'"; //whereClause = QString("%1 = ':%2'").arg(key).arg(key);

                counter++;
//                whereClauses.append(whereClause);
//                query.bindValue(QString(":%1").arg(key), value);
            }

            //queryString += " WHERE " + whereClauses.join(" AND ");
        }

        qDebug() << queryString;
        query.prepare(queryString);

        if (!query.exec()) {
            qDebug() << "Failed to execute query:" << query.lastError().text();
            return records;
        }

        const QMetaObject* metaObject = &T::staticMetaObject;
        if (!metaObject) {
            qDebug() << "Failed to retrieve meta object.";
            return records;
        }

        while (query.next()) {
            T* record = new T;
            for (int i = 0; i < metaObject->propertyCount(); ++i) {
                const QMetaProperty metaProperty = metaObject->property(i);
                const QString propertyName = QString::fromLatin1(metaProperty.name());
                const QVariant value = query.value(propertyName);
                metaProperty.write(record, value);
            }

            records.push_back(record);
        }

        return records;
    }


    bool createTable(const QString& tableName, const QObject* object);
    QString getSqlType(QMetaType::Type metaType);
    bool isNumber(const std::string& str);

private:
    QSqlDatabase m_database;
};
