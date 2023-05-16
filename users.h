#ifndef USERS_H
#define USERS_H

#include <QString>
#include <QObject>

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




#endif // USERS_H
