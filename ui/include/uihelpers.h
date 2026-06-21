#ifndef UIHELPERS_H
#define UIHELPERS_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QColor>

inline void applyShadow(QWidget* widget)
{
    auto* shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(15);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor(74, 158, 234, 180));
    widget->setGraphicsEffect(shadow);
}

#endif