#include <QApplication>
#include <QStringListModel>
#include <QListView>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QListView>
#include <modeltest.h>

#include "CelestialBodyModel.h"
#include "CelestialDelegate.h"
#include "myTreeView.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //QTreeView *view = new QTreeView;
    MyTreeView *view = new MyTreeView;
    CelestialBodyModel *model = new CelestialBodyModel();
    CelestialDelegate *delegate = new CelestialDelegate();
    // ModelTest tester(model); // подключение ModelTest к модели

    view->setModel(model);
    view->setItemDelegate(delegate);

    view->setWindowTitle("Celestial Body System");
    view->setMinimumSize(480, 640);

    view->header()->hide(); // скрываем заголовок-бар с номерами колонок
    // view->setRootIsDecorated(false); // для корня
    view->setExpandsOnDoubleClick(false); // отключаем стрелки для дочерних узлов
    view->setUniformRowHeights(true); // отключаем неравномерные высоты строк
    view->setEditTriggers(QAbstractItemView::DoubleClicked); // включает редактирование при двойном клике

    // отключаем отображение стрелок для дочерних элементов
    view->setIndentation(0); // устанавливаем отступ в 0 для всех элементов

    view->expandAll();
    view->show();

    return app.exec();
}

