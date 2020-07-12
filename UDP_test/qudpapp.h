#ifndef QUDPAPP_H
#define QUDPAPP_H

#include <QWidget>
#include <QtNetwork/QUdpSocket>
#include <QPushButton>
#include <QHostInfo>
namespace Ui {
class QUdpApp;
}

class QUdpApp : public QWidget
{
    Q_OBJECT

public:
    explicit QUdpApp(QWidget *parent = 0);
    ~QUdpApp();
     void getHostIpAddress();

private slots:

    void on_sendBt_clicked(); //发送按钮

//===========================================
    void on_recvCb_clicked(bool checked);  //选择接收


    void on_sendMesEdit_cursorPositionChanged();//检测消息框是否有数据

    void read_data();

    //void on_clearRecvListBt_clicked();

    void on_SingleIP_clicked();

    void on_MulIP_clicked();

    void on_BroadcastIP_clicked();

    void on_Join_clicked();

    void on_Exit_clicked();

    void on_clear_all_clicked();

private:
    Ui::QUdpApp *ui;
    QUdpSocket *mSocket;
    QUdpSocket *m_receiveSocker;
    QHostAddress TargetAddrees;
    QHostAddress SourceAddrees;
    QString TargetPort;
    QString SourcePort;
    //使用几个变量确定按钮状态
    bool danbo;
    bool zubo;
    bool guangbo;

};

#endif // QUDPAPP_H
