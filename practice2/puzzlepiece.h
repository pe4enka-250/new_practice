#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <QPolygonF>
#include <QColor>
#include <QString>

struct PuzzlePiece {
    int id;
    QPolygonF polygon;      // Сложная геометрическая форма детали
    QPointF position;       // Координаты на игровом поле
    QColor color;           // Цвет заливки
    int lineWidth;          // Толщина линий контура
    Qt::PenStyle lineStyle; // Стиль линий контура
    QString name;           // Имя детали для отображения в таблице
    bool isSelected = false;
};

#endif // PUZZLEPIECE_H