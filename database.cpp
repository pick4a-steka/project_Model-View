#include "database.h"

#include <QtSql/QSqlError>
#include <QDebug>

DataBase::DataBase(const QString &tableName) : tableName(tableName) {
    /*
     * метод добавления драйвера PostrgreSQL к объекту db
     * это необъодимо для того, чтобы Qt знал какой драйвер
     * использовать для подключения к базе данных
    */
    db = QSqlDatabase::addDatabase("QPSQL");
}

bool DataBase::connect(const QString &dbName, const QString &user, const QString &password, const QString &host) {
    /*
     * setHostName - устанавливает имя хоста сервера базы данных
    */
    db.setHostName(host);

    /*
     * setDatabaseName - устанавливает имя подключаемой базы данных
    */
    db.setDatabaseName(dbName);

    /*
     * setUserName - устанавливает имя пользователя для аутентификации
     * в базе данных
    */
    db.setUserName(user);

    /*
     * setPassword - устанавливает пароль для аутентификации
    */
    db.setPassword(password);

    // проверка соединения с БД
    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
        return false;
    }

    return true;
}

QSqlQuery DataBase::executeQuery(const QString &query) {
    QSqlQuery sqlQuery;
    if (!sqlQuery.exec(query)) {
        qDebug() << "Ошибка выполнения запроса:" << sqlQuery.lastError().text();
        qDebug() << "Запрос:" << query;

        // Вывести список всех таблиц в схеме public
        QSqlQuery tableQuery("SELECT tablename FROM pg_tables WHERE schemaname = 'public';");
        while (tableQuery.next()) {
            qDebug() << "Доступная таблица:" << tableQuery.value(0).toString();
        }
    }
    return sqlQuery;
}

QList<QVariantMap> DataBase::fetchAll() {
    QSqlQuery query = executeQuery(QString("SELECT * FROM %1;").arg(tableName));

    QList<QVariantMap> results;

    while (query.next()) {
        QVariantMap row;
        // row["celestial_id"] = query.value("celestial_id");
        row["name"] = query.value("name");
        row["type"] = query.value("type");
        row["info"] = query.value("info");
        row["hex_color"] = query.value("hex_color");
        row["radius"] = query.value("radius");
        row["parent"] = query.value("parent");
        results.append(row);
    }

    return results;
}

void DataBase::saveInDataBase(const QList<CelestialBodyNode*> &celestialBodies) {
    QSqlQuery query;
    QString query_delete = QString("DELETE FROM %1;").arg(tableName);
    QString query_insert = QString("INSERT INTO %1 (name, type, info, hex_color, radius, parent)").arg(tableName);

    // Очистка таблицы
    if (!query.exec(query_delete)) {
        qDebug() << "Ошибка при очистке таблицы:" << query.lastError().text();
        return;
    }

    // Подготавлияваем запрос для вставки данных
    query.prepare(query_insert + "VALUES (:name, :type, :info, :hex_color, :radius, :parent);");

    // Обходим каждый узел и добавляем его в таблицу
    for (const auto &node : celestialBodies) {
        query.bindValue(":name", node->getName());
        query.bindValue(":type", node->getType());
        query.bindValue(":info", node->getInfo());
        query.bindValue(":hex_color", node->getColor().name());
        query.bindValue(":radius", node->getRadius());
        query.bindValue(":parent", node->getParent() ? node->getParent()->getName() : "???");

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке данных:" << query.lastError().text();
            return;
        }
    }

    qDebug() << "Структура успешно сохранена в базу данных";
}
