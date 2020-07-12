/*this program is create by ly in 2020-07-02*/
/*app:to UDP receive and send in one window by ui*/

#include "qudpapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    if(QT_VERSION>=QT_VERSION_CHECK(5,6,0))
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QUdpApp w;
    w.show();

    return a.exec();
}
