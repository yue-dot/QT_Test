#include "qudpapp.h"
#include "ui_qudpapp.h"
#include <QMessageBox>
#include <QDebug>
#include <QNetworkInterface>
QUdpApp::QUdpApp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QUdpApp)
{
    ui->setupUi(this);
    mSocket = new QUdpSocket();//创建套接字
    m_receiveSocker = new QUdpSocket();//创建接收套接字
    ui->sendBt->setEnabled(false);
    getHostIpAddress();
    ui->version->setText("version_udp_test6");
    //设置缓冲区
    //mSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,1024*1024*8);
}

QUdpApp::~QUdpApp()
{
    delete mSocket;
    delete ui;
    delete m_receiveSocker;
}




/*
void App::readSendData()
{
    //读取发送数据列表
    App::Datas.clear();
    QString fileName = QString("%1/%2").arg(QUIHelper::appPath()).arg(App::SendFileName);
    QFile file(fileName);
    if (file.size() > 0 && file.open(QFile::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.trimmed();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            if (!line.isEmpty()) {
                App::Datas.append(line);
            }
        }

        file.close();
    }
}

void frmUdpServer::sendData(const QString &ip, int port, const QString &data)
{
    QByteArray buffer;
    if (App::HexSendUdpServer) {
        buffer = QUIHelper::hexStrToByteArray(data);
    } else if (App::AsciiUdpServer) {
        buffer = QUIHelper::asciiStrToByteArray(data);
    } else {
        buffer = data.toLatin1();
    }

    udpSocket->writeDatagram(buffer, QHostAddress(ip), port);

    QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
    append(0, str);

}
*/



//==========================发送端====================


void QUdpApp::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     SourceAddrees.setAddress(strIpAddress);
     qDebug()<<"SourceAddrees_LocalIP:"<<strIpAddress;
}


//单播选择
void QUdpApp::on_SingleIP_clicked()
{
    danbo = 1;
    zubo = 0;
    guangbo = 0;
    //此部分检测是否有数据输入
    if(ui->SourcePort->text().isEmpty() || ui->TargetIPSign->text().isEmpty() || ui->TargetPort->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请分别输入源端口号、单播目的ip、目的端口号");
        //ui->sendSigRb->setChecked(false);
        return;
    }
      //获取本地IP和目标端口
      //SourceAddrees.setAddress( ui->SourceAddrees->text());
      //QString localHostName = QHostInfo->localHostName();
      //mSocket->bind(QHostAddress(localHostName), port);
      //SourceAddrees.setAddress("192.168.1.10");
      TargetAddrees.setAddress(ui->TargetIPSign->text());
      SourcePort = ui->SourcePort->text();
      TargetPort  = ui->TargetPort->text();
      //构造绑定地址
      mSocket->bind(SourceAddrees,SourcePort.toInt());
}

//组播选择
void QUdpApp::on_MulIP_clicked()
{
    danbo = 0;
    zubo = 1;
    guangbo = 0;
    //此部分检测是否有数据输入
    if(ui->SourcePort->text().isEmpty() || ui->TargetIPMul->text().isEmpty() || ui->TargetPort->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请分别输入源端口号、组播目的ip、目的端口号");
        QMessageBox::information(this,tr("注意"),tr("ip地址在224.0.0.0-239.255.255.255\n且不为224.0.0.1"));
        return;
    }
    //获取本地IP和目标端口
    //SourceAddrees.setAddress("192.168.1.10");
    TargetAddrees.setAddress(ui->TargetIPMul->text());
    SourcePort = ui->SourcePort->text();
    TargetPort  = ui->TargetPort->text();
    mSocket->joinMulticastGroup(TargetAddrees);
    //构造绑定地址
    mSocket->bind(SourceAddrees,SourcePort.toInt(),QUdpSocket::ShareAddress);
    //mSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, 10);
}

//广播选择
void QUdpApp::on_BroadcastIP_clicked()
{
    danbo = 0;
    zubo = 0;
    guangbo = 1;
    //此部分检测是否有数据输入
    if(ui->SourcePort->text().isEmpty() || ui->TargetBroadcastIP->text().isEmpty() || ui->TargetPort->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请分别输入源端口号、广播目的ip、目的端口号");
        return;
    }
    //获取本地IP和目标端口
    //QString localHostName = QHostInfo->localHostName();
    //mSocket->bind(QHostAddress(localHostName), port);
    //SourceAddrees.setAddress("192.168.1.10");
    TargetAddrees.setAddress(ui->TargetBroadcastIP->text());
    SourcePort = ui->SourcePort->text();
    TargetPort  = ui->TargetPort->text();
    //mSocket->joinMulticastGroup(TargetAddrees);
    //构造绑定地址
    mSocket->bind(SourceAddrees,SourcePort.toInt());
}


//发送按钮
void QUdpApp::on_sendBt_clicked()
{
    if(guangbo)
        mSocket->writeDatagram(ui->sendMesEdit->toPlainText().toUtf8(),QHostAddress::Broadcast,TargetPort.toInt());
    else
        mSocket->writeDatagram(ui->sendMesEdit->toPlainText().toUtf8(),TargetAddrees,TargetPort.toInt());
    //ui->recvCb->setEnabled(true);
}



//检测发送消息对话框中是否有消息
void QUdpApp::on_sendMesEdit_cursorPositionChanged()
{
    if(ui->sendMesEdit->toPlainText().isEmpty())
    {
        ui->sendBt->setEnabled(false);
    }
    else
    {
        ui->sendBt->setEnabled(true);
    }

}

//==========================发送端END====================


//==========================接收端=====================

//选择接收
void QUdpApp::on_recvCb_clicked(bool checked)
{
    if(ui->TargetPort->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入目标端口号");
        //ui->recvCb->setChecked(true);
        return;
    }
    if(checked)
    {
        //此处任何IPV4的IP都可被接收所以单播和广播都用此函数，端口号要正确即可,共享的方式允许多个客户端监听服务器
        if(guangbo)
            m_receiveSocker->bind(QHostAddress::AnyIPv4,ui->TargetPort->text().toInt(),QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        else
            m_receiveSocker->bind(QHostAddress::AnyIPv4,ui->TargetPort->text().toInt());
        //readyRead为QIODevice定义信号，每当缓存区有新信号则此信号会触发
        connect(m_receiveSocker,SIGNAL(readyRead()),this,SLOT(read_data()));
        //ui->recvCb->setEnabled(false);
    }
    else
    {
        m_receiveSocker->close();
        //ui->recvCb->setEnabled(true);
    }
}

//加入组播按钮
void QUdpApp::on_Join_clicked()
{
    if(ui->JoinMulBr_IP->text().isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入组播ip");
        return;
    }
    if(m_receiveSocker->joinMulticastGroup(QHostAddress(ui->JoinMulBr_IP->text()))) //加入组播
    {
        ui->MulBr_Group->addItem(ui->JoinMulBr_IP->text());
    }
    else
    {
        QMessageBox::warning(this,"提示","加入组播失败,请修改ip后继续加入");
        QMessageBox::information(this,tr("注意"),tr("ip地址在224.0.0.0-239.255.255.255\n且不为224.0.0.1"));
    }
}

void QUdpApp::on_Exit_clicked()
{
    m_receiveSocker->leaveMulticastGroup(QHostAddress(ui->MulBr_Group->currentIndex()));//退出组播地址列表当前的组播
    ui->MulBr_Group->removeItem(ui->MulBr_Group->currentIndex()); //删除组播地址列表中当前的组播地址
}

void QUdpApp::read_data()
{
    QHostAddress recvaddress;
    quint16 port;
    QByteArray datagram;//拥于存放接收的数据报
    do{
        datagram.resize(m_receiveSocker->pendingDatagramSize());//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        m_receiveSocker->readDatagram(datagram.data(),datagram.size(),&recvaddress,&port);//接收数据报，将其存放到datagram中
        ui->recvList->addItem(datagram);
        qDebug()<<"Rece_address:"<<recvaddress.toString()<<"Rece_port"<<port;

    }while(m_receiveSocker->hasPendingDatagrams());//拥有等待的数据报
    qDebug()<<"read_end";
}

void QUdpApp::on_clear_all_clicked()
{
    ui->TargetIPSign->clear();
    ui->TargetIPMul->clear();
    ui->TargetBroadcastIP->clear();
    ui->recvCb->setChecked(false);
    m_receiveSocker->close();
    mSocket->close();
    ui->MulBr_Group->clear();
}

//==========================接收端END=====================






