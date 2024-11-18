#pragma once

#include <QAbstractItemModel>
#include "CelestialBodyNode.h"

/*
 * Настраивается структура данных: Светило -> планета -> спутник
 * Реализуются методы доступа (data, setData, rowCount, columnCount, parent, index и тд.)
 * Добавляется логика для добавления, редактирования и удаления элементов
*/

class CelestialBodyModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit CelestialBodyModel(QObject *parent = nullptr);
    virtual ~CelestialBodyModel() override;

    /*
     * data - возвращает данные для отображения в QTreeView или
     * другом виджете связанном с моделью
     * Параметры:
     *  index - указывает на конктреный элемент модели (строку и колонку)
     *  role - определяет, какие именно данные требуются. Например:
     *      Qt::DisplayRole - текст, который отображается в ячейке
     *      Qt::EditRole - данные, используемые для редактирования
     * Внутри этой функции опредляется, что будет возвращено для
     * конкретного узла (например, название планеты, тип небесного
     * тела и т.д.)
    */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /*
     * setData - устанавливает новые данные для элемента модели.
     * Параметры:
     *  index - указывает на элемент, который нужно изменить
     *  value - новое значение
     *  role - роль данных, которые изменяются
     * Эта функция отвечает за сохранение изменений в модель
    */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    /*
     * rowCount - возвращает количество строк для определенного элемента (или узла) модели
     * Параметры:
     *  parent - родительский элемент. Если он пустой (QModelIndex()),
     *  возвращает количество корневых элементов.
     * Использование:
     *  У корня (светила) строк будет столько, сколько планет
     *  У узла (планеты) будет столько строк, сколько спутников
    */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /*
     * parent - определяет родителя для узла (или элемента)
     * Параметры:
     *  child - элемент, для которого нужно найти родителя
     * Использование:
     *  Для спутника родителем будет планета
     *  Для планеты родителем будет светило
     *  Для корня (светила) родителя нет, поэтому возврашается пустой QModelIndex
    */
    QModelIndex parent(const QModelIndex &child) const override;

    /*
     * index - создает и возвращает QModelIndex для конкретной строки и колонки. Этот
     * индекс связывает виджет с данными.
     * Параметры:
     *  row - строка
     *  column - колонка
     *  parent - родительский узел
     * Использование:
     *  Эта функция используется для создания индексов, чтобы виджет
     *  мог получить доступ к данным модели
    */
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setupTestModel();
private:
    CelestialBodyNode *m_rootNode;
};
