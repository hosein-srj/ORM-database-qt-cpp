# ORM-database-qt-cpp
ORM Database for Qt C++ (SQL Server, MySQL, SQLLite)

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
std::vector<User*> users = dbContext.selectRecords<User>("users2",whereConditions);
```

