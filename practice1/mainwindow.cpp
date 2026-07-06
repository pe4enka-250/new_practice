#include "mainwindow.h"
#include <QHeaderView>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);

    sizeInput = new QLineEdit(this);
    sizeInput->setPlaceholderText("Введите N (размер массива)");

    surnameInput = new QLineEdit(this);
    surnameInput->setPlaceholderText("Фамилия");

    courseInput = new QSpinBox(this);
    courseInput->setRange(1, 6);
    courseInput->setPrefix("Курс: ");

    phoneInput = new QSpinBox(this);
    phoneInput->setRange(100000, 999999);
    phoneInput->setPrefix("Тел: ");

    QPushButton *btnAdd = new QPushButton("Добавить", this);
    QPushButton *btnRemove = new QPushButton("Удалить выбранное", this);
    QPushButton *btnSortStd = new QPushButton("Стандартная сортировка (List)", this);
    QPushButton *btnSortBubble = new QPushButton("Сортировка Пузырьком (N эл-тов)", this);

    tableWidget = new QTableWidget(0, 3, this);
    tableWidget->setHorizontalHeaderLabels({"Фамилия", "Курс", "Телефон"});
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    listWidget = new QListWidget(this);
    comboBox = new QComboBox(this);

    imageLabel = new QLabel(this);
    QPixmap pix("image.png");
    if (!pix.isNull()) {
        imageLabel->setPixmap(pix.scaled(100, 100, Qt::KeepAspectRatio));
    } else {
        imageLabel->setText("Нет image.png");
    }

    layout->addWidget(sizeInput, 0, 0, 1, 2);
    layout->addWidget(surnameInput, 1, 0);
    layout->addWidget(courseInput, 1, 1);
    layout->addWidget(phoneInput, 1, 2);
    layout->addWidget(btnAdd, 2, 0);
    layout->addWidget(btnRemove, 2, 1);
    layout->addWidget(btnSortStd, 3, 0);
    layout->addWidget(btnSortBubble, 3, 1);
    layout->addWidget(comboBox, 4, 0, 1, 3);
    layout->addWidget(tableWidget, 5, 0, 1, 2);
    layout->addWidget(listWidget, 5, 2, 1, 1);
    layout->addWidget(imageLabel, 6, 0, 1, 3);

    setCentralWidget(centralWidget);
    resize(800, 600);

    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeSelected);
    connect(btnSortStd, &QPushButton::clicked, this, &MainWindow::sortStandard);
    connect(btnSortBubble, &QPushButton::clicked, this, &MainWindow::sortBubbleVariant);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

void MainWindow::addStudent() {
    Student s;
    s.surname = surnameInput->text();
    s.course = courseInput->value();
    s.phone = phoneInput->value();

    if(!s.surname.isEmpty()) {
        students.append(s);
        updateWidgets();
    }
}

void MainWindow::removeSelected() {
    int row = tableWidget->currentRow();
    if (row >= 0 && row < students.size()) {
        students.removeAt(row);
        updateWidgets();
    }
}

void MainWindow::sortStandard() {
    listWidget->sortItems(Qt::AscendingOrder);
}

void MainWindow::sortBubbleVariant() {
    int n = sizeInput->text().toInt();
    if (n <= 0 || n > students.size()) {
        n = students.size();
    }
    bubbleSortDescending(n);
    updateWidgets();
}

void MainWindow::bubbleSortDescending(int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (students[j].surname < students[j + 1].surname) {
                std::swap(students[j], students[j + 1]);
            }
        }
    }
}

void MainWindow::updateWidgets() {
    tableWidget->setRowCount(0);
    listWidget->clear();
    comboBox->clear();

    QIcon icon("image.png");

    for (int i = 0; i < students.size(); ++i) {
        tableWidget->insertRow(i);

        QTableWidgetItem *itemSurname = new QTableWidgetItem(icon, students[i].surname);
        QTableWidgetItem *itemCourse = new QTableWidgetItem(QString::number(students[i].course));
        QTableWidgetItem *itemPhone = new QTableWidgetItem(QString::number(students[i].phone));

        tableWidget->setItem(i, 0, itemSurname);
        tableWidget->setItem(i, 1, itemCourse);
        tableWidget->setItem(i, 2, itemPhone);

        QString listText = students[i].surname + " - Курс: " + QString::number(students[i].course);
        new QListWidgetItem(icon, listText, listWidget);

        comboBox->addItem(students[i].surname);
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu menu(this);
    QAction *delAction = menu.addAction("Удалить запись");
    connect(delAction, &QAction::triggered, this, &MainWindow::removeSelected);
    menu.exec(tableWidget->mapToGlobal(pos));
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        removeSelected();
    }
    QMainWindow::keyPressEvent(event);
}