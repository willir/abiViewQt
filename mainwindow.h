#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphwidget.h"

#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onLoadAbiFileClicked();
private:
    Ui::MainWindow *ui;
    int pushButtonClickCount;

    GraphWidget *graphWidget;
    QScrollArea *sarea;
    QVBoxLayout *vBoxlayout;
};

#endif // MAINWINDOW_H
