#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QMainWindow>
#include <QProgressDialog>


class ProgressDialog : public QProgressDialog {
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);

public slots:
   void networkReplyProgress(qint64 bytesRead, qint64 totalBytes);
};

#endif // PROGRESSDIALOG_H
