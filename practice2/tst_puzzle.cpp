#include <QtTest/QtTest>
#include "puzzlepiece.h"
#include "gameboard.h"

class TestPuzzleGame : public QObject {
    Q_OBJECT

private slots:
    void testInitialization() {
        GameBoard board;
        QCOMPARE(board.pieces.size(), 2);
        QCOMPARE(board.selectedIndex, -1);
    }

    void testReset() {
        GameBoard board;
        board.selectedIndex = 0;
        board.resetGame();
        QCOMPARE(board.selectedIndex, -1);
    }

    void testPolygonMath() {
        PuzzlePiece p;
        p.polygon << QPointF(0,0) << QPointF(50,0) << QPointF(50,50) << QPointF(0,50);

        // Точка внутри полигона
        QVERIFY(p.polygon.containsPoint(QPointF(25,25), Qt::OddEvenFill) == true);
        // Точка снаружи полигона
        QVERIFY(p.polygon.containsPoint(QPointF(100,100), Qt::OddEvenFill) == false);
    }
};

QTEST_MAIN(TestPuzzleGame)
#include "tst_puzzle.moc"