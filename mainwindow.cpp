#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QLayout>
#include <typeinfo>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
//    ui->scrollArea->viewport()->adjustSize();

    this->pushButtonClickCount = 0;

    connect(ui->butLoadAbiFile, SIGNAL(clicked()), this, SLOT(onLoadAbiFileClicked()));
    connect(ui->checkHideTrash, SIGNAL(stateChanged(int)),
            ui->graphWidget,    SLOT(hideTrashStateChanged(int)));

    ui->graphWidget->hideTrashStateChanged(ui->checkHideTrash->isChecked());
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoadAbiFileClicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open abi sequince file"),
                                                    "/home/willir",
                                                    tr("Abi (*.ab1 *.abi);; All files(*.*)"));
    if(fileName.isEmpty())
        return;

    ui->graphWidget->readSeqFrom(fileName.toStdString().c_str());
    ui->graphWidget->update();
}
