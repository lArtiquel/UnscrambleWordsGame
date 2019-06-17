#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMessageBox>
#include "wordsinfodialog.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    void changeWordInLabel(const QString&);
    QVector<QString> guessedWords;
    QVector<QString> unguessedWords;
    ~GameWindow();

private slots:
    void backToMenuSlot();
    void showWords();
    void tryToEnterWord();


signals:
    void backToMenuSignal();

private:
    Ui::GameWindow *ui;
};

#endif // GAMEWINDOW_H
