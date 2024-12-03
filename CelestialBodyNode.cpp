#include "CelestialBodyNode.h"

CelestialBodyNode::CelestialBodyNode() {
    m_name = "???";
    m_type = "???";
    m_parent = nullptr;
}

CelestialBodyNode::CelestialBodyNode(const QString &name, const QString &type)
    : m_name(name), m_type(type) {

}

CelestialBodyNode::CelestialBodyNode(const QString &name, const QString &type, CelestialBodyNode *parent)
    : m_name(name), m_type(type), m_parent(parent) {
    if (m_parent) {
        m_parent->addChild(this); // Автоматически добавляем себя в список дочерних небесных тел
    }
}

CelestialBodyNode::~CelestialBodyNode() {

}

CelestialBodyNode* CelestialBodyNode::createHeirarchyFromDataBase(DataBase &__db__) {
    QList<QVariantMap> celestialBodies = __db__.fetchAll();

    // Карта для хранения узлов по имени
    QMap<QString, CelestialBodyNode*> nodeMap;
    CelestialBodyNode *root = new CelestialBodyNode("KMA-2005", "Светило");
    root->setColor(Qt::yellow);
    root->setRadius(28.0);

    for (const auto &row : celestialBodies) {
        // создаем узел
        CelestialBodyNode *node = new CelestialBodyNode(
            row["name"].toString(),
            row["type"].toString()
        );

        node->setInfo(row["info"].toString());
        node->setColor(QColor(row["hex_color"].toString()));
        node->setRadius(row["radius"].toDouble());

        // Добавляем узел в карту
        QString name = row["name"].toString();
        nodeMap[name] = node;

        // Привязываем к родителю, если указан parent
        QString parentName = row["parent"].toString();
        if (parentName == "KMA-2005") {
            root->addChild(node); // Добавляем планету к светилу
        }
        else {
            nodeMap[parentName]->addChild(node);
        }
    }

    return root;
}

void CelestialBodyNode::addChild(CelestialBodyNode *child) {
    if (child) {
        child->m_parent = this;
        m_children.append(child);
    }
}

void CelestialBodyNode::addChild(CelestialBodyNode *child, int index) {
    if (child) {
        child->m_parent = this;
        if (index <= m_children.size() && index >= 0) {
            m_children.insert(index, child);
        }
    }
}

QList<CelestialBodyNode*> CelestialBodyNode::getListChildren() const {
    return m_children;
}

void CelestialBodyNode::setName(const QString &name) {
    m_name = name;
}

void CelestialBodyNode::setType(const QString &newType) {
    if (m_type != newType) {
        m_type = newType;
    }
}

void CelestialBodyNode::clearChildren() {
    for (int i = 0; i < m_children.size(); ++i) {
        CelestialBodyNode *child = m_children[i];
        child->setParent(nullptr);
    }
    m_children.clear();
}

int CelestialBodyNode::row() const {
    if (m_parent && m_parent->getType() != "fake") {
        int rowIndex = m_parent->getListChildren().indexOf(const_cast<CelestialBodyNode*>(this));
        if (rowIndex == -1) {
            qWarning() << "Ошибка: Узел не найден в родительском списке! Узел:" << m_name;
        }
        return rowIndex;
    }
    return 0;
}

void CelestialBodyNode::setParent(CelestialBodyNode *parent) {
    m_parent = parent;
}

void CelestialBodyNode::setInfo(const QString &info) {
    m_info = info;
}

QString CelestialBodyNode::getName() const {
    return m_name;
}

QString CelestialBodyNode::getType() const {
    return m_type;
}

QString CelestialBodyNode::getInfo() const {
    return m_info;
}

void CelestialBodyNode::removeChild(CelestialBodyNode *child) {
    m_children.removeOne(child);
    child->setParent(nullptr);
}

int CelestialBodyNode::childCount() const {
    return m_children.size();
}

CelestialBodyNode* CelestialBodyNode::getParent() {
    // qDebug() << "Проверка" << m_parent->getName();
    if (!m_parent) {
        qDebug() << "пытаемся вернуть несуществующего родителя!";
    }
    return m_parent;
}

CelestialBodyNode* CelestialBodyNode::getChild(int row) {
    return m_children.at(row);
}

void CelestialBodyNode::setColor(const QColor &color) {
    m_color = color;
}

QColor CelestialBodyNode::getColor() const {
    return m_color;
}

void CelestialBodyNode::setRadius(const qreal &radius) {
    m_radius = radius;
}

qreal CelestialBodyNode::getRadius() const {
    return m_radius;
}
