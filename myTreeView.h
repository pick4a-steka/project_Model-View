#pragma once

#include <QCloseEvent>
#include <QTreeView>
#include <QPainter>
#include <QRandomGenerator>
#include <QPainter>

#include "CelestialBodyModel.h"

class MyTreeView : public QTreeView {
    Q_OBJECT

public:
    MyTreeView(QWidget *parent = nullptr) : QTreeView(parent), model(nullptr) {}

    void setModel(CelestialBodyModel *m) {
        model = m;
        QTreeView::setModel(m);
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        if (model) {
            delete model; // Удаляем модель вручную
            model = nullptr;
        }
        QTreeView::closeEvent(event); // Вызываем базовую реализацию
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(viewport());

        // Рисуем черный фон
        painter.fillRect(rect(), Qt::black);

        // Рисуем звезды
        int numStars = 50; // количество звезд
        for (int i = 0; i < numStars; ++i) {
            int x = QRandomGenerator::global()->bounded(width());
            int y = QRandomGenerator::global()->bounded(height());
            painter.setPen(Qt::white);
            painter.drawPoint(x, y);
        }

        // Продолжить стандартное рисование
        QTreeView::paintEvent(event);
    }

private:
    CelestialBodyModel *model;
};
