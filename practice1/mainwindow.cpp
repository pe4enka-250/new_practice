#include "mainwindow.h"
#include <QHeaderView>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Создание центрального виджета и сетки компоновки
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget); // Использование QGridLayout

    // Инициализация полей ввода данных
    sizeInput = new QLineEdit(this);
    sizeInput->setPlaceholderText("Введите N (размер массива для сортировки)");

    surnameInput = new QLineEdit(this);
    surnameInput->setPlaceholderText("Фамилия студента");

    courseInput = new QSpinBox(this);
    courseInput->setRange(1, 6);
    courseInput->setPrefix("Курс: ");

    phoneInput = new QSpinBox(this);
    phoneInput->setRange(100000, 999999); // Имитация 6-значного номера
    phoneInput->setPrefix("Тел: ");

    // Инициализация управляющих кнопок
    QPushButton *btnAdd = new QPushButton("Добавить в массив", this);
    QPushButton *btnRemove = new QPushButton("Удалить выбранное", this);
    QPushButton *btnSortStd = new QPushButton("Стандартная сортировка (List)", this);
    QPushButton *btnSortBubble = new QPushButton("Сортировка Пузырьком (Вариант 14)", this);

    // Инициализация виджетов вывода информации
    tableWidget = new QTableWidget(0, 3, this); // 3 колонки под поля структуры
    tableWidget->setHorizontalHeaderLabels({"Фамилия", "Курс", "Телефон"});
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu); // Включение контекстного меню
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    listWidget = new QListWidget(this);
    comboBox = new QComboBox(this); // Четный вариант виджета выбора

    // Отображение статической картинки из файла
    imageLabel = new QLabel(this);
    QPixmap pix("image.png");
    if (!pix.isNull()) {
        imageLabel->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        imageLabel->setText("[ Изображение 'image.png' не найдено в папке запуска ]");
    }

    // Размещение виджетов в сетке QGridLayout
    layout->addWidget(sizeInput, 0, 0, 1, 3);
    layout->addWidget(surnameInput, 1, 0);
    layout->addWidget(courseInput, 1, 1);
    layout->addWidget(phoneInput, 1, 2);
    layout->addWidget(btnAdd, 2, 0);
    layout->addWidget(btnRemove, 2, 1);
    layout->addWidget(btnSortBubble, 2, 2);
    layout->addWidget(btnSortStd, 3, 0, 1, 3);
    layout->addWidget(comboBox, 4, 0, 1, 3);
    layout->addWidget(tableWidget, 5, 0, 1, 2);
    layout->addWidget(listWidget, 5, 2, 1, 1);
    layout->addWidget(imageLabel, 6, 0, 1, 3, Qt::AlignCenter);

    setCentralWidget(centralWidget);
    setWindowTitle("Лабораторная работа №1 - Вариант 14");
    resize(850, 650);

    // Назначение сигналов и слотов
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addStudent);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeSelected);
    connect(btnSortStd, &QPushButton::clicked, this, &MainWindow::sortStandard);
    connect(btnSortBubble, &QPushButton::clicked, this, &MainWindow::sortBubbleVariant);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
}

// Добавление нового студента в динамический массив
void MainWindow::addStudent() {
    Student s;
    s.surname = surnameInput->text().trimmed();
    s.course = courseInput->value();
    s.phone = phoneInput->value();

    if(!s.surname.isEmpty()) {
        students.append(s); // Добавление структуры в конец QVector
        surnameInput->clear();
        updateWidgets(); // Обновление представлений
    } else {
        QMessageBox::warning(this, "Ошибка ввода", "Поле 'Фамилия' не может быть пустым!");
    }
}

// Удаление элементов из контейнера и виджетов
void MainWindow::removeSelected() {
    int row = tableWidget->currentRow();
    if (row >= 0 && row < students.size()) {
        students.removeAt(row); // Удаление из вектора структур
        updateWidgets();
    }
}

// Встроенная стандартная сортировка QListWidget (по возрастанию)
void MainWindow::sortStandard() {
    listWidget->sortItems(Qt::AscendingOrder);
}

// Подготовка параметров для пузырьковой сортировки
void MainWindow::sortBubbleVariant() {
    // Считывание размера обрабатываемого массива из текстового поля
    bool ok;
    int n = sizeInput->text().toInt(&ok);

    if (!ok || n <= 0 || n > students.size()) {
        n = students.size(); // Если введено некорректно, берем весь размер массива
    }

    bubbleSortDescending(n); // Вызов алгоритма
    updateWidgets(); // Визуальное обновление таблиц
}

// Алгоритм пузырьковой сортировки по убыванию первого поля
void MainWindow::bubbleSortDescending(int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // Строки сравниваются лексикографически. < означает сортировку по убыванию (Z->A)
            if (students[j].surname < students[j + 1].surname) {
                std::swap(students[j], students[j + 1]);
            }
        }
    }
}

// Синхронный вывод текстовых, числовых данных и иконок во все виджеты
void MainWindow::updateWidgets() {
    tableWidget->setRowCount(0);
    listWidget->clear();
    comboBox->clear();

    QIcon itemIcon("image.png"); // Использование картинки в элементах списков

    for (int i = 0; i < students.size(); ++i) {
        // Заполнение QTableWidget
        tableWidget->insertRow(i);
        QTableWidgetItem *itemSurname = new QTableWidgetItem(itemIcon, students[i].surname);
        QTableWidgetItem *itemCourse = new QTableWidgetItem(QString::number(students[i].course));
        QTableWidgetItem *itemPhone = new QTableWidgetItem(QString::number(students[i].phone));

        tableWidget->setItem(i, 0, itemSurname);
        tableWidget->setItem(i, 1, itemCourse);
        tableWidget->setItem(i, 2, itemPhone);

        // Заполнение QListWidget
        QString listText = students[i].surname + " (Курс: " + QString::number(students[i].course) + ")";
        new QListWidgetItem(itemIcon, listText, listWidget);

        // Заполнение QComboBox
        comboBox->addItem(students[i].surname);
    }
}

// Обработка нажатия правой кнопки мыши (Вызов контекстного меню)
void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = tableWidget->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        QAction *delAction = menu.addAction("Удалить выбранного студента");
        connect(delAction, &QAction::triggered, this, &MainWindow::removeSelected);
        menu.exec(tableWidget->mapToGlobal(pos));
    }
}

// Обработка системного нажатия клавиши на клавиатуре
void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        removeSelected(); // Удаление по кнопке Delete
    } else {
        QMainWindow::keyPressEvent(event); // Передача остальных событий базовому классу
    }
}