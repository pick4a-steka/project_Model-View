#pragma once

#include <QAbstractItemModel>
#include <QObject>
#include <QHBoxLayout>

#include "CelestialBodyNode.h"
#include "database.h"
#include "myTreeView.h"
#include "info_widget.h"

/*
 * Настраивается структура данных: Светило -> планета -> спутник
 * Реализуются методы доступа (data, setData, rowCount, columnCount, parent, index и тд.)
 * Добавляется логика для добавления, редактирования и удаления элементов
*/

class MyTreeView;

class CelestialBodyModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit CelestialBodyModel(MyTreeView *view, InfoWidget *iw, QWidget *bindWidget, QObject *parent = nullptr);
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

    /*
     * insertRow - метод для добавления новых объектов
     * Параметры:
     *  row - строка
     *  column - колонка
     *  parent - родительский узел
     * Использование:
     *  Метод используется для добавление в представление и структуру
     *  новых небесных тел
    */
    bool insertRow_(int row, const QString type, const QModelIndex &parent = QModelIndex());

    /*
     * removeRow - метод для удаления объектов
     * Параметры:
     *  row - строка
     *  column - колонка
     *  parent - родительский узел
     * Использование:
     *  Метод используется для удаления из структуры
     *  и представления небесных тел
    */
    bool removeRow_(int row, const QModelIndex &parent = QModelIndex());

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setupTestModel();

    QModelIndex indexForNode(CelestialBodyNode *node) const;

    void updateHierarchy(CelestialBodyNode *node, const QString &oldType, const QString &newType);
    int findValidPlaceForNewPlanet(const QString &deleteName);
    CelestialBodyNode* findValidParentForSputnik(CelestialBodyNode *node);
    void debugPrintTree(CelestialBodyNode *node, int depth = 0) const;
    void printRows(const QModelIndex &parent = QModelIndex(), int level = 0) const;
    void structurePrepare(CelestialBodyNode *node, int level, QList<CelestialBodyNode*> &celestialBodies) const;
    int searchPlanet(const QString &name, const QString &type) const;
    QPair<int, int> searchSputnik(const QString &name, const QString &type) const;
    CelestialBodyNode* getRoot() const;

    /*
     * flags - чтобы модель поддерживала редактирование этот метод должен возвращать
     * флаг Qt::ItemIsEditable для редактируемых ячеек
    */
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void newInfoWidget(const QModelIndex &index);

signals:
    void structureChanged(); // сигнал об изменение иерархии

private:
    CelestialBodyNode *m_rootNode;
    CelestialBodyNode *m_fakeRoot;
    InfoWidget *iw;
    QWidget *bW;

    DataBase db;
    QList<CelestialBodyNode*> celestialBodies;
};
