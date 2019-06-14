#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"
#include "progressdialog.h"

#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QUrl>
#include <QMessageBox>
#include <QProgressDialog>
#include <QRegularExpression>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void backToMenuSlot();
    void prepareRequest();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void sslErrors(QNetworkReply *, const QList<QSslError> &errors);

private:
    void startRequest(const QUrl &requestedUrl);
    int parseDataToContainers();

private:
    GameWindow *gw;
    Ui::MainWindow *ui;

    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QUrl url;
    QString data;
    const char *defaultUrl = "https://bodyonov.ru/xprojects/words-game/words.py?q=";
    bool httpRequestAborted;
};

#endif // MAINWINDOW_H
