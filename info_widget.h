#pragma once

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QDebug>
#include <QRandomGenerator>
#include <QFont>

class InfoWidget : public QFrame {
    Q_OBJECT

public:
    InfoWidget(int w, int h, const QString &n, const QString &t, \
               const QString &i, QColor c, QWidget *parent = nullptr);
    virtual ~InfoWidget();

    void paintEvent(QPaintEvent *event) override;

    void setDataOfCelestial(QColor c, const QString &n, const QString &t, const QString &i);

private:
    QColor color;
    QString info;

    QLabel *labelForName;
    QLabel *labelForType;
    QFont font;
};
