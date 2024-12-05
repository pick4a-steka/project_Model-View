#include <QApplication>
#include <QStringListModel>
#include <QListView>
#include <QWidget>
#include <QTreeView>
#include <QHeaderView>
#include <QListView>
#include <modeltest.h>
#include <QHBoxLayout>
#include <QFontDatabase>

#include "CelestialBodyModel.h"
#include "CelestialDelegate.h"
#include "myTreeView.h"
#include "info_widget.h"
#include "BindWidget.h"

#define W 480
#define H 640

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString info_text = QString("Светило системы KMA, неизведанный и далекий источник света и энергии в глубинах черной космической бездны");

    // QWidget *bindWidget = new QWidget();
    BindWidget *bindWidget = new BindWidget();
    bindWidget->setWindowTitle("The Interactive Guide");
    bindWidget->setMinimumSize(W * 2, H); // размер родительского окна

    QHBoxLayout *horizontal_box = new QHBoxLayout(bindWidget);
    InfoWidget *iw = new InfoWidget(W, H, "KMA-2005", "Светило", info_text, Qt::yellow, bindWidget);

    //QTreeView *view = new QTreeView;
    MyTreeView *view = new MyTreeView(bindWidget);

    horizontal_box->addWidget(view, 1); // равномерное распределение пространства
    horizontal_box->addWidget(iw, 1);

    CelestialBodyModel *model = new CelestialBodyModel(view, iw, bindWidget);
    CelestialDelegate *delegate = new CelestialDelegate();
    // ModelTest tester(model); // подключение ModelTest к модели

    view->setModel(model);
    bindWidget->setModel(model);
    view->setItemDelegate(delegate);
    view->setMinimumSize(W, H);

    view->header()->hide(); // скрываем заголовок-бар с номерами колонок
    // view->setRootIsDecorated(false); // для корня
    view->setExpandsOnDoubleClick(false); // отключаем стрелки для дочерних узлов
    view->setUniformRowHeights(true); // отключаем неравномерные высоты строк
    view->setEditTriggers(QAbstractItemView::DoubleClicked); // включает редактирование при двойном клике

    // отключаем отображение стрелок для дочерних элементов
    view->setIndentation(0); // устанавливаем отступ в 0 для всех элементов

    view->expandAll();
    bindWidget->show();

    //QFontDatabase fd;
    //qDebug() << "Доступные шрифты:" << fd.families();

    return app.exec();
}

