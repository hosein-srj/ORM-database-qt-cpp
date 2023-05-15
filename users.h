#ifndef USERS_H
#define USERS_H

#include <QString>
#include <QObject>

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)

public:
    explicit User(QObject *parent = nullptr);

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString& name);

private:
    int m_id;
    QString m_name;
};




#endif // USERS_H
