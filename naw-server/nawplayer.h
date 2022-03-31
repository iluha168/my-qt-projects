#ifndef NAWPLAYER_H
#define NAWPLAYER_H

#include <QTcpSocket>

class NawPlayer : public QObject
{
    Q_OBJECT
private slots:
    void readyRead();
    void disconnected();
signals:
    void Disconnected();
    void Packet(unsigned short);
public:
    QString nickname;
    QTcpSocket* socket;
    explicit NawPlayer(QTcpSocket*, QString);
};

#endif // NAWPLAYER_H
