#include "CelestialBodyModel.h"

/*
 * 1. В методах модели (data, rowCount, parent, index)
 * обращайся к своей структуре данных для получения информации
 * о дереве
 * 2. Подключить модель к QTreeView для отображения
*/

CelestialBodyModel::CelestialBodyModel(QObject *parent) : QAbstractItemModel(parent) {
    setupTestModel();
}

CelestialBodyModel::~CelestialBodyModel() {

}

QVariant CelestialBodyModel::data(const QModelIndex &index, int role) const {
    // Проверяем валидность индекса
    if (!index.isValid()) {
        return QVariant();
    }

    // Получаем указатель на узел, связанный с данным индексом
    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(index.internalPointer());
    if (!node) {
        return QVariant();
    }

    // Обрабатываем различные роли
    switch (role) {
        case Qt::DisplayRole: // Отображение текста
            if (index.column() == 0) { // название небесного тела
                return node->getName();
            }
            else if (index.column() == 1) { // тип небесного тела
                return node->getType();
            }
            else if (index.column() == 2) { // цвет небесного тела
                return node->getColor();
            }
            else if (index.column() == 3) { // радиус небесного тела
                return node->getRadius();
            }
            break;
        case Qt::EditRole: // Редактирование
            if (index.column() == 0) {
                return node->getName();
            }
            else if (index.column() == 1) {
                return node->getType();
            }
            else if (index.column() == 2) { // цвет небесного тела
                return node->getInfo();
            }
            else if (index.column() == 3) { // радиус небесного тела
                return node->getRadius();
            }
            break;
        default:
            break;
    }

    return QVariant(); // если роль не поддерживается, возвращаем пустой QVariant
}

bool CelestialBodyModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }

    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(index.internalPointer());
    if (!node) {
        return false;
    }

    switch (role) {
        case Qt::EditRole:
            node->setName(value.toString());
            break;
        case Qt::EditRole + 1:
            node->setType(value.toString());
            break;
        case Qt::EditRole + 2:
            node->setInfo(value.toString());
            break;
        default:
            return false;
    }

    emit dataChanged(index, index, {role}); // уведомляем предстваление об изменении данных
    return true;
}

int CelestialBodyModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) {
        // Корень не имеет родителя, возвращаем 1 для самого себя
        return 1;
    }

    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(parent.internalPointer());
    return node ? node->childCount() : 0;
}

QModelIndex CelestialBodyModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(child.internalPointer());
    if (!node || node == m_rootNode) {
        return QModelIndex(); // Корень не имеет родителя
    }

    CelestialBodyNode *parentNode = node->getParent();
    if (parentNode == m_rootNode) {
        return createIndex(0, 0, m_rootNode); // Родитель для уровня 1 - корень
    }

    CelestialBodyNode *grandParentNode = parentNode->getParent();
    int row = grandParentNode ? grandParentNode->getListChildren().indexOf(parentNode) : 0;

    return createIndex(row, 0, parentNode);
}

QModelIndex CelestialBodyModel::index(int row, int column, const QModelIndex &parent) const {
    if (row < 0 || column < 0 || column >= columnCount(parent)) {
        return QModelIndex();
    }

    // Если parent невалиден, возвращаем корень как первый узел
    if (!parent.isValid()) {
        if (row == 0) {
            return createIndex(row, column, m_rootNode); // Создаем индекс для корня
        }
        return QModelIndex();
    }

    CelestialBodyNode *parentNode = static_cast<CelestialBodyNode*>(parent.internalPointer());
    if (!parentNode || row >= parentNode->childCount()) {
        return QModelIndex();
    }

    CelestialBodyNode *childNode = parentNode->getChild(row);
    return createIndex(row, column, childNode);
}

int CelestialBodyModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Количество колонок одинаковое для всех узлов
    return 4;
}


void CelestialBodyModel::setupTestModel() {
    m_rootNode = new CelestialBodyNode(QString("KMA-2005"), QString("Светило"));
    CelestialBodyNode *planet1 = new CelestialBodyNode("Planet_1", "Планета", m_rootNode);
    CelestialBodyNode *planet2 = new CelestialBodyNode("Planet_2", "Планета", m_rootNode);
    CelestialBodyNode *sputnik1 = new CelestialBodyNode("Sputnik_1", "Спутник", planet1);
    CelestialBodyNode *sputnik2 = new CelestialBodyNode("Sputnik_1", "Спутник", planet2);
    CelestialBodyNode *sputnik__ = new CelestialBodyNode("Sputnik__", "Спутние", planet1);

    qreal r1 = 5, r2 = 3, r_m = 10;
    planet1->setColor(QColor(Qt::blue));
    planet1->setRadius(r1);
    planet2->setColor(QColor(Qt::blue));
    planet2->setRadius(r1);

    sputnik1->setColor(QColor(Qt::gray));
    sputnik1->setRadius(r2);
    sputnik2->setColor(QColor(Qt::green));
    sputnik2->setRadius(r2);
    sputnik__->setColor(QColor(Qt::red));
    sputnik__->setRadius(r2);

    m_rootNode->setColor(QColor(Qt::yellow));
    m_rootNode->setRadius(r_m);
}
