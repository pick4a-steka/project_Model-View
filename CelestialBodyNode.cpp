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

QList<CelestialBodyNode*> CelestialBodyNode::getListChildren() const {
    return m_children;
}

void CelestialBodyNode::setName(const QString &name) {
    m_name = name;
}

void CelestialBodyNode::setType(const QString &type) {
    m_type = type;
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

void CelestialBodyNode::removeChild(int row) {
    if (row >= 0 && row < m_children.size()) {
        delete m_children.takeAt(row);
    }
}

int CelestialBodyNode::childCount() const {
    return m_children.size();
}

CelestialBodyNode* CelestialBodyNode::getParent() {
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
