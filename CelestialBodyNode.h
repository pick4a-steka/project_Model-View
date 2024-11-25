#pragma once

#include <QString>
#include <QList>
#include <QColor>
#include <QDebug>
#include <stack>

// Класс, описывающий небесное тело
class CelestialBodyNode {
public:
    CelestialBodyNode();
    CelestialBodyNode(const QString &name, const QString &type);
    CelestialBodyNode(const QString &name, const QString &type, CelestialBodyNode *parent);
    virtual ~CelestialBodyNode();

    void addChild(CelestialBodyNode *child);
    void addChild(CelestialBodyNode *child, int index);
    void removeChild(CelestialBodyNode *child);

    CelestialBodyNode* getChild(int row);
    CelestialBodyNode* getParent();
    void setParent(CelestialBodyNode *parent);

    int childCount() const;

    void setName(const QString &name);
    QString getName() const;

    void setType(const QString &type);
    QString getType() const;

    void setInfo(const QString &info);
    QString getInfo() const;

    QList<CelestialBodyNode*> getListChildren() const;

    void setColor(const QColor &color);
    QColor getColor() const;

    void setRadius(const qreal &radius);
    qreal getRadius() const;

    int row() const;

    // void updateHierarchy(CelestialBodyNode *node, const QString &oldType, const QString &newType);

    // int findValidPlaceForNewPlanet(const QString &deleteName);
    // void changePlanetToSputnik(CelestialBodyNode *node);

    void clearChildren();

private:
    QString m_name; // Название для небесного тела
    QString m_type; // Тип небесного тела
    QString m_info; // Для хранения текстовой информации о небесном теле
    QColor m_color; // цвет небесного тела
    int m_radius; // радиус небесного тела
    CelestialBodyNode *m_parent; // Родительское небесное тело
    QList<CelestialBodyNode*> m_children; // Список дочерних небесных тел
};
