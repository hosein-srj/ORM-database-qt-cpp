# ORM-database-qt-cpp
ORM Database for Qt C++ (SQL Server, MySQL, SQLLite)

Just add DbContext.h and DbContext.cpp file to your Project

Define your Tables Like This

```cpp
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)
    Q_PROPERTY(QString family READ getFamily WRITE setFamily)

public:
    explicit User(QObject *parent = nullptr);
    User(const User&) = delete;
    User& operator=(const User&) = delete;

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString& name);

    QString getFamily() const;
    void setFamily(const QString& family);
private:
    int m_id;
    QString m_name;
    QString m_family;
};
```

Create DataBase
```cpp
/// Set connection string in Constructor
DbContext dbContext;
```
Create Table
```cpp
User record;
/// Define Table Column at User.h file
dbContext.createTable("users2",&record);
```
Insert Into Database Table
```cpp
record.setId(1);
record.setName("Tom");
record.setFamily("Shelby");
if (dbContext.insertRecord("users", record)) {
    qDebug() << "Record inserted successfully.";
} else {
    qDebug() << "Failed to insert record.";
}
```
Delete From table By Id
```cpp
int x = 1;
dbContext.deleteRecord("users",x);
```

Select All Table
```cpp
std::vector<User*> users = dbContext.selectAllRecords<User>("users");
```


Select by Where Condiction
```
QMap<QString, QVariant> whereConditions;
whereConditions.insert("id", 2);
whereConditions.insert("name", "hosein");
std::vector<User*> users = dbContext.selectRecords<User>("users",whereConditions);
```

