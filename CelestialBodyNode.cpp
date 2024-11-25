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
    if (m_parent && m_parent->getType() != "fake") { // Проверка, что это не фиктивный узел
        return m_parent->getListChildren().indexOf(const_cast<CelestialBodyNode*>(this));
    }
    return -1;
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
