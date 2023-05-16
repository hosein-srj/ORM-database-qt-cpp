#include "users.h"

User::User(QObject *parent) : QObject(parent), m_id(0)
{
}

int User::getId() const
{
    return m_id;
}

void User::setId(int id)
{
    m_id = id;
}

QString User::getName() const
{
    return m_name;
}

void User::setName(const QString& name)
{
    m_name = name;
}

QString User::getFamily() const
{
    return m_family;
}

void User::setFamily(const QString& family)
{
    m_family = family;
}
