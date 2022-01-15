//存在的问题：print字符串的时候还是会报错
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QEventLoop>
#include <QElapsedTimer>
#include <QTime>
#include <QTimer>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include "program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace std;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void handleSyntaxTree();
    void showSyntaxTree(Statement *sta, int lineNum);
    void Showhelp();
    void ListCode();
    void handleInput(string s);

    ~MainWindow();

private slots:
    void on_cmdLineEdit_editingFinished();
    void on_btnClearCode_clicked();
    void on_btnRunCode_clicked();
    void on_btnLoadCode_clicked();
public slots:
    void EditClear();
    void ChangeValue();
    void GetInput();


signals:
    void ValueChanged();

private:
    Program program;
    EvalState evalTable;
    QString tmpString;
    string lastIdenti;
    string Control[6] = {"RUN", "LOAD", "LIST", "CLEAR", "HELP", "QIUT"};
    string InstantState[3] = {"LET", "PRINT", "INPUT"};
    Ui::MainWindow *ui;

    // QWidget interface

};
#endif // MAINWINDOW_H
