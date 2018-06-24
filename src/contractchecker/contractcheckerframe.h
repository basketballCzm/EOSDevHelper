#ifndef CONTRACTCHECKERFRAME_H
#define CONTRACTCHECKERFRAME_H

#include <QFrame>
#include <QTcpSocket>

namespace Ui {
class ContractCheckerFrame;
}

class ContractCheckerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ContractCheckerFrame(QWidget *parent = 0);
    ~ContractCheckerFrame();

private slots:
    void on_pushButtonCheck_clicked();
    void socketReadData();
    void connected();

    void receiveConnectedError(QTcpSocket::SocketError socketError);

private:
    Ui::ContractCheckerFrame *ui;
    QTcpSocket *clientSocket;
    QString address;
    int port;
};

#endif // CONTRACTCHECKERFRAME_H
