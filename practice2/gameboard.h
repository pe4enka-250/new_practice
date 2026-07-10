#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QWidget>
#include <QVector>
#include "puzzlepiece.h"

class GameBoard : public QWidget {
    Q_OBJECT
public:
    explicit GameBoard(QWidget *parent = nullptr);

    QVector<PuzzlePiece> pieces;
    int selectedIndex = -1;
    int currentLevel = 0; // 0 - Квадрат, 1 - Прямоугольник, 2 - Треугольник

    void resetGame();

public slots:
    void setLevel(int levelIndex); // Слот для переключения уровня из главного окна

signals:
    void pieceMoved(int id, QPointF newPos);
    void pieceSelected(int id);
    void logAction(const QString &message);
    void gameWon();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QPointF dragOffset;
    bool isDragging = false;
};

#endif // GAMEBOARD_H