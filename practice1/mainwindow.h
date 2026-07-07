#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTableWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <QKeyEvent>
#include <QMenu>
#include "student.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() {}

    // Метод кастомной сортировки пузырьком по убыванию
    void bubbleSortDescending(int n);

    // Геттер и сеттер для работы механизма юнит-тестирования
    QVector<Student> getStudents() const { return students; }
    void setStudents(const QVector<Student>& newStudents) { students = newStudents; }

protected:
    // П.3: Переопределение обработчика нажатий клавиш клавиатуры
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void addStudent();       // Слот добавления записи
    void removeSelected();   // Слот удаления выбранной записи
    void sortStandard();     // Слот встроенной сортировки списка
    void sortBubbleVariant(); // Слот вызова пузырьковой сортировки для N элементов
    void showContextMenu(const QPoint& pos); // Слот контекстного меню

private:
    void updateWidgets();    // Метод синхронизации данных во всех виджетах

    QVector<Student> students; // Контейнер для хранения динамического массива структур

    // Элементы графического интерфейса
    QLineEdit* sizeInput;
    QLineEdit* surnameInput;
    QSpinBox* courseInput;
    QSpinBox* phoneInput;

    QTableWidget* tableWidget;
    QListWidget* listWidget;
    QComboBox* comboBox;
    QLabel* imageLabel;
};

#endif // MAINWINDOW_H