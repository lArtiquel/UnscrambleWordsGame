#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    connect(ui->backButton, SIGNAL(clicked()), this, SLOT(backToMenuSlot()));
    connect(ui->checkWords, SIGNAL(clicked()), this, SLOT(showWords()));
    connect(ui->enterWordButton, SIGNAL(clicked()), this, SLOT(tryToEnterWord()));
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::backToMenuSlot()
{
    ui->lineEdit->setText("");      // just because gw is not deleting after game, we need to cler lineEdit from prev game
    emit backToMenuSignal();        // and then emit signal to show menu window
}

void GameWindow::showWords()
{
    QString guessed, unguessed;

    for (int i = 0; i<unguessedWords.size(); i++){      // fill guessed string with guessed words
        unguessed += unguessedWords[i];
        if(i!=unguessedWords.size()-1){
            unguessed += ", ";
        }
    }

    for (int i = 0; i<guessedWords.size(); i++){        // fill unguessed string with unguessed words
        guessed += guessedWords[i];
        if(i!=guessedWords.size()-1){
            guessed += ", ";
        }
    }

    WordsInfoDialog *wd = new WordsInfoDialog(nullptr, unguessed, guessed); // creating the QWidget instance of WordsInfoDialog which displays guessed and unguessed words
    wd->setAttribute(Qt::WA_DeleteOnClose);                                 // this needed to clear memory after deletion of dialog
    wd->show();
}

void GameWindow::tryToEnterWord()
{
    QString word = ui->lineEdit->text();                                    // get text from lineEdit

    if(word == ""){
        QMessageBox::information(this, "Извинись!", "Может введешь хотя бы пару букв в поле ввода?м?");
        return;
    }

    for(int i=0; i<unguessedWords.size(); i++){
        if(unguessedWords[i] == word){
            unguessedWords.removeAt(i);
            if(unguessedWords.size() == 0){
                QMessageBox::information(this, "Держу в курсе", "Поздравлямба! Вы угадали ВСЕ слова!!!");   // print what you guessed all words
                ui->lineEdit->setText("");      // just because gw is not deleting after game, we need to cler lineEdit from prev game
                emit backToMenuSignal();                                    // and then emit signal to show menu window again
            } else {
                QMessageBox::information(this, "Держу в курсе", "Поздравлямба! Вы угадали слово!");         // else print what you guessed word
                guessedWords.push_back(word);
            }
            return;
        }
    }

    QMessageBox::information(this, "Держу в курсе", "Такого слова нет или кудахтер его не знает!");         // else print what we didn't guess the word
}

void GameWindow::changeWordInLabel(const QString& word)
{
    ui->label->setStyleSheet("font-size:12pt; font-weight:600; color:#aa22ff;");    // set 12 pt font size weigth Bald and magenta color
    ui->label->setAlignment(Qt::Alignment(Qt::AlignmentFlag::AlignCenter));         // to allign in center
    ui->label->setText("Текущий набор букв(слово):\n\"" + word +"\"");              // set Text
}
