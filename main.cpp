#include <QApplication>
#include <QStringListModel>
#include <QListView>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QListView>

#include "CelestialBodyModel.h"
#include "CelestialDelegate.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTreeView *view = new QTreeView;
    CelestialBodyModel *model = new CelestialBodyModel();
    CelestialDelegate *delegate = new CelestialDelegate();

    view->setModel(model);
    view->setItemDelegate(delegate);

    view->setWindowTitle("Celestial Body System");
    view->setMinimumSize(400, 300);

    view->header()->hide(); // скрываем заголовок-бар с номерами колонок
    view->setRootIsDecorated(false); // для корня
    view->setExpandsOnDoubleClick(false); // отключаем стрелки для дочерних узлов
    view->setUniformRowHeights(true); // отключаем неравномерные высоты строк

    // отключаем отображение стрелок для дочерних элементов
    view->setIndentation(0); // устанавливаем отступ в 0 для всех элементов

    view->expandAll();
    view->show();

    return app.exec();
}

