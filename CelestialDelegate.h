#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QColorDialog>
#include <QComboBox>
#include <QPair>

#include "CelestialBodyNode.h"
#include "CelestialBodyModel.h"

class CelestialDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit CelestialDelegate(QObject *parent = nullptr);
    ~CelestialDelegate();

    /*
     * paint - позволяет кастомизировать, как данные отображаются
     * (например, изменить стиль текста или нарисовать прогресс-бар)
    */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /*
     * createEditor - создает виджет для редактирования (например,
     * QLineEdit, QComboBox, QSpinBox и т.д.)
    */
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    /*
     * setEditorData - загружает данные из МОДЕЛИ в РЕДАКТОР
    */
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    /*
     * setModelData - передает данные из редактора обратно в модель
    */
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    /*
     * updateEditorGeometry - метод для верного геометрического места представления
    */
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    /*
     * sizeHint - настраивает размер ячеек
    */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:

};
