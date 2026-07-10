#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QListWidget>
#include <QListView>
#include <QSlider>
#include <QScrollBar>
#include <QCheckBox>
#include <QStringListModel>

#include "gameboard.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void handleResetButton();
    void handleApplyStyle();
    void updateTableData();
    void onPieceSelected(int id);
    void appendLog(const QString &message);

private:
    GameBoard *m_gameBoard;

    QLineEdit *m_playerNameEdit;
    QPushButton *m_resetButton;
    QPushButton *m_applyStyleButton;
    QLabel *m_imageLabel;

    QComboBox *m_lineTypeCombo;
    QComboBox *m_lineThicknessCombo;

    QListView *m_activityListView;
    QStringListModel *m_listModel;

    QTableWidget *m_piecesTableWidget;
    QListWidget *m_inventoryListWidget;

    QSlider *m_thicknessSlider;
    QScrollBar *m_opacityScrollBar;

    QCheckBox *m_gridSnapCheckBox;
};

#endif // MAINWINDOW_H