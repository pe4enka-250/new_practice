#include <QtTest>
#include "mainwindow.h"
#include "student.h"

// Реализация класса тестирования на базе QtTest
class StudentTest : public QObject {
    Q_OBJECT

private slots:
    void testBubbleSortDescending(); // Тест-кейс для проверки логики пузырька
};

void StudentTest::testBubbleSortDescending() {
    MainWindow w;

    // Формирование неотсортированного тестового набора данных
    QVector<Student> initialData = {
        {"Andreev", 2, 111111},
        {"Zaharov", 3, 999999},
        {"Ivanov", 1, 123456}
    };

    w.setStudents(initialData);

    // Вызов тестируемого алгоритма для N = 3
    w.bubbleSortDescending(3);
    QVector<Student> sortedData = w.getStudents();

    // Проверка утверждений (Сортировка по убыванию: Zaharov -> Ivanov -> Andreev)
    QCOMPARE(sortedData[0].surname, QString("Zaharov"));
    QCOMPARE(sortedData[1].surname, QString("Ivanov"));
    QCOMPARE(sortedData[2].surname, QString("Andreev"));
}

// Макрос автоматической генерации функции main() для выполнения тестов
QTEST_MAIN(StudentTest)
#include "test_main.moc"