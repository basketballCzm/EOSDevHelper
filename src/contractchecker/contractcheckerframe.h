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
    void sendZipFileData();

private slots:
    void on_pushButtonCheck_clicked();
    void on_pushButtonSendFile_clicked();
    void socketReadData();
    void socketFileReadData();
    void connected();
    void fileConnected();
    void on_pushButtonOpenFile_clicked();

private:
    Ui::ContractCheckerFrame *ui;
    QTcpSocket *clientSocket;
    QTcpSocket * clientFileSocket;
    QString address;
    int port;
    int fport;
};

#endif // CONTRACTCHECKERFRAME_H
