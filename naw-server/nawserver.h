#ifndef NAWSERVER_H
#define NAWSERVER_H

#include <QList>
#include <QTcpServer>
#include <QTcpSocket>
#include "nawplayer.h"

class NawServer : public QTcpServer
{
    Q_OBJECT
    QList<NawPlayer*> clients;
private slots:
    void onConnection();
    void onDisconnection();

    void onPacket(unsigned short);
    void status();
public:
    explicit NawServer(QObject* = nullptr);
};

#endif // NAWSERVER_H
