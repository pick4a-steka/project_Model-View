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

QWidget* CelestialDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    Q_UNUSED(index);
    Q_UNUSED(option);
    QWidget *wgt = new QWidget(parent);

    QFrame *frame = new QFrame(wgt);
    frame->setFrameStyle(QFrame::Box | QFrame::Plain);
    frame->setLineWidth(2); // Толщина бордера
    frame->setStyleSheet("background-color: white;"); // Цвет фона

    QLabel *labelName = new QLabel(frame);
    labelName->setText("Название:");
    QLineEdit *editName = new QLineEdit(frame);
    editName->setObjectName("editName");

    /*
    QLabel *labelType = new QLabel(frame);
    labelType->setText("Тип:");
    QLineEdit *editType = new QLineEdit(frame);
    editType->setObjectName("editType");
    */

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
    // color_L->setStyleSheet("background-color: black;");
    //color_L->setProperty("selectedColor", QColor(Qt::black));

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

    qDebug() << "setModelData: " << index.sibling(index.row(), 0).data(Qt::DisplayRole).toString();

    // Получаем текстовые поля из редактора
    QLineEdit *editName = wgt->findChild<QLineEdit*>("editName");
    QComboBox *editType = wgt->findChild<QComboBox*>("editType");
    QLineEdit *editInfo = wgt->findChild<QLineEdit*>("editInfo");
    QDoubleSpinBox *radiusSpinBox = wgt->findChild<QDoubleSpinBox*>("radiusSpinBox");
    QLabel *color_L = wgt->findChild<QLabel*>("color_L");

    if (!editName || !editType || !editInfo || !radiusSpinBox || !color_L) {
        return;
    }

    // qDebug() << "editName: " << editName->text();
    // qDebug() << "editType: " << editType->currentText();

    // Сохраняем ДАННЫЕ в модель
    QString selectedValue = editType->currentText();
    model->setData(index.sibling(index.row(), 1), selectedValue, Qt::EditRole);

    CelestialBodyNode *node = static_cast<CelestialBodyNode*>(index.internalPointer());
    int newRow = node->row();
    qDebug() << "newRow в ДЕЛЕГАТЕ" << newRow;

    model->setData(index.sibling(newRow, 0), editName->text(), Qt::EditRole);
    model->setData(index.sibling(newRow, 2), editInfo->text(), Qt::EditRole);
    qDebug() << "Сейчас мы отправляем изменения объекта:" << index.sibling(index.row(), 0).data(Qt::DisplayRole).toString();
    qDebug() << "index.row в делегате" << index.row();
    qDebug() << "В модель отправляется радиус" << radiusSpinBox->value();
    model->setData(index.sibling(newRow, 3), radiusSpinBox->value(), Qt::EditRole);

    // Извлекаем цвет из QLabel и передаем его в модель
    QVariant colorData = color_L->property("selectedColor");
    if (colorData.isValid() && colorData.canConvert<QColor>()) {
        QColor color = colorData.value<QColor>();
        model->setData(index.sibling(newRow, 4), color, Qt::EditRole);
        qDebug() << "В модель отправляется цвет" << color;
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
