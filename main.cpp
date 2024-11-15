#include <QApplication>
#include <QStringListModel>
#include <QListView>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QStringList dataList = {"Item 1", "Item 2", "Item 3"};

    QStringListModel model;
    model.setStringList(dataList);

    QListView view;
    view.setModel(&model); // Устанавливаем модель в представление
    view.show();

    return app.exec();
}
