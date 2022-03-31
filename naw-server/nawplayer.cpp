#include "nawplayer.h"
#include <iostream>

NawPlayer::NawPlayer(QTcpSocket *socket, QString nickname)
{
    this->socket = socket;
    this->nickname = nickname;
    connect(socket, &QTcpSocket::disconnected, this, &NawPlayer::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &NawPlayer::readyRead);
}

void NawPlayer::readyRead(){
    unsigned packetLength = socket->peek(sizeof(unsigned)).toUInt();
    if(packetLength > socket->bytesAvailable()) return;
    socket->read(sizeof(unsigned));
    unsigned packetType = socket->read(sizeof(unsigned short)).toUShort();
    emit this->Packet(packetType);
}

void NawPlayer::disconnected(){
    emit this->Disconnected();
    qDebug()<<"Disconnection";
}
