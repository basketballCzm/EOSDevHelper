#include "contractcheckerframe.h"
#include "ui_contractcheckerframe.h"

#include <QFileDialog>
#include <QMessageBox>

ContractCheckerFrame::ContractCheckerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ContractCheckerFrame)
{
    ui->setupUi(this); 
    this->clientSocket = new QTcpSocket();
    this->clientFileSocket = new QTcpSocket();
    //This is ip and port of our server
    this->address = "192.168.118.128";
    this->port = 19000;
    this->fport = 19001;
    connect(this->clientSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(this->clientFileSocket, SIGNAL(connected()), this, SLOT(fileConnected()));
}

ContractCheckerFrame::~ContractCheckerFrame()
{
    this->clientSocket->close();
    this->clientFileSocket->close();
    delete ui;
}

void ContractCheckerFrame::on_pushButtonCheck_clicked()
{
    ui->textEditOutput->clear();
    ui->textEditOutput->setText(tr("waiting..."));
    QString rawContract = ui->textEditContract->toPlainText();
    if(rawContract.isEmpty()){
        QMessageBox::warning(nullptr, "Error", "Please input your smart contract!");
        return;
    }
    if(QAbstractSocket::UnconnectedState == this->clientSocket->state()){
        this->clientSocket->connectToHost(this->address, this->port);
        if(!this->clientSocket->waitForConnected(500)){
            QMessageBox::warning(nullptr, "Error", "Connection failure, Please check if the server is turned on!");
            return;
        }
    }

    qint64 ret = this->clientSocket->write(rawContract.toStdString().c_str(), rawContract.size());
    this->clientSocket->waitForBytesWritten(300);
    if(-1 == ret){
        QMessageBox::warning(nullptr, "Error", "Smart contract send error, Please check if the server is turned on!");
    }
}

void ContractCheckerFrame::on_pushButtonSendFile_clicked()
{
    ui->textEditOutput->clear();
    ui->textEditOutput->setText(tr("waiting..."));
    if(QAbstractSocket::UnconnectedState == this->clientFileSocket->state()){
        this->clientFileSocket->connectToHost(this->address, this->fport);
        if(!this->clientFileSocket->waitForConnected(500)){
            QMessageBox::warning(nullptr, "Error", "Connection failure, Please check if the server is turned on!");
            return;
        }
    }

    //qint64 ret = this->clientSocket->write(rawContract.toStdString().c_str(), rawContract.size());
    //this->clientSocket->waitForBytesWritten(300);

    sendZipFileData();
}

void ContractCheckerFrame::socketReadData()
{
    QByteArray arr = this->clientSocket->readAll();
    if(!arr.isEmpty()){
        ui->textEditOutput->setText(tr(arr));
    }
}

void ContractCheckerFrame::socketFileReadData()
{
    QByteArray arr = this->clientFileSocket->readAll();
    if(!arr.isEmpty()){
        ui->textEditOutput->setText(tr(arr));
    }
}

void ContractCheckerFrame::connected()
{
    connect(this->clientSocket, SIGNAL(readyRead()), this, SLOT(socketReadData()));
    QMessageBox::about(nullptr ,"success", "Connection server succeed!");
}

void ContractCheckerFrame::fileConnected()
{
    connect(this->clientFileSocket, SIGNAL(readyRead()), this, SLOT(socketFileReadData()));
    QMessageBox::about(nullptr ,"success", "Connection server succeed!");
}


void ContractCheckerFrame::on_pushButtonOpenFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("打开文件"),
                tr("C:\\"),
                tr(" All Files (*.*);;Text Files (*.txt);;*.cpp *.h *hpp(*.cpp *.h *.hpp *.cc)"));
    if(!filePath.isEmpty()){
        QFile data(filePath);
        if(data.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&data);
            QApplication::setOverrideCursor(Qt::WaitCursor);
            ui->textEditContract->setText(in.readAll());
            QApplication::restoreOverrideCursor();
        }
    }
}

void ContractCheckerFrame::sendZipFileData()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("打开文件"),
                tr("C:\\"),
                tr(" zip Files (*.zip)"));
    if(!filePath.isEmpty()){
        QFile f(filePath);
        if(f.open(QIODevice::ReadOnly)){
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QByteArray block;
            QByteArray data = f.readAll();
            QDataStream dts(&block, QIODevice::WriteOnly);
            dts.device()->seek(0);
            dts<<data;
            qint64 ret = this->clientFileSocket->write(block);
            this->clientFileSocket->waitForBytesWritten(300);
            if(-1 == ret){
                QMessageBox::warning(nullptr, "Error", "*.zip send error, Please check if the server is turned on!");
            }
            QApplication::restoreOverrideCursor();

        }
    }
}

