#include "info_widget.h"

InfoWidget::InfoWidget(int w, int h, const QString &n, const QString &t, \
                       const QString &i, QColor c, QWidget *parent) : QFrame(parent), color(c), info(i) {
    resize(w, h);
    font = QFont("Courier 10 Pitch", 12);
    labelForName = new QLabel(this);
    labelForName->setStyleSheet(" color: white; ");
    labelForName->setText(n); // установка имени
    labelForName->setGeometry(w / 5, h / 10, 100, 40);
    labelForName->setFont(font);
    labelForName->setFixedSize(150, 40);

    labelForType = new QLabel(this);
    labelForType->setStyleSheet(" color: white; ");
    labelForType->setText(t); // установка типа
    labelForType->setGeometry(w / 5, h / 10 + 30, 100, 40);
    labelForType->setFont(font);
    labelForType->setFixedSize(150, 40);
}

InfoWidget::~InfoWidget() {

}

void InfoWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(Qt::black));
    // Рисуем звезды
    int numStars = 30; // количество звезд
    for (int i = 0; i < numStars; ++i) {
        int x = QRandomGenerator::global()->bounded(width());
        int y = QRandomGenerator::global()->bounded(height());
        painter.setPen(Qt::white);
        painter.drawEllipse(x, y, 3, 3);
    }

    // Устнавливаем цвет для небесного тела
    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);

    // Определим размер четверти круга
    int R = std::min(width(), height()) / 2; // радиус равен половине меньшей стороны виджета
    QRectF circleRect(width() - R, -R, 2 * R, 2 * R); // прямоугольник у окружности

    // Рисуем четверть круга
    painter.drawPie(circleRect, 180 * 16, 90 * 16); // 90 градусов в угловых единицах Qt

    // QFont font("Roboto", 11); // настройка шрифта
    painter.setFont(font);

    // Прямоугольник для текста (с отступами)
    int margin = 20;
    QRectF textRect(
                margin,                     // отступ слева
                height() / 2 + margin / 2,  // отступ сверху
                width() - 2 * margin,       // ширина
                height() - 2 * margin       // высота
    );

    // Устанавливаем параметры текста и сам текст
    painter.setPen(Qt::white); // белый цвет для текста
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, info);
}

void InfoWidget::setDataOfCelestial(QColor c, const QString &n, const QString &t, const QString &i) {
    if (!c.isValid()) {
        qDebug() << "Ошибка при передаче цвета";
        return;
    }

    labelForName->clear();
    labelForName->setText(n);

    labelForType->clear();
    labelForType->setText(t);

    info.clear();
    info = i;

    color = c;

    update();
}
