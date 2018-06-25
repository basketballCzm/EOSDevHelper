#include "contractcheckerframe.h"
#include "ui_contractcheckerframe.h"

#include <QMessageBox>

ContractCheckerFrame::ContractCheckerFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ContractCheckerFrame)
{
    ui->setupUi(this); 
    this->clientSocket = new QTcpSocket();
    this->address = "192.168.0.101";
    this->port = 19000;
    connect(this->clientSocket,SIGNAL(QAbstractSocket::error(QAbstractSocket::SocketError socketError)),this,SLOT(receiveConnectedError(QTcpSocket::SocketError socketError)));
    connect(this->clientSocket, SIGNAL(connected()), this, SLOT(connected()));
}

ContractCheckerFrame::~ContractCheckerFrame()
{
    this->clientSocket->close();
    delete ui;
}

void ContractCheckerFrame::on_pushButtonCheck_clicked()
{
    ui->textEditOutput->clear();

    QString rawContract = ui->textEditContract->toPlainText();
    if(rawContract.isEmpty()){
        QMessageBox::warning(nullptr, "Error", "Please input your smart contract!");
        return;
    }
    if(QAbstractSocket::UnconnectedState == this->clientSocket->state()){
        this->clientSocket->connectToHost(this->address, this->port);
        if(!this->clientSocket->waitForConnected(1000)){
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

void ContractCheckerFrame::socketReadData()
{
    QByteArray arr = this->clientSocket->readAll();
    if(!arr.isEmpty()){
        ui->textEditOutput->setText(tr(arr));
    }
}

void ContractCheckerFrame::connected()
{
    connect(this->clientSocket, SIGNAL(readyRead()), this, SLOT(socketReadData()));
    QMessageBox::about(nullptr ,"success", "Connection server succeed!");
}

void ContractCheckerFrame::receiveConnectedError(QTcpSocket::SocketError socketError)
{
    QMessageBox::warning(nullptr, "Error","Connection failure!");
    this->clientSocket->close();
}

