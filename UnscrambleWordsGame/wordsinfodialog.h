#ifndef WORDSINFODIALOG_H
#define WORDSINFODIALOG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class WordsInfoDialog;
}

class WordsInfoDialog : public QWidget
{
    Q_OBJECT

public:
    explicit WordsInfoDialog(QWidget *parent = nullptr, const QString& = nullptr, const QString& = nullptr);
    ~WordsInfoDialog();

private:
    Ui::WordsInfoDialog *ui;
};

#endif // WORDSINFODIALOG_H
