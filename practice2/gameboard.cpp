#include "gameboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QRandomGenerator>
#include <QtGlobal>

GameBoard::GameBoard(QWidget *parent) : QWidget(parent) {
    setMinimumSize(400, 400);
    resetGame();
}

void GameBoard::setLevel(int levelIndex) {
    currentLevel = levelIndex;
    resetGame();
    emit logAction(QString("Запущен Уровень %1").arg(currentLevel + 1));
}

void GameBoard::resetGame() {
    pieces.clear();
    selectedIndex = -1;

    if (currentLevel == 0) {
        // УРОВЕНЬ 1: Квадрат (Цель на 150, 150, размер 120x120)
        PuzzlePiece p1;
        p1.id = 1;
        p1.name = "Блок А (Зигзаг)";
        p1.polygon << QPointF(0, 0) << QPointF(120, 0) << QPointF(120, 40)
                   << QPointF(80, 40) << QPointF(80, 80) << QPointF(40, 80)
                   << QPointF(40, 120) << QPointF(0, 120);
        p1.position = QPointF(20, 40);
        p1.color = Qt::cyan;
        p1.lineWidth = 2;
        p1.lineStyle = Qt::SolidLine;

        PuzzlePiece p2;
        p2.id = 2;
        p2.name = "Блок Б (Дополнение)";
        p2.polygon << QPointF(120, 40) << QPointF(120, 120) << QPointF(40, 120)
                   << QPointF(40, 80) << QPointF(80, 80) << QPointF(80, 40);
        p2.position = QPointF(260, 240);
        p2.color = Qt::green;
        p2.lineWidth = 2;
        p2.lineStyle = Qt::SolidLine;

        pieces.append(p1);
        pieces.append(p2);
    }
    else if (currentLevel == 1) {
        // УРОВЕНЬ 2: Прямоугольник (Цель на 100, 150, размер 200x100)
        PuzzlePiece p1;
        p1.id = 1;
        p1.name = "Верхний треугольник";
        p1.polygon << QPointF(0, 0) << QPointF(200, 0) << QPointF(0, 100);
        p1.position = QPointF(20, 40);
        p1.color = QColor(255, 165, 0); // Оранжевый
        p1.lineWidth = 2;
        p1.lineStyle = Qt::SolidLine;

        PuzzlePiece p2;
        p2.id = 2;
        p2.name = "Нижний треугольник";
        p2.polygon << QPointF(200, 0) << QPointF(200, 100) << QPointF(0, 100);
        p2.position = QPointF(180, 260);
        p2.color = QColor(138, 43, 226); // Фиолетовый
        p2.lineWidth = 2;
        p2.lineStyle = Qt::SolidLine;

        pieces.append(p1);
        pieces.append(p2);
    }
    else if (currentLevel == 2) {
        // УРОВЕНЬ 3: Большой Треугольник (Цель: полигон с вершинами (150,150), (270,150), (150,270))
        PuzzlePiece p1;
        p1.id = 1;
        p1.name = "Левая часть";
        p1.polygon << QPointF(0, 0) << QPointF(60, 0) << QPointF(0, 120);
        p1.position = QPointF(30, 40);
        p1.color = QColor(255, 105, 180); // Розовый
        p1.lineWidth = 2;
        p1.lineStyle = Qt::SolidLine;

        PuzzlePiece p2;
        p2.id = 2;
        p2.name = "Правая часть";
        p2.polygon << QPointF(60, 0) << QPointF(120, 0) << QPointF(0, 120);
        p2.position = QPointF(240, 250);
        p2.color = QColor(255, 215, 0); // Золотой
        p2.lineWidth = 2;
        p2.lineStyle = Qt::SolidLine;

        pieces.append(p1);
        pieces.append(p2);
    }

    update();
}

void GameBoard::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(240, 240, 245));

    // Настройка пера для контура цели
    QPen targetPen(Qt::gray, 2, Qt::DashLine);
    painter.setPen(targetPen);
    painter.setBrush(Qt::NoBrush);

    QString levelTaskText = "";

    // Отрисовка шаблона-цели в зависимости от выбранного уровня
    if (currentLevel == 0) {
        painter.drawRect(150, 150, 120, 120);
        levelTaskText = "Уровень 1: Соберите Квадрат в серой зоне";
    } else if (currentLevel == 1) {
        painter.drawRect(100, 150, 200, 100);
        levelTaskText = "Уровень 2: Соберите Прямоугольник в серой зоне";
    } else if (currentLevel == 2) {
        QPolygonF targetTri;
        targetTri << QPointF(150, 150) << QPointF(270, 150) << QPointF(150, 270);
        painter.drawPolygon(targetTri);
        levelTaskText = "Уровень 3: Соберите Большой Треугольник";
    }

    // Отрисовка фигур
    for (int i = 0; i < pieces.size(); ++i) {
        const auto &piece = pieces[i];
        QPen pen(piece.color.darker(), piece.lineWidth, piece.lineStyle);
        QBrush brush(piece.color);

        if (i == selectedIndex) {
            pen.setColor(Qt::red);
        }

        painter.setPen(pen);
        painter.setBrush(brush);

        painter.save();
        painter.translate(piece.position);
        painter.drawPolygon(piece.polygon);
        painter.restore();
    }

    // Текст задачи
    QFont statusFont("Arial", 11, QFont::Bold);
    painter.setFont(statusFont);
    painter.setPen(QColor(80, 80, 80));
    painter.drawText(10, 25, levelTaskText);
}

void GameBoard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        selectedIndex = -1;
        for (int i = pieces.size() - 1; i >= 0; --i) {
            QPointF localPoint = event->position() - pieces[i].position;
            if (pieces[i].polygon.containsPoint(localPoint, Qt::OddEvenFill)) {
                selectedIndex = i;
                isDragging = true;
                dragOffset = event->position() - pieces[i].position;
                emit pieceSelected(pieces[i].id);
                emit logAction(QString("Выбрана фигура: %1").arg(pieces[i].name));
                break;
            }
        }
        update();
    }
    else if (event->button() == Qt::RightButton) {
        for (int i = pieces.size() - 1; i >= 0; --i) {
            QPointF localPoint = event->position() - pieces[i].position;
            if (pieces[i].polygon.containsPoint(localPoint, Qt::OddEvenFill)) {
                selectedIndex = i;
                update();

                QMenu contextMenu(this);
                QAction *changeColorAct = contextMenu.addAction("Случайный цвет детали");
                QAction *resetPosAct = contextMenu.addAction("Сбросить позицию");

                QAction *selectedAct = contextMenu.exec(event->globalPosition().toPoint());
                if (selectedAct == changeColorAct) {
                    pieces[i].color = QColor::fromRgb(QRandomGenerator::global()->generate());
                    emit logAction(QString("Контекстное меню: Изменен цвет %1").arg(pieces[i].name));
                } else if (selectedAct == resetPosAct) {
                    if (currentLevel == 0) {
                        pieces[i].position = (i == 0) ? QPointF(20, 40) : QPointF(260, 240);
                    } else if (currentLevel == 1) {
                        pieces[i].position = (i == 0) ? QPointF(20, 40) : QPointF(180, 260);
                    } else {
                        pieces[i].position = (i == 0) ? QPointF(30, 40) : QPointF(240, 250);
                    }
                    emit logAction(QString("Контекстное меню: Сброс позиции %1").arg(pieces[i].name));
                }
                update();
                break;
            }
        }
    }
}

void GameBoard::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging && selectedIndex != -1) {
        pieces[selectedIndex].position = event->position() - dragOffset;
        emit pieceMoved(pieces[selectedIndex].id, pieces[selectedIndex].position);
        update();
    }
}

void GameBoard::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;

        if (pieces.size() >= 2) {
            // Координаты победы зависят от уровня
            double targetX = (currentLevel == 1) ? 100.0 : 150.0;
            double targetY = 150.0;

            bool p1_correct = (qAbs(pieces[0].position.x() - targetX) < 15 && qAbs(pieces[0].position.y() - targetY) < 15);
            bool p2_correct = (qAbs(pieces[1].position.x() - targetX) < 15 && qAbs(pieces[1].position.y() - targetY) < 15);

            if (p1_correct && p2_correct) {
                pieces[0].position = QPointF(targetX, targetY);
                pieces[1].position = QPointF(targetX, targetY);
                update();

                emit gameWon();
            }
        }
    }
}