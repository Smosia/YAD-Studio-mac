#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


//    //Undo
//    ui->actionUndo->setEnabled(false);
//    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), ui->actionUndo,
//            SLOT(setEnabled(bool)));
//    connect(ui->actionUndo, SIGNAL(triggered()), ui->textEdit,
//            SLOT(undo()));

//    //Redo
//    ui->actionRedo->setEnabled(false);
//    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), ui->actionRedo,
//            SLOT(setEnabled(bool)));
//    connect(ui->actionRedo, SIGNAL(triggered()), ui->textEdit,
//            SLOT(redo()));

//    //Select All
//    connect(ui->actionSelect_All, SIGNAL(triggered()), ui->textEdit,
//            SLOT(selectAll()));

//    //Copy
//    ui->actionCopy->setEnabled(false);
//    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCopy,
//            SLOT(setEnabled(bool)));
//    connect(ui->actionCopy, SIGNAL(triggered()), ui->textEdit,
//            SLOT(copy()));

//    //Paste
//    connect(ui->actionPaste, SIGNAL(triggered()), ui->textEdit,
//            SLOT(paste()));

//    //Cut
//    ui->actionCut->setEnabled(false);
//    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionCut,
//            SLOT(setEnabled(bool)));
//    connect(ui->actionCut, SIGNAL(triggered()), ui->textEdit,
//            SLOT(cut()));

//    //Delete
//    ui->actionDelete->setEnabled(false);
//    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), ui->actionDelete,
//            SLOT(setEnabled(bool)));
//    connect(ui->actionDelete, SIGNAL(triggered()), this,
//            SLOT(actionDeleteClicked()));


}

void MainWindow::actionDeleteClicked()
{
//    QTextCursor cursor = ui->textEdit->textCursor();
//    cursor.removeSelectedText();
//    ui->textEdit->setTextCursor(cursor);
}

void MainWindow::onButtonClicked()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
