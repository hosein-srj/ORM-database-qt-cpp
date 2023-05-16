#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <dbcontext.h>
#include <users.h>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    DbContext dbContext;
//    User record;
//    dbContext.createTable("users2",&record);
//    record.setId(3);
//    record.setName("hasan");
//    record.setFamily("sj3");

//    if (dbContext.insertRecord("users2", record)) {
//        qDebug() << "Record inserted successfully.";
//    } else {
//        qDebug() << "Failed to insert record.";
//    }
    //int x = 1;
    //dbContext.deleteRecord("users2",x);

    // Create an instance of the Database class
    //QString x= ;

    QMap<QString, QVariant> whereConditions;
    whereConditions.insert("id", 2);
    whereConditions.insert("name", "hosein");
    std::vector<User*> users = dbContext.selectRecords<User>("users2",whereConditions);
    for(int i=0;i<users.size();i++){
        qDebug() << users[i]->getId() << users[i]->getName() << users[i]->getFamily();
    }
    //qDebug() << "Is?" << isNumber("-125");

    return app.exec();
}
