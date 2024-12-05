#include "CelestialBodyModel.h"
#include "myTreeView.h"

/*
 * 1. В методах модели (data, rowCount, parent, index)
 * обращайся к своей структуре данных для получения информации
 * о дереве
 * 2. Подключить модель к QTreeView для отображения
*/

CelestialBodyModel::CelestialBodyModel(MyTreeView *view, InfoWidget *iw, QWidget *bindWidget, QObject *parent) : QAbstractItemModel(parent), \
    iw(iw), bW(bindWidget) {
    db = DataBase(QString("celestial"));
    setupTestModel();

    connect(view, &QAbstractItemView::clicked, this, &CelestialBodyModel::newInfoWidget);
}

CelestialBodyModel::~CelestialBodyModel() {
    structurePrepare(m_rootNode, 0, celestialBodies);
    db.saveInDataBase(celestialBodies);
    delete m_fakeRoot;
}

void CelestialBodyModel::newInfoWidget(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    // Получаем данные о небесном теле из модели
    auto celestialBodyNode = static_cast<CelestialBodyNode*>(index.internalPointer());
    if (!celestialBodyNode) {
        return;
    }

    QString name = celestialBodyNode->getName();
    QString type = celestialBodyNode->getType();
    QColor color = celestialBodyNode->getColor();
    QString info = celestialBodyNode->getInfo();

    iw->setDataOfCelestial(color, name, type, info);
    //QPaintEvent *plug = nullptr;
    //iw->paintEvent(plug);
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
            else if (index.column() == 2) { // информация
                return node->getInfo();
            }
            else if (index.column() == 3) { // радиус небесного тела
                return node->getRadius();
            }
            else if (index.column() == 4) { // цвет
                return node->getColor();
            }
            break;
        case Qt::EditRole: // Редактирование
            if (index.column() == 0) {
                return node->getName();
            }
            else if (index.column() == 1) {
                return node->getType();
            }
            else if (index.column() == 2) { // информация небесного тела
                return node->getInfo();
            }
            else if (index.column() == 3) { // радиус небесного тела
                return node->getRadius();
            }
            else if (index.column() == 4) { // цвет небесного тела
                return node->getColor();
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
            if (index.column() == 0) {
                qDebug() << "Имя изменяем на" << value.toString();
                node->setName(value.toString());
            }
            else if (index.column() == 1) {
                printRows();
                QString oldType = node->getType();
                QString newType = value.toString();

                if (oldType != newType) {
                    node->setType(newType);
                    updateHierarchy(node, oldType, newType);
                    return true;
                }
            }
            else if (index.column() == 2) {
                node->setInfo(value.toString());
            }
            else if (index.column() == 3) {
                qDebug() << "изменяем радиус на" << value.toDouble();
                node->setRadius(value.toDouble());
            }
            else if (index.column() == 4) {
                QColor newColor = value.value<QColor>();
                if (newColor.isValid()) {
                    qDebug() << "изменяем цвет на" << newColor;
                    node->setColor(newColor);
                    printRows();
                }
                else {
                    return false;
                }
            }
            break;
        case Qt::UserRole:
            if (index.column() == 0) {
                node->setName(value.toString());
            }
            else if (index.column() == 1) {
                node->setType(value.toString());
            }
            else if (index.column() == 2) {
                node->setInfo(value.toString());
            }
            else if (index.column() == 3) {
                qDebug() << "Устанавливаемый радиус" << value.toDouble();
                node->setRadius(value.toDouble());
            }
            else if (index.column() == 4) {
                QColor color = value.value<QColor>();
                if (color.isValid()) {
                    node->setColor(color);
                }
                //printRows();
            }
        default:
            return false;
    }

    emit dataChanged(index, index, {role}); // уведомляем предстваление об изменении данных
    return true;
}

int CelestialBodyModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) {
        return 0; // В модели только первая колонка может иметь дочерние узлы
    }

    CelestialBodyNode *parentNode = parent.isValid()
        ? static_cast<CelestialBodyNode*>(parent.internalPointer())
        : m_fakeRoot;

    return parentNode ? parentNode->getListChildren().size() : 0;
}

QModelIndex CelestialBodyModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    CelestialBodyNode *childNode = static_cast<CelestialBodyNode*>(child.internalPointer());
    CelestialBodyNode *parentNode = childNode->getParent();

    // Если у узла нет родителя или он корневой
    if (!parentNode || parentNode == m_fakeRoot) {
        return QModelIndex();
    }

    // Возвращаем индекс родителя
    int row = parentNode->row();
    return createIndex(row, 0, parentNode);
}

QModelIndex CelestialBodyModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    CelestialBodyNode *parentNode = parent.isValid()
        ? static_cast<CelestialBodyNode*>(parent.internalPointer())
        : m_fakeRoot;

    if (!parentNode || row >= parentNode->getListChildren().size()) {
        return QModelIndex();
    }

    CelestialBodyNode *childNode = parentNode->getListChildren().at(row);

    //qDebug() << "Index parent:" << parent;

    // Создание уникального индекса
    return createIndex(row, column, childNode);
}

bool CelestialBodyModel::insertRow_(int row, const QString type, const QModelIndex &parent) {
    beginInsertRows(parent, row, row);

    // Получаем родительский узел
    CelestialBodyNode *parentNode = static_cast<CelestialBodyNode*>(parent.internalPointer());
    if (!parentNode) {
        return false;
    }

    // Создаем новый узел с базовыми параметрами
    /// автоматическое добавление в список дочерних элементов в конструкторе
    CelestialBodyNode *newNode = new CelestialBodyNode("Небесное тело", type, parentNode);
    //newNode->set

    qDebug() << "Вывод дерева из insertRow:";    
    debugPrintTree(m_fakeRoot, 0);

    endInsertRows();
    emit layoutChanged();
    return true;
}

bool CelestialBodyModel::removeRow_(int row, const QModelIndex &parent) {
    beginRemoveRows(parent, row, row);

    // Получаем родительский узел
    CelestialBodyNode *parentNode = static_cast<CelestialBodyNode*>(parent.internalPointer());
    if (!parentNode) {
        return false;
    }

    // Удаляем узел
    CelestialBodyNode *removeNode = parentNode->getChild(row);

    if (removeNode->getType() == "Планета") {
        removeNode->clearChildren();
    }

    parentNode->removeChild(removeNode);
    delete removeNode;

    endRemoveRows();
    return true;
}

int CelestialBodyModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent); // Количество колонок одинаковое для всех узлов
    return 5;
}

Qt::ItemFlags CelestialBodyModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    if (index.isValid()) {
        return defaultFlags | Qt::ItemIsEditable; // добавляем возможность редактирования
    }

    return defaultFlags;
}

void CelestialBodyModel::updateHierarchy(CelestialBodyNode *node, const QString &oldType, const QString &newType) {
    if (!node || oldType == newType) {
        return;
    }

    QModelIndex oldParentIndex = indexForNode(node->getParent());
    //qDebug() << "Родитель у которого будем удалять узел:" << node->getParent()->getName();
    int oldRow = node->getParent()->getListChildren().indexOf(node);

    if (newType != "Спутник" || m_rootNode->childCount() != 1) {
        // Уведомляем представление об удалении узла
        beginRemoveRows(oldParentIndex, oldRow, oldRow);
        qDebug() << "Уведомили об удалении";
    }

    //qDebug() << "Row для удаления:" << oldRow;
    //qDebug() << "Что удаляем:" << node->getParent()->getListChildren().at(oldRow)->getName();

    CelestialBodyNode *newParent = nullptr;
    int newRow = 0;

    // Находим нового родителя
    if (newType == "Планета") {
        newRow = findValidPlaceForNewPlanet(node->getName());
        newParent = m_rootNode;
    }
    else if (newType == "Спутник") {
        newParent = findValidParentForSputnik(node);
        newRow = newParent ? newParent->getListChildren().size() : 0;

        if (!newParent) {
            //qDebug() << "Невозможно удалить единственную планету";
            node->setType("Планета");
            printRows();
            return;
        }
    }

    //qDebug() << "row было было" << node->row();

    node->getParent()->removeChild(node);
    endRemoveRows();

    printRows();

    if (newParent) {
        // уведомляем представление о добавлении узла
        //qDebug() << "Узел который будет добавляться" << node->getName();
        //qDebug() << "Новый родитель для узла" << newParent->getName();
        //qDebug() << "Row в который добавим узел" << newRow;
        if (!newParent) {
            //qDebug() << "Ошибка: newParent равен nullptr!";
            return;
        }

        QModelIndex newIndex = indexForNode(newParent);
        if (!newIndex.isValid() && newParent) {
            //qDebug() << "Ошибка: indexForNode() вернул невалидный индекс!";
            return;
        }

        if (newRow < 0 || newRow > rowCount(newIndex)) {
            //qDebug() << "Ошибка: newRow выходит за допустимые пределы!";
            return;
        }
        beginInsertRows(newIndex, newRow, newRow);
        //qDebug() << "ПОСТАВИЛИ NEWROW:" << newRow;
        //qDebug() << "РОДИТЕЛЬ ОТНОСИТЕЛЬНО КОТОРОГО РАБОТАЕТ ROW:" << static_cast<CelestialBodyNode*>(newIndex.internalPointer())->getName();

        //qDebug() << "row было" << node->row();
        newParent->addChild(node, newRow);
        //qDebug() << "newIndex isValid:" << newIndex.isValid();
        //qDebug() << "Parent for newIndex:" << (newParent->getParent() ? newParent->getParent()->getName() : "nullptr");
        //qDebug() << "newParent children count:" << newParent->childCount();
        //qDebug() << "Current state of tree structure:";
        //printTreeStructure(m_fakeRoot, 0); // Реализуйте функцию для вывода структуры дерева

        endInsertRows();
        //qDebug() << "row теперь" << node->row();

        //printRows();
    }
}

int CelestialBodyModel::searchPlanet(const QString &name, const QString &type) const {
   // QList<CelestialBodyNode*> planets = m_rootNode->getListChildren();

    for (int i = 0; i < m_rootNode->childCount(); ++i) {
        if (m_rootNode->getChild(i)->getName() == name && m_rootNode->getChild(i)->getType() == type) {
            return i;
        }
    }

    return -1;
}

QPair<int, int> CelestialBodyModel::searchSputnik(const QString &name, const QString &type) const {
    QList<CelestialBodyNode*> planets = m_rootNode->getListChildren();

    for (int i = 0; i < m_rootNode->childCount(); ++i) {
        for (int j = 0; j < planets[i]->childCount(); ++j) {
            if (planets[i]->getChild(j)->getName() == name && planets[i]->getChild(j)->getType() == type) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

CelestialBodyNode* CelestialBodyModel::getRoot() const {
    return m_rootNode;
}

int CelestialBodyModel::findValidPlaceForNewPlanet(const QString &deleteName) {
    QList<CelestialBodyNode*> planets = m_rootNode->getListChildren();
    for (int j = 0; j < m_rootNode->childCount(); ++j) {
        for (int i = 0; i < planets[j]->childCount(); ++i) {
            if (planets[j]->getChild(i)->getName() == deleteName) {
                return j;
            }
        }
    }

    return -1;
}

CelestialBodyNode* CelestialBodyModel::findValidParentForSputnik(CelestialBodyNode *node) {
    if (!node || !node->getParent()) {
        return nullptr;
    }

    CelestialBodyNode *parent = node->getParent();
    QList<CelestialBodyNode*> siblings = parent->getListChildren();
    int index = siblings.indexOf(node);

    // Попробуем найти ближайших соседей (слева или справа)
    if (index > 0) {
        CelestialBodyNode *leftSibling = siblings.at(index - 1);
        if (leftSibling->getType() == "Планета") {
            //qDebug() << "Новый родитель для бывшей планеты" << leftSibling->getName();
            node->clearChildren();
            return leftSibling;
        }
    }

    if (index < siblings.size() - 1) {
        CelestialBodyNode *rightSibling = siblings.at(index + 1);
        if (rightSibling->getType() == "Планета") {
            //qDebug() << "Новый родитель для бывшей планеты" << rightSibling->getName();
            node->clearChildren();
            return rightSibling;
        }
    }

    return nullptr; // Новый родитель не найден
}

QModelIndex CelestialBodyModel::indexForNode(CelestialBodyNode *node) const {
    if (!node) {
        //qDebug() << "Ошибка: node == nullptr";
        return QModelIndex(); // Возвращаем пустой индекс, если узел null
    }
    if (node == m_fakeRoot) {
        //qDebug() << "Индекс фиктивного корня возвращается как пустой.";
        return QModelIndex(); // Возвращаем пустой индекс для фиктивного корня
    }

    CelestialBodyNode *parent = node->getParent();
    if (!parent) {
        //qDebug() << "Ошибка: У узла" << node->getName() << "нет родителя.";
        return QModelIndex();
    }

    // Проверяем список детей родителя
    const auto &children = parent->getListChildren();
    int row = children.indexOf(const_cast<CelestialBodyNode*>(node));
    if (row == -1) {
        //qDebug() << "Ошибка: У родителя" << parent->getName() << "нет такого ребёнка:" << node->getName();
        return QModelIndex();
    }

    // Проверяем корректность указателей
    //qDebug() << "Индекс создаётся для узла" << node->getName() << "с родителем" << parent->getName() << "и row =" << row;
    QModelIndex index = createIndex(row, 0, node);
    return index;
}

void CelestialBodyModel::printRows(const QModelIndex &parent, int level) const {
    int rows = rowCount(parent);

    for (int row = 0; row < rows; ++row) {
        QModelIndex currentIndex = index(row, 0, parent);
        CelestialBodyNode *node = static_cast<CelestialBodyNode*>(currentIndex.internalPointer());

        // Печатаем информацию об узле
        if (node) {
            qDebug() << QString(level * 2, ' ') // Отступ для уровней
                     << "Row:" << row
                     << ", Parent:" << node->getParent()->getName()
                     << ", Name:" << node->getName()
                     << ", Type:" << node->getType()
                     << ", Color:" << node->getColor()
                     << ", Radius:" << node->getRadius();
        }

        // Рекурсивно проходим детей
        if (currentIndex.isValid() && rowCount(currentIndex) > 0) {
            printRows(currentIndex, level + 1);
        }
    }
}

void CelestialBodyModel::structurePrepare(CelestialBodyNode *node, int level, QList<CelestialBodyNode*> &celestialBodies) const {
    if (!node) {
        return;
    }

    if (node->getType() != "Светило") {
        celestialBodies.append(node);
    }

    for (CelestialBodyNode *child : node->getListChildren()) {
        structurePrepare(child, level + 1, celestialBodies);
    }
}

void CelestialBodyModel::debugPrintTree(CelestialBodyNode *node, int level) const {
    if (!node) return;  // Если узел пустой, выходим

        // Выводим имя текущего узла с отступами
        QString indent(level * 2, ' ');  // Создаем отступы в зависимости от уровня дерева
        qDebug() << indent << node->getName();

        // Рекурсивно выводим всех детей текущего узла
        for (CelestialBodyNode* child : node->getListChildren()) {
            debugPrintTree(child, level + 1);  // Увеличиваем уровень отступа для дочерних элементов
    }
}

void CelestialBodyModel::setupTestModel() {
    qreal fake_radius = 1;
    m_fakeRoot = new CelestialBodyNode("root", "fake");
    m_fakeRoot->setColor(Qt::black);
    m_fakeRoot->setRadius(fake_radius);

    if (!db.connect("db_celestials", "myuser", "mypassword")) {
        qDebug() << "Ошибка подключения к БД";
        return;
    }

    m_rootNode = m_fakeRoot->createHeirarchyFromDataBase(db);
    m_fakeRoot->addChild(m_rootNode);
}
