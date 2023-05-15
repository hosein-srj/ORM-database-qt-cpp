#include <QtCore>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QMetaProperty>

class DbContext {
public:
    DbContext();
    ~DbContext();

    bool open();
    void close();

    // CRUD operations
    bool createRecord(const QString& tableName, const QVariantMap& data);
    QList<QVariantMap> readRecords(const QString& tableName);
    bool updateRecord(const QString& tableName, int id, const QVariantMap& data);
    bool deleteRecord(const QString& tableName, int id);
    bool createTable(const QString& tableName, const QObject* object);
    QString getSqlType(QMetaType::Type metaType);

private:
    QSqlDatabase m_database;
};
