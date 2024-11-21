#include "CelestialDelegate.h"

CelestialDelegate::CelestialDelegate(QObject *parent) : QStyledItemDelegate(parent) {

}

CelestialDelegate::~CelestialDelegate() {

}

void CelestialDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return;
    }

    if (index.column() == 0) {
        painter->save();

        painter->setRenderHint(QPainter::Antialiasing);

        // Получаем данные из соответствующих столбцов
        QVariant colorData = index.sibling(index.row(), 2).data(Qt::DisplayRole); // Цвет из столбца 2
        QVariant radiusData = index.sibling(index.row(), 3).data(Qt::DisplayRole); // Радиус из столбца 3

        QColor color = colorData.value<QColor>();
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

QWidget* CelestialDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    Q_UNUSED(option);
    QWidget *wgt = new QWidget(parent);

    QLineEdit *editName = new QLineEdit(wgt);
    editName->setObjectName("editName");

    QLineEdit *editType = new QLineEdit(wgt);
    editType->setObjectName("editType");

    QLineEdit *editInfo = new QLineEdit(wgt);
    editInfo->setObjectName("editInfo");

    QVBoxLayout *layout = new QVBoxLayout(wgt);
    layout->addWidget(editName);
    layout->addWidget(editType);
    layout->addWidget(editInfo);

    layout->setContentsMargins(0, 0, 0, 0);

    return wgt;
}

void CelestialDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QWidget *wgt = qobject_cast<QWidget*>(editor);
    if (!wgt) {
        return;
    }

    // Получаем текстовые поля из редактора
    QLineEdit *editName = wgt->findChild<QLineEdit*>("editName");
    QLineEdit *editType = wgt->findChild<QLineEdit*>("editType");
    QLineEdit *editInfo = wgt->findChild<QLineEdit*>("editInfo");

    if (!editName || !editType || !editInfo) {
        return;
    }

    // Устанавливаем данные из МОДЕЛИ в поля
    editName->setText(index.sibling(index.row(), 0).data(Qt::EditRole).toString());
    editType->setText(index.sibling(index.row(), 1).data(Qt::EditRole).toString());
    editInfo->setText(index.sibling(index.row(), 2).data(Qt::EditRole).toString());
}

void CelestialDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QWidget *wgt = qobject_cast<QWidget*>(editor);
    if (!wgt) {
        return;
    }

    // Получаем текстовые поля из редактора
    QLineEdit *editName = wgt->findChild<QLineEdit*>("editName");
    QLineEdit *editType = wgt->findChild<QLineEdit*>("editType");
    QLineEdit *editInfo = wgt->findChild<QLineEdit*>("editInfo");

    if (!editName || !editType || !editInfo) {
        return;
    }

    // Сохраняем ДАННЫЕ в модель
    model->setData(index.sibling(index.row(), 0), editName->text(), Qt::EditRole);
    model->setData(index.sibling(index.row(), 1), editName->text(), Qt::EditRole);
    model->setData(index.sibling(index.row(), 2), editName->text(), Qt::EditRole);
}

void CelestialDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
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
