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

    void bubbleSortDescending(int n);
    QVector<Student> getStudents() const { return students; }
    void setStudents(const QVector<Student>& newStudents) { students = newStudents; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void addStudent();
    void removeSelected();
    void sortStandard();
    void sortBubbleVariant();
    void showContextMenu(const QPoint &pos);

private:
    void updateWidgets();

    QVector<Student> students;

    QLineEdit *sizeInput;
    QLineEdit *surnameInput;
    QSpinBox *courseInput;
    QSpinBox *phoneInput;

    QTableWidget *tableWidget;
    QListWidget *listWidget;
    QComboBox *comboBox;
    QLabel *imageLabel;
};

#endif // MAINWINDOW_H