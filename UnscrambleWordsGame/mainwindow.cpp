#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    reply(nullptr),
    httpRequestAborted(false)
{
    ui->setupUi(this);

    gw = new GameWindow;          // create instance of GameWindow class which contains all nessessary info about game
                                  // don't pass "this" to GameWindow constructor because we wouldn't have seen the app icon in toolbar when swap the window to the game

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(prepareRequest())); // this will prepare request for a new data for game
                                                                              // and then do it and open new window to play on success
    connect(gw, SIGNAL(backToMenuSignal()), this, SLOT(backToMenuSlot()));   // this needed to reopen main window

    connect(&qnam, &QNetworkAccessManager::sslErrors, this, &MainWindow::sslErrors);    // connect sslErrors
}

MainWindow::~MainWindow()
{
    delete gw;
    delete ui;
}

void MainWindow::backToMenuSlot()
{
    gw->unguessedWords.clear(); // clear vectors from words from prev game
    gw->guessedWords.clear();   // clear guessed words vector
    gw->close();                // close game window
    ui->lineEdit->setText("");  // clear input string
    this->show();               // and reopen main window
}

void MainWindow::prepareRequest()
{
    const QString wordForUrl = ui->lineEdit->text().trimmed();                  // returns a string that has whitespace removed from the start and the end.
    if (wordForUrl.size() < 4){
        QMessageBox::information(this, "Извинись!!!",
                                 "Букв в слове должно быть > 3!");
        return;
    }

    const QUrl newUrl = QUrl::fromUserInput(QString(defaultUrl) + wordForUrl);  // create valid URL from user input for query
    if (!newUrl.isValid()) {
        QMessageBox::information(this, tr("\"Такого никогда не случится\""),
                                 tr("Invalid URL: %1: %2").arg(wordForUrl, newUrl.errorString()));
        return;
    }

    data.clear();                                                               // clear string array of data
    ui->playButton->setEnabled(false);                                          // set playButton disabled

    // start the request
    startRequest(newUrl);
}

void MainWindow::startRequest(const QUrl &requestedUrl)
{
    url = requestedUrl;                                                          // set current Url
    reply = qnam.get(QNetworkRequest(url));                                      // send getrequest to server

    connect(reply, &QNetworkReply::finished, this, &MainWindow::httpFinished);   // connect signals finished and readyRead
    connect(reply, &QIODevice::readyRead, this, &MainWindow::httpReadyRead);

    ProgressDialog *progressDialog = new ProgressDialog(this);
    progressDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(progressDialog, &QProgressDialog::canceled, this, &MainWindow::cancelDownload); // connect the signals of the progress dialog
    connect(reply, &QNetworkReply::downloadProgress, progressDialog, &ProgressDialog::networkReplyProgress);
    connect(reply, &QNetworkReply::finished, progressDialog, &ProgressDialog::hide);
    progressDialog->show();
}

void MainWindow::cancelDownload()
{
    reply->abort();
    ui->playButton->setEnabled(true);
    httpRequestAborted = true;
}

void MainWindow::httpFinished()
{
    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    reply->deleteLater();
    reply = nullptr;

    ui->playButton->setEnabled(true);

    if(httpRequestAborted){                         // if flag http request in cancel state just return;
        return;
    }

    switch(parseDataToContainers()) {               // try to parse data
        case -2: QMessageBox::information(this, "Упс",
                                          "Количество слов не совпадает!");
                 gw->unguessedWords.clear();        // clear unguessed words vector
                 break;
        case -1: QMessageBox::information(this, "Упс",
                                          "Квери корруптед!");
                 gw->unguessedWords.clear();
                 break;
        case 0:  QMessageBox::information(this, "Упс",
                                         "Слова не найдены!");
                 break;
        default: this->close();                     // on successfully request close this window
                 gw->changeWordInLabel(ui->lineEdit->text());   // change text in label
                 gw->show();                        // and show gamewindow
    }
}

int MainWindow::parseDataToContainers(){
    /*
     * data received(that's how response looking):
     * <words sample='индус' count='4'>\nиндус,сиу,суд,Сун\n</words>\n
    */

    QRegularExpression re("<words sample=\'[^\']+\' count=\'(?<numberOfWords>\\d+)\'>\\n(?<unparsedWords>[^<]+)</words>\\n"); // set pattern to parse whole XML response
    QRegularExpressionMatch match = re.match(data);                             // get regex matches with data string
    if (match.hasMatch()) {                                                     // if there is match(it should be)
        QString numOfWordsStr = match.captured("numberOfWords");                // get second match in this string(?<numberOfWords>\\d+) -- it equals match.captured(1)
        int numOfWords = numOfWordsStr.toInt(nullptr, 10);                      // try to convert string number of words to int representation
        // unparsed string looks like "индус,сиу,суд,Сун\n"
        QString unparsedWords = match.captured("unparsedWords");                // get third match in this string(?<unparsedWords>[^<]+) -- it equals match.captured(2)
        re.setPattern("([^,]+),|([^\\n]+)\\n");                                 // set another one pattern to parse words from unparsedWordsStr
        QRegularExpressionMatchIterator iterator = re.globalMatch(unparsedWords);// the QRegularExpressionMatchIterator class provides an iterator on the results of a global match of a QRegularExpression object against a string

        int i=0;                                                                // words counter
        while (iterator.hasNext()) {                                            // if iterator have next
            i++;
            QRegularExpressionMatch match = iterator.next();                    // get next match in string
            QString word;
            if(i != numOfWords){                                                // if word is not last, the captured string will have 1 pattern ([^,]+)
                word = match.captured(1);
            } else{                                                             // else the word will have this pattern ([^\\n]+)\\n
                word = match.captured(2);
            }

            gw->unguessedWords.push_back(word);                                 // add word to the unguessed word vector
        }

        if(i!=numOfWords){                                                      // so, if number of words in xml and in unparsed string do not match return -1 mistake code
            return -2;
        }

        return numOfWords;                                                      // else return number of words
    } else {                                                // query corrupted(there is no match at the string)
        return -1;
    }
}

void MainWindow::httpReadyRead()
{
    data += QString(reply->readAll());               // write it into data array
}

void MainWindow::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += '\n';
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("SSL Errors"),
                             tr("One or more SSL errors has occurred:\n%1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}
