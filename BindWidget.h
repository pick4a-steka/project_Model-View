#pragma once

#include <QWidget>
#include "CelestialBodyModel.h"

class BindWidget : public QWidget {
    Q_OBJECT

public:
    BindWidget(QWidget *parent = nullptr) : QWidget(parent), model(nullptr) {}

    void setModel(CelestialBodyModel *m) {
        model = m;
    }

protected:
    void closeEvent(QCloseEvent *event) override {
        if (model) {
            delete model; // Удаляем модель вручную
            model = nullptr;
        }
        QWidget::closeEvent(event); // Вызываем базовую реализацию
    }

private:
    CelestialBodyModel *model;
};
