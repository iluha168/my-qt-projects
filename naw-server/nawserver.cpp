#include "nawserver.h"

NawServer::NawServer(QObject *parent) : QTcpServer{parent}
{
    connect(this, &QTcpServer::newConnection, this, &NawServer::onConnection);
}

void NawServer::onPacket(unsigned short type){
    NawPlayer* player = (NawPlayer*)sender();
    switch(type){
        default:
            player->socket->close();
        break;
    }
}

void NawServer::onConnection(){
    QTcpSocket* newSocket =  nextPendingConnection();
    qDebug()<<"New connection";
    connect(newSocket, &QTcpSocket::readyRead, this, &NawServer::status);
}

void NawServer::status(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    unsigned packetLen = socket->peek(sizeof (unsigned)).toUInt();
    if(packetLen > socket->bytesAvailable()) return;
    socket->read(sizeof (unsigned));
    ushort packetID = socket->read(sizeof(ushort)).toUShort();
    switch(packetID){
        case 0: {
            QString serverName = "Epic Naw server";
            //name as string
            ushort nameLen = serverName.length();
            ushort packetID = 0;
            QByteArray bytes = QByteArray((char*)&packetID, sizeof (ushort)) //packet ID
                    + QByteArray((char*)&nameLen, sizeof (ushort)) //length of string
                    + QByteArray(serverName.toStdString().c_str()); //string data
            unsigned packetLen = bytes.length();
            bytes = bytes.prepend((char*)&packetLen, sizeof (unsigned));
            socket->write(bytes);
        } break;
        case 1: {
            //reading nickname (string)
            ushort nickLen = socket->read(sizeof(unsigned short)).toUShort();
            QString nickname = QString::fromStdString(socket->read(nickLen).toStdString());
            //creating player
            NawPlayer* newPlayer = new NawPlayer(socket, nickname);
            connect(newPlayer, &NawPlayer::Disconnected, this, &NawServer::onDisconnection);
            connect(newPlayer, &NawPlayer::Packet, this, &NawServer::onPacket);
            this->clients.append(newPlayer);

            qDebug()<<"Player logged from "<<socket->peerAddress().toString()<<" with nickname "<<nickname;
            disconnect(socket, &QTcpSocket::readyRead, this, &NawServer::status);
        } break;
        default:
            socket->close();
        break;
    }
}

void NawServer::onDisconnection(){
    NawPlayer* player = (NawPlayer*)sender();
    this->clients.removeAll(player);
    delete player;
}
