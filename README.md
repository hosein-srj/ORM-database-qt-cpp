# ORM-database-qt-cpp
ORM Database for Qt C++ (SQL Server, MySQL, SQLLite)

``` cpp /// Create DataBase
DbContext dbContext;

User record;
/// Create Table
dbContext.createTable("users2",&record);

/// Insert Into Database Table
record.setId(3);
record.setName("hasan");
record.setFamily("sj3");
if (dbContext.insertRecord("users2", record)) {
    qDebug() << "Record inserted successfully.";
} else {
    qDebug() << "Failed to insert record.";
}

/// Delete From table By Id
int x = 1;
dbContext.deleteRecord("users2",x);


/// Select by Where Condiction
QMap<QString, QVariant> whereConditions;
whereConditions.insert("id", 2);
whereConditions.insert("name", "hosein");
std::vector<User*> users = dbContext.selectRecords<User>("users2",whereConditions);
for(int i=0;i<users.size();i++){
    qDebug() << users[i]->getId() << users[i]->getName() << users[i]->getFamily();
}

'''
