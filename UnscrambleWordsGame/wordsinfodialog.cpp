#include "wordsinfodialog.h"
#include "ui_wordsinfodialog.h"

WordsInfoDialog::WordsInfoDialog(QWidget *parent, const QString& unguessedWords, const QString& guessedWords) :
    QWidget(parent),
    ui(new Ui::WordsInfoDialog)
{
    ui->setupUi(this);
    ui->ugwTextBrowser->setFontWeight(QFont::Bold);
    ui->ugwTextBrowser->setTextColor(Qt::yellow);
    ui->ugwTextBrowser->setTextBackgroundColor(Qt::black);
    ui->ugwTextBrowser->setText(unguessedWords);

    ui->gwTextBrowser->setFontWeight(QFont::Bold);
    ui->gwTextBrowser->setTextColor(Qt::white);
    ui->gwTextBrowser->setTextBackgroundColor(Qt::magenta);
    ui->gwTextBrowser->setText(guessedWords);
}

WordsInfoDialog::~WordsInfoDialog()
{
    delete ui;
}
