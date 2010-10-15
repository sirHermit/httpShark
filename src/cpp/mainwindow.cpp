#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWebSettings *ws = QWebSettings::globalSettings();
    ws->setAttribute(QWebSettings::JavascriptEnabled, true);
    ws->setAttribute(QWebSettings::PluginsEnabled, true);
    ws->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_returnPressed()
{
    QString url_str = this->ui->lineEdit->text();
    if (!url_str.startsWith("http://")){
        url_str = "http://"+url_str;
    }
    QUrl url(url_str);
    this->ui->webView->load(url);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"httpShark","Program for testing HTTP-responses.\n(c) sirHermit");
}
