#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariantMap>

#include "CelestialBodyNode.h"

class CelestialBodyNode;

class DataBase {
public:
    DataBase() {}
    DataBase(const QString &tableName);
    bool connect(const QString &dbName, const QString &user, \
                 const QString &password, const QString &host = "localhost");
    QSqlQuery executeQuery(const QString &query);
    QList<QVariantMap> fetchAll();
    void saveInDataBase(const QList<CelestialBodyNode*> &celestialBodies);

private:
    QSqlDatabase db;
    QString tableName;
};
