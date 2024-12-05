#include "CelestialDelegate.h"

CelestialDelegate::CelestialDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

CelestialDelegate::~CelestialDelegate() {

}

void CelestialDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return;
    }

    if (index.column() == 2) {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);

        // Получаем данные из соответствующих столбцов
        QVariant colorData = index.sibling(index.row(), 4).data(Qt::DisplayRole); // Цвет из столбца 2
        QVariant radiusData = index.sibling(index.row(), 3).data(Qt::DisplayRole); // Радиус из столбца 3

        QColor color = colorData.value<QColor>();
        if (!color.isValid()) {
            color = QColor(Qt::red); // цвет по умолчанию
        }
        qreal radius = radiusData.value<qreal>();

        painter->setBrush(QBrush(color));
        painter->setPen(Qt::NoPen);

        // Вычисляем центр ячейки
        QRect rect = option.rect;
        QPointF center = rect.center();

        // Рисуем круг
        painter->drawEllipse(center, radius, radius);

        painter->setRenderHint(QPainter::Antialiasing, true);

        painter->restore();
    }
}

/*
 * Имя:
 * ....
 * Тип:
 * ....
 * Добавить | Удалить
 */

QWidget* CelestialDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option);
    QWidget *wgt = new QWidget(parent);

    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(index.internalPointer());

    QFrame *frame = new QFrame(wgt);
    frame->setFrameStyle(QFrame::Box | QFrame::Plain);
    frame->setLineWidth(2); // Толщина бордера
    frame->setStyleSheet("background-color: white;"); // Цвет фона

    if (node->getType() != "Светило") {

        QLabel *labelName = new QLabel(frame);
        labelName->setText("Название:");
        QLineEdit *editName = new QLineEdit(frame);
        editName->setObjectName("editName");

        QLabel *labelType = new QLabel(frame);
        labelType->setText("Тип:");
        QComboBox *editType = new QComboBox(frame);
        editType->setObjectName("editType");
        editType->addItem("Планета");
        editType->addItem("Спутник");

        QLabel *labelInfo = new QLabel(frame);
        labelInfo->setText("Информация:");
        QLineEdit *editInfo = new QLineEdit(frame);
        editInfo->setObjectName("editInfo");

        QLabel *labelColor = new QLabel(frame);
        labelColor->setText("Цвет:");
        QPushButton *btn_color = new QPushButton("Выбрать цвет", frame);
        btn_color->setObjectName("btn_color");
        btn_color->setStyleSheet("background-color: lightgray;");

        // отображение выбранного цвета
        QLabel *color_L = new QLabel(frame);
        color_L->setObjectName("color_L");
        color_L->setFixedSize(50, 20);

        QLabel *labelRadius = new QLabel(frame);
        labelRadius->setText("Радиус");
        QDoubleSpinBox *radiusSpinBox = new QDoubleSpinBox(frame);
        radiusSpinBox->setObjectName("radiusSpinBox");
        radiusSpinBox->setRange(1.0, 20.0); // диапазон для изменения радиуса
        radiusSpinBox->setDecimals(2); // два знака после запятой

        QVBoxLayout *layout = new QVBoxLayout(frame);
        layout->addWidget(labelName);
        layout->addWidget(editName);
        layout->addWidget(labelType);
        layout->addWidget(editType);
        layout->addWidget(labelInfo);
        layout->addWidget(editInfo);
        layout->addWidget(labelRadius);
        layout->addWidget(radiusSpinBox);
        layout->addWidget(labelColor);
        QHBoxLayout *colorLayout = new QHBoxLayout();
        colorLayout->addWidget(btn_color);
        colorLayout->addWidget(color_L);
        layout->addLayout(colorLayout);

        layout->setContentsMargins(5, 5, 5, 5);

        QVBoxLayout *mainEditorLayout = new QVBoxLayout(wgt);
        mainEditorLayout->addWidget(frame);
        mainEditorLayout->setContentsMargins(0, 0, 0, 0);

        // Сигнал для выбора цвета
        connect(btn_color, &QPushButton::clicked, wgt, [color_L]() {
            QColor color = QColorDialog::getColor(Qt::black, nullptr, "Выберите цвет");
            if (color.isValid()) {
                color_L->setStyleSheet(QString("background-color: %1").arg(color.name()));
                color_L->setProperty("selectedColor", color); // Сохраняем цвет как свойство
            }
        });
    }
    else {
        QLabel *labelName = new QLabel(frame);
        labelName->setText("Имя:");
        QLineEdit *editName_addOrdel = new QLineEdit(frame);
        editName_addOrdel->setText("Небесное тело"); // устанавливаем дефолтное значение
        editName_addOrdel->setObjectName("editName_addOrdel");

        QLabel *labelType = new QLabel(frame);
        labelType->setText("Тип:");
        QComboBox *editType_addOrdel = new QComboBox(frame);
        editType_addOrdel->addItem("Планета");
        editType_addOrdel->addItem("Спутник");
        editType_addOrdel->setCurrentIndex(0); // По умолчанию ставим планету
        editType_addOrdel->setObjectName("editType_addOrdel");

        QLabel *labelAddOrDel = new QLabel(frame);
        labelAddOrDel->setText("Добавить/Удалить:");
        QComboBox *editAddOrDel = new QComboBox(frame);
        editAddOrDel->setObjectName("editAddOrDel");
        editAddOrDel->addItem("Добавить");
        editAddOrDel->addItem("Удалить");
        editAddOrDel->setCurrentIndex(0); // По умолчанию ставим добавить

        QVBoxLayout *layout = new QVBoxLayout(frame);
        layout->addWidget(labelName);
        layout->addWidget(editName_addOrdel);
        layout->addWidget(labelType);
        layout->addWidget(editType_addOrdel);
        layout->addWidget(labelAddOrDel);
        layout->addWidget(editAddOrDel);

        layout->setContentsMargins(5, 5, 5, 5);
    }

    return wgt;
}

void CelestialDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QWidget *wgt = qobject_cast<QWidget*>(editor);
    if (!wgt) {
        return;
    }

    // Получаем текстовые поля из редактора
    QLineEdit *editName = wgt->findChild<QLineEdit*>("editName");
    QComboBox *editType = wgt->findChild<QComboBox*>("editType");
    QLineEdit *editInfo = wgt->findChild<QLineEdit*>("editInfo");
    QDoubleSpinBox *radiusSpinBox = wgt->findChild<QDoubleSpinBox*>("radiusSpinBox");
    QLabel *color_L = wgt->findChild<QLabel*>("color_L");

    if (!editName || !editType || !editInfo || !radiusSpinBox || !color_L) {
        return;
    }

    // Устанавливаем данные из МОДЕЛИ в поля
    editName->setText(index.sibling(index.row(), 0).data(Qt::EditRole).toString());

    QString currValue = index.sibling(index.row(), 1).data(Qt::EditRole).toString();
    int combo_index = editType->findText(currValue);
    if (combo_index >= 0) {
        editType->setCurrentIndex(combo_index);
    }
    else {
        QStyledItemDelegate::setEditorData(editor, index);
    }

    editInfo->setText(index.sibling(index.row(), 2).data(Qt::EditRole).toString());
    radiusSpinBox->setValue(index.sibling(index.row(), 3).data(Qt::EditRole).toDouble());

    // Получаем цвет из модели
    QVariant colorData = index.sibling(index.row(), 4).data(Qt::EditRole);
    if (colorData.isValid() && colorData.canConvert<QColor>()) {
        QColor color = colorData.value<QColor>();
        color_L->setStyleSheet(QString("background-color: %1;").arg(color.name())); // устанавливаем цвет фона
        color_L->setProperty("selectedColor", color); // сохраняем цвет как свойство для дальнейшего использования
    }
}

void CelestialDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QWidget *wgt = qobject_cast<QWidget*>(editor);
    if (!wgt) {
        return;
    }

    CelestialBodyNode *celestial = static_cast<CelestialBodyNode*>(index.internalPointer());

    // Приводим модель к типу CelestialBodyModel
    CelestialBodyModel *celestial_model = qobject_cast<CelestialBodyModel*>(model);

    if (celestial->getType() != "Светило") {
        qDebug() << "setModelData: " << index.sibling(index.row(), 0).data(Qt::DisplayRole).toString();

        // Получаем текстовые поля из редактора
        QLineEdit *editName = wgt->findChild<QLineEdit*>("editName");
        QComboBox *editType = wgt->findChild<QComboBox*>("editType");
        QLineEdit *editInfo = wgt->findChild<QLineEdit*>("editInfo");
        QDoubleSpinBox *radiusSpinBox = wgt->findChild<QDoubleSpinBox*>("radiusSpinBox");
        QLabel *color_L = wgt->findChild<QLabel*>("color_L");

        // qDebug() << "Сейчас радиус равен" << radiusSpinBox->value();
        QString _name_ = editName->text();
        QString _info_ = editInfo->text();
        qreal _radius_ = radiusSpinBox->value();
        QVariant _colorData_ = color_L->property("selectedColor");
        QColor _color_;
        if (_colorData_.isValid() && _colorData_.canConvert<QColor>()) {
            _color_ = _colorData_.value<QColor>();
        }

        if (!editName || !editType || !editInfo || !radiusSpinBox || !color_L) {
            return;
        }

        // Сохраняем ДАННЫЕ в МОДЕЛЬ
        QString selectedValue = editType->currentText();
        CelestialBodyNode *node = static_cast<CelestialBodyNode*>(index.internalPointer());

        if (selectedValue != node->getType()) {
            model->setData(index.sibling(index.row(), 1), selectedValue, Qt::EditRole);
            CelestialBodyNode *indexNode = static_cast<CelestialBodyNode*>(index.internalPointer());
            //qDebug() << "index.internalPointer:" <<  indexNode->getName();
            QModelIndex newIndex = celestial_model->indexForNode(indexNode->getParent());

            int newRow = indexNode->row();
            //qDebug() << "newRow в ДЕЛЕГАТЕ" << newRow;

            if (selectedValue == "Спутник" && node->getType() == "Планета") {
                model->setData(index.sibling(index.row(), 1), node->getType(), Qt::EditRole);
            }

            model->setData(model->index(newRow, 0, newIndex), _name_, Qt::EditRole);
            model->setData(model->index(newRow, 2, newIndex), _info_, Qt::EditRole);
            //qDebug() << "Сейчас мы отправляем изменения объекта:" << index.sibling(index.row(), 0).data(Qt::DisplayRole).toString();
            //qDebug() << "index.row в делегате" << index.row();
            //qDebug() << "В модель отправляется радиус" << radiusSpinBox->value();
            model->setData(model->index(newRow, 3, newIndex), _radius_, Qt::EditRole);
            model->setData(model->index(newRow, 4, newIndex), _color_, Qt::EditRole);
        }
        else {
            // Поиск индекса объекта для изменения
            QModelIndex newIndex = celestial_model->indexForNode(celestial_model->getRoot());
            int _row_ = node->row();
            if (selectedValue == "Спутник") {
                QPair<int, int> _rows_ = celestial_model->searchSputnik(_name_, selectedValue);
                CelestialBodyNode *child = celestial_model->getRoot()->getChild(_rows_.first);
                newIndex = celestial_model->indexForNode(child);
                _row_ = _rows_.second;
            }

            model->setData(model->index(_row_, 0, newIndex), _name_, Qt::EditRole);
            model->setData(model->index(_row_, 2, newIndex), _info_, Qt::EditRole);
            model->setData(model->index(_row_, 3, newIndex), _radius_, Qt::EditRole);
            model->setData(model->index(_row_, 4, newIndex), _color_, Qt::EditRole);
        }
    }
    else {
        // Получаем текстовые поля из редактора
        QLineEdit *editName_addOrdel = wgt->findChild<QLineEdit*>("editName_addOrdel");
        QComboBox *editType_addOrdel = wgt->findChild<QComboBox*>("editType_addOrdel");
        QComboBox *editAddOrDel = wgt->findChild<QComboBox*>("editAddOrDel");

        if (!editName_addOrdel || !editType_addOrdel || !editAddOrDel) {
            return;
        }

        QString name = editName_addOrdel->text();
        QString type = editType_addOrdel->currentText();

        if (editAddOrDel->currentText() == "Добавить") {
            // Добавление объекта
            if (editName_addOrdel && !editName_addOrdel->text().isEmpty() && \
                    editType_addOrdel->currentIndex() >= 0) {
                int row = celestial->childCount();
                // QModelIndex newIndex = index;
                qDebug() << "Name of celestials" << celestial->getName();
                QModelIndex newIndex = celestial_model->indexForNode(celestial);
                if (type == "Спутник") {
                    CelestialBodyNode *child = celestial->getChild(row - 1);

                    // Поиск индекса родителя
                    newIndex = celestial_model->indexForNode(child);

                    row = child->childCount();
                }

                //qDebug() << "index.row():" << index.row();

                // Вставка нового объекта
                celestial_model->insertRow_(row, type, newIndex);
                CelestialBodyNode *node = celestial->getChild(row);

                //qDebug() << "node-row():" << node->row();

                // Установка дефолтного цвета для небесного тела
                QColor color = Qt::gray;

                // Устанавливаем данные для нового объекта в модель
                model->setData(model->index(node->row(), 0, newIndex), name, Qt::UserRole);
                model->setData(model->index(node->row(), 1, newIndex), type, Qt::UserRole);
                model->setData(model->index(node->row(), 2, newIndex), QString(""), Qt::UserRole);
                model->setData(model->index(node->row(), 3, newIndex), qreal(4.0), Qt::UserRole);
                model->setData(model->index(node->row(), 4, newIndex), color, Qt::UserRole);
            }
        }
        else {
            if (editName_addOrdel && !editName_addOrdel->text().isEmpty() && \
                    editType_addOrdel->currentIndex() >= 0) {
                if (type == "Планета") {
                    int row = celestial_model->searchPlanet(name, type);
                    //qDebug() << "Удаление элемента row" << row;
                    celestial_model->removeRow_(row, index);
                }
                else {
                    QPair<int, int> rows = celestial_model->searchSputnik(name, type);
                    //qDebug() << "row Планеты" << rows.first << "row Спутника" << rows.second;

                    // Находим индекс планеты родителя для спутника, который собираемся удалить
                    QModelIndex newIndex = celestial_model->indexForNode(celestial_model->getRoot()->getChild(rows.first));
                    celestial_model->removeRow_(rows.second, newIndex);
                }
            }
        }
    }
}

void CelestialDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    const int width = 200, height = 300;
    const int x = (option.widget->width() / 2) - width / 2;
    const int y = (option.widget->height() / 2) - height / 2;

    editor->setGeometry(x, y, width, height);
}

QSize CelestialDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(option);

    if (index.column() == 3) {
        bool ok = false;
        qreal radius = static_cast<qreal>(index.sibling(index.row(), 3).data(Qt::EditRole).toDouble(&ok));

        // Если радиус невалиден
        if (!ok || radius <= 0) {
            return QStyledItemDelegate::sizeHint(option, index);
        }

        // Учитываем радиус и дополнительные отступы
        return QSize(radius * 2 + 20, radius * 2 + 20);
    }

    return QStyledItemDelegate::sizeHint(option, index);
}
