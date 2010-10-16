#include "../headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <QBuffer>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_isBusy(false)
{
    ui->setupUi(this);

    m_netAccessManager = new QNetworkAccessManager(this);
    QObject::connect(m_netAccessManager, SIGNAL(finished(QNetworkReply*))
                     , this, SLOT(replyFinished(QNetworkReply*)));
    m_buffer = new QBuffer(this);

    // Reading settings
    QSettings appSettings;

    ui->txtUrl->setText(appSettings.value("Main/lastUrl").toString());
    if (appSettings.value("Main/requestMethod").toString() == "GET") ui->radioGET->setChecked(true);
    if (appSettings.value("Main/requestMethod").toString() == "POST") ui->radioPOST->setChecked(true);

    QStringList someStringsList;
    someStringsList<<tr("Header")<<tr("Value");
    int someCount;
    someCount = appSettings.value("requestHeaders/Count").toInt();
    ui->tblHeaders->setRowCount(someCount);
    ui->tblHeaders->setHorizontalHeaderLabels(someStringsList);

    int i;

    for (i=0;i<someCount;i++)
    {
        setCellText(ui->tblHeaders,i,0,appSettings.value("requestHeaders/headerName"+QString::number(i)).toString());
        setCellText(ui->tblHeaders,i,1,appSettings.value("requestHeaders/headerValue"+QString::number(i)).toString());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        // Remember last URL
        appSettings.setValue("Main/lastUrl",ui->txtUrl->text());

        ui->tblResponseHeaders->clear();
        ui->tblResponseHeaders->setRowCount(reply->rawHeaderList().count());

        QStringList tableHeaders;
        tableHeaders << tr("Header") << tr("Value");
        ui->tblResponseHeaders->setHorizontalHeaderLabels(tableHeaders);

        //ui->txtHeaders->clear();
        ui->txtResult->setText(Qt::escape(reply->readAll()));
        //ui->txtResult->setText(QString::fromUtf8(reply->readAll()));
        ui->statusBar->showMessage(QString::number(reply->rawHeaderList().count()),5000);
        int i;
        QString header, value;

        for (i = 0; i < reply->rawHeaderList().count(); i++)
        {
            header = "";
            value = "";
            header.append(reply->rawHeaderList().value(i));
            value.append(reply->rawHeader(reply->rawHeaderList().value(i)));

            // Black magic for edit cells
            setCellText(ui->tblResponseHeaders, i,0, header);
            setCellText(ui->tblResponseHeaders, i,1, value);
         };

        //ui->tblResponseHeaders-
    }
    else
        ui->txtResult->setText("Error: " + reply->errorString());
    m_buffer->close();
    m_isBusy = false;
    ui->btnGo->setEnabled(true);
};

void MainWindow::Go()
{
    if (!m_isBusy)
    {
        QNetworkRequest request;
        request.setUrl(QUrl(ui->txtUrl->text()));

        if (ui->tblHeaders->rowCount() > 0) {
            int i;

            QByteArray baHeader(""), baValue("");

            for (i = 0; i<= ui->tblHeaders->rowCount()-1;i++)
            {
                baHeader.clear();
                baValue.clear();

                baHeader.append(ui->tblHeaders->item(i, 0)->text());
                baValue.append(ui->tblHeaders->item(i, 1)->text());
                request.setRawHeader(baHeader, baValue);
            }
        }
        else
            request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; uk; rv:1.9.1.8) Gecko/20100202 Firefox/3.5.8");

        if (ui->radioPOST->isChecked())
        {
            QString someString;
            someString = ui->txtPostData->text();
            QByteArray m_byteArray("");
            m_byteArray.append(someString);

            m_buffer->setData(m_byteArray);
            m_buffer->open(QIODevice::ReadOnly);

            m_netAccessManager->post(request, m_buffer);
        }

        if (ui->radioGET->isChecked())
                m_netAccessManager->get(request);
        m_isBusy = true;
        ui->btnGo->setEnabled(false);
        SaveRequestHeaders();
    }
    else
        QMessageBox::information(this, "Http Shark", tr("Busy. Previous request has not finished yet")
                                 , QMessageBox::Ok, QMessageBox::Ok);
};

void MainWindow::on_btnGo_clicked()
{
    Go();
}

void MainWindow::on_radioGET_toggled(bool checked)
{
    Q_UNUSED(checked)
    ui->lblUrl->setText(tr("Request:"));
    ui->lblPostData->setEnabled(false);
    ui->txtPostData->setEnabled(false);
    appSettings.setValue("Main/requestMethod", "GET");
}

void MainWindow::on_radioPOST_toggled(bool checked)
{
    Q_UNUSED(checked)
    ui->lblUrl->setText("URL:");
    ui->lblPostData->setEnabled(true);
    ui->txtPostData->setEnabled(true);
    appSettings.setValue("Main/requestMethod", "POST");
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,"Http Shark",tr("Program for testing HTTP-responses.")+"\n(C) 2010 sir Hermit",QMessageBox::Ok);
}

void MainWindow::on_btnAddHeader_clicked()
{
    ui->tblHeaders->insertRow(1);
}

void MainWindow::on_btnDelHeader_clicked()
{
    ui->tblHeaders->removeRow(ui->tblHeaders->currentRow());
}

void MainWindow::on_tblHeaders_cellEntered(int row, int column)
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    SaveRequestHeaders();
}

void MainWindow::SaveRequestHeaders()
{
    appSettings.setValue("requestHeaders/count", ui->tblHeaders->rowCount());

    int i;
    for (i = 0; i<ui->tblHeaders->rowCount();i++)
    {
        appSettings.setValue("requestHeaders/headerName"+QString::number(i),ui->tblHeaders->item(i,0)->text());
        appSettings.setValue("requestHeaders/headerValue"+QString::number(i),ui->tblHeaders->item(i,1)->text());
    }
}

void MainWindow::setCellText(QTableWidget *someTable, int row, int column, QString cellText)
{
    QTableWidgetItem *itemCell = someTable -> item(row,column);
    if (!itemCell)
        {
            itemCell = new QTableWidgetItem;
            someTable->setItem(row,column,itemCell);
        }

    itemCell -> setText(cellText);
}

void MainWindow::on_pushButton_clicked()
{
    QString exportFileName ("");
    exportFileName = QFileDialog ::getSaveFileName(this,tr("Export to file"),
                                  ui->txtUrl->text().right(ui->txtUrl->text().length()-6) + ".txt",
                                  tr("Text files")+" (*.txt)");

    if (exportFileName != "")
    {
        QFile exportFile(exportFileName);

        int i;
        QTextStream exportData(&exportFile);
        if (!exportFile.open(QIODevice::WriteOnly | QIODevice::Text))
                 return;

        for (i = 0; i < ui->tblResponseHeaders->rowCount(); i++)
        {
            exportData<<ui->tblResponseHeaders->item(i,0)->text()<<" = "<<ui->tblResponseHeaders->item(i,1)->text()<<"\n";
        }
    }

}
