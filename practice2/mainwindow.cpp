#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QKeyEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Курсовой проект: Геометрическая мозаика");
    resize(1050, 680);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    m_gameBoard = new GameBoard(this);
    mainLayout->addWidget(m_gameBoard, 2);

    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    mainLayout->addLayout(sidebarLayout, 1);

    m_imageLabel = new QLabel(this);
    QPixmap logoPixmap(50, 50);
    logoPixmap.fill(Qt::transparent);
    QPainter p(&logoPixmap);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(Qt::darkBlue);
    p.drawRect(5, 5, 40, 40);
    p.end();
    m_imageLabel->setPixmap(logoPixmap);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    sidebarLayout->addWidget(m_imageLabel);

    QFormLayout *formLayout = new QFormLayout();
    m_playerNameEdit = new QLineEdit(this);
    m_playerNameEdit->setText("Игрок_1");
    formLayout->addRow("Имя игрока:", m_playerNameEdit);
    sidebarLayout->addLayout(formLayout);

    QFormLayout *styleForm = new QFormLayout();
    m_lineTypeCombo = new QComboBox(this);
    m_lineTypeCombo->addItems({"Сплошной контур", "Штриховой контур", "Пунктирный контур"});

    m_lineThicknessCombo = new QComboBox(this);
    m_lineThicknessCombo->addItems({"2 px", "4 px", "6 px"});

    styleForm->addRow("Тип линии:", m_lineTypeCombo);
    styleForm->addRow("Толщина (пресет):", m_lineThicknessCombo);
    sidebarLayout->addLayout(styleForm);

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    QLabel *sliderLabel = new QLabel("Толщина контура:", this);
    m_thicknessSlider = new QSlider(Qt::Horizontal, this);
    m_thicknessSlider->setRange(1, 10);
    m_thicknessSlider->setValue(2);
    sliderLayout->addWidget(sliderLabel);
    sliderLayout->addWidget(m_thicknessSlider);
    sidebarLayout->addLayout(sliderLayout);

    QHBoxLayout *scrollLayout = new QHBoxLayout();
    QLabel *scrollLabel = new QLabel("Яркость поля:", this);
    m_opacityScrollBar = new QScrollBar(Qt::Horizontal, this);
    m_opacityScrollBar->setRange(0, 100);
    m_opacityScrollBar->setValue(100);
    scrollLayout->addWidget(scrollLabel);
    scrollLayout->addWidget(m_opacityScrollBar);
    sidebarLayout->addLayout(scrollLayout);

    m_gridSnapCheckBox = new QCheckBox("Шаг перемещения по сетке (20px)", this);
    sidebarLayout->addWidget(m_gridSnapCheckBox);

    m_applyStyleButton = new QPushButton("Применить стиль к детали", this);
    sidebarLayout->addWidget(m_applyStyleButton);

    m_resetButton = new QPushButton("Сбросить текущий уровень", this);
    sidebarLayout->addWidget(m_resetButton);

    // НАСТРОЙКА СЕЛЕКТОРА УРОВНЕЙ (Используем QListWidget из ТЗ)
    sidebarLayout->addWidget(new QLabel("Выберите уровень:", this));
    m_inventoryListWidget = new QListWidget(this);
    m_inventoryListWidget->addItems({"Уровень 1: Квадрат", "Уровень 2: Прямоугольник", "Уровень 3: Треугольник"});
    m_inventoryListWidget->setCurrentRow(0); // По умолчанию первый уровень
    sidebarLayout->addWidget(m_inventoryListWidget);

    sidebarLayout->addWidget(new QLabel("Лог событий (QListView):", this));
    m_activityListView = new QListView(this);
    m_listModel = new QStringListModel(this);
    m_activityListView->setModel(m_listModel);
    sidebarLayout->addWidget(m_activityListView);

    sidebarLayout->addWidget(new QLabel("Координаты фигур:", this));
    m_piecesTableWidget = new QTableWidget(this);
    m_piecesTableWidget->setColumnCount(3);
    m_piecesTableWidget->setHorizontalHeaderLabels({"Цвет", "Фигура", "Позиция"});
    m_piecesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    sidebarLayout->addWidget(m_piecesTableWidget);

    // КОННЕКТЫ
    connect(m_resetButton, &QPushButton::clicked, this, &MainWindow::handleResetButton);
    connect(m_applyStyleButton, &QPushButton::clicked, this, &MainWindow::handleApplyStyle);
    connect(m_gameBoard, &GameBoard::pieceMoved, this, &MainWindow::updateTableData);
    connect(m_gameBoard, &GameBoard::pieceSelected, this, &MainWindow::onPieceSelected);
    connect(m_gameBoard, &GameBoard::logAction, this, &MainWindow::appendLog);

    // СВЯЗЫВАЕМ КЛИК ПО СПИСКУ С СИСТЕМОЙ УРОВНЕЙ
    connect(m_inventoryListWidget, &QListWidget::currentRowChanged, m_gameBoard, &GameBoard::setLevel);
    // При смене уровня также нужно обновлять таблицу координат
    connect(m_inventoryListWidget, &QListWidget::currentRowChanged, this, &MainWindow::updateTableData);

    connect(m_gameBoard, &GameBoard::gameWon, this, [this]() {
        QMessageBox::information(this, "Победа!",
                                 QString("Отличная работа, %1! Уровень %2 успешно пройден!").arg(m_playerNameEdit->text()).arg(m_gameBoard->currentLevel + 1));
        appendLog(QString("ПОБЕДА: Пройден уровень %1").arg(m_gameBoard->currentLevel + 1));
    });

    updateTableData();
    appendLog("Игра инициализирована для: " + m_playerNameEdit->text());
}

void MainWindow::handleResetButton() {
    m_gameBoard->resetGame();
    updateTableData();
    appendLog("Уровень перезапущен пользователем " + m_playerNameEdit->text());
}

void MainWindow::handleApplyStyle() {
    if (m_gameBoard->selectedIndex == -1) {
        QMessageBox::warning(this, "Внимание", "Сначала выберите деталь кликом левой кнопкой мыши!");
        return;
    }

    int idx = m_gameBoard->selectedIndex;

    int typeIdx = m_lineTypeCombo->currentIndex();
    if (typeIdx == 0) m_gameBoard->pieces[idx].lineStyle = Qt::SolidLine;
    else if (typeIdx == 1) m_gameBoard->pieces[idx].lineStyle = Qt::DashLine;
    else m_gameBoard->pieces[idx].lineStyle = Qt::DotLine;

    m_gameBoard->pieces[idx].lineWidth = m_thicknessSlider->value();

    m_gameBoard->update();
    updateTableData();
    appendLog(QString("Обновлен контур фигуры: %1").arg(m_gameBoard->pieces[idx].name));
}

void MainWindow::updateTableData() {
    m_piecesTableWidget->setRowCount(m_gameBoard->pieces.size());
    for (int i = 0; i < m_gameBoard->pieces.size(); ++i) {
        const auto &piece = m_gameBoard->pieces[i];

        QPixmap px(16, 16);
        px.fill(piece.color);
        QTableWidgetItem *iconItem = new QTableWidgetItem();
        iconItem->setIcon(QIcon(px));
        m_piecesTableWidget->setItem(i, 0, iconItem);

        m_piecesTableWidget->setItem(i, 1, new QTableWidgetItem(piece.name));

        QString posText = QString("[%1, %2]").arg(qRound(piece.position.x())).arg(qRound(piece.position.y()));
        m_piecesTableWidget->setItem(i, 2, new QTableWidgetItem(posText));
    }
}

void MainWindow::onPieceSelected(int id) {
    Q_UNUSED(id);
    if (m_gameBoard->selectedIndex != -1) {
        m_thicknessSlider->setValue(m_gameBoard->pieces[m_gameBoard->selectedIndex].lineWidth);
    }
}

void MainWindow::appendLog(const QString &message) {
    QStringList list = m_listModel->stringList();
    list.append(message);
    m_listModel->setStringList(list);
    m_activityListView->scrollToBottom();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (m_gameBoard->selectedIndex != -1) {
        int idx = m_gameBoard->selectedIndex;
        int step = m_gridSnapCheckBox->isChecked() ? 20 : 5;

        switch (event->key()) {
        case Qt::Key_W: case Qt::Key_Up:
            m_gameBoard->pieces[idx].position.ry() -= step;
            break;
        case Qt::Key_S: case Qt::Key_Down:
            m_gameBoard->pieces[idx].position.ry() += step;
            break;
        case Qt::Key_A: case Qt::Key_Left:
            m_gameBoard->pieces[idx].position.rx() -= step;
            break;
        case Qt::Key_D: case Qt::Key_Right:
            m_gameBoard->pieces[idx].position.rx() += step;
            break;
        default:
            QMainWindow::keyPressEvent(event);
            return;
        }
        m_gameBoard->update();
        updateTableData();
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}