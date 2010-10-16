#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QNetworkAccessManager>
#include <QBuffer>
#include <QSettings>
#include <QTableWidget>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void Go();
    void SaveRequestHeaders();
    void setCellText(QTableWidget *someTable, int row, int column, QString cellText);

    Ui::MainWindow *ui;
    QNetworkAccessManager* m_netAccessManager;
    bool m_isBusy;
    QBuffer* m_buffer;
    QSettings appSettings;

 private slots:
        void on_pushButton_clicked();
        void on_tblHeaders_cellEntered(int row, int column);
        void on_btnDelHeader_clicked();
        void on_btnAddHeader_clicked();
        void on_actionAbout_triggered();
        void on_radioPOST_toggled(bool checked);
        void on_radioGET_toggled(bool checked);
        void on_btnGo_clicked();
        void replyFinished(QNetworkReply*);
};

#endif // MAINWINDOW_H
