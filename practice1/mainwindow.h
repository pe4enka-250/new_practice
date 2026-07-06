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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

    // Метод сортировки пузырьком по убыванию (Вариант 14)
    void bubbleSortDescending(int n);
    QVector<Student> getStudents() const { return students; }
    void setStudents(const QVector<Student>& newStudents) { students = newStudents; }

protected:
    void keyPressEvent(QKeyEvent *event) override; // П.3: Обработка нажатий клавиш

private slots:
    void addStudent();
    void removeSelected();
    void sortStandard();
    void sortBubbleVariant();
    void showContextMenu(const QPoint &pos); // П.5: Обработка правой кнопки мыши

private:
    void updateWidgets();

    QVector<Student> students;

    QLineEdit *sizeInput;     // П.1: Размер массива
    QLineEdit *surnameInput;
    QSpinBox *courseInput;
    QSpinBox *phoneInput;

    QTableWidget *tableWidget; // П.7, П.8
    QListWidget *listWidget;   // П.7, П.9
    QComboBox *comboBox;       // П.6: Четный вариант
    QLabel *imageLabel;        // П.4: Отображение картинки
};

#endif // MAINWINDOW_H