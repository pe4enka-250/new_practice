#include <QtTest>
#include "mainwindow.h"
#include "student.h"

class StudentTest : public QObject {
    Q_OBJECT

private slots:
    void testBubbleSortDescending();
};

void StudentTest::testBubbleSortDescending() {
    MainWindow w;
    QVector<Student> initialData = {
        {"Andreev", 2, 111111},
        {"Ivanov", 1, 123456},
        {"Zaharov", 3, 999999}
    };

    w.setStudents(initialData);

    w.bubbleSortDescending(3);
    QVector<Student> sortedData = w.getStudents();

    QCOMPARE(sortedData[0].surname, QString("Zaharov"));
    QCOMPARE(sortedData[1].surname, QString("Ivanov"));
    QCOMPARE(sortedData[2].surname, QString("Andreev"));
}

#ifndef MAIN_APP
QTEST_MAIN(StudentTest)
#include "test_main.moc"
#endif