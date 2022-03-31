#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include "nawserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //find network interface
    QNetworkInterface perfectInterface;
    for(const QNetworkInterface &interface : QNetworkInterface::allInterfaces()){
        if(interface.flags() != 0b100111)
            continue;
        perfectInterface = interface;
    }
    if(perfectInterface.name().isEmpty()){
        std::wcerr<<L"Sorry, there is no good IP to listen available for your machine.";
        return -1;
    }
    //ask for address
    std::wcout<<L"Select IP to listen:\n";
    for(unsigned i = 0; i<perfectInterface.addressEntries().count(); i++){
        std::wcout<<L"  "<<i<<L". "
            <<perfectInterface.addressEntries().at(i).ip().toString().toStdWString()
            <<L"\n";
    }
    unsigned choice;
    do{
        std::wcout<<L"Enter the number here: ";
        std::wcin>>choice;
    }while(choice>=perfectInterface.addressEntries().count());
    //
    NawServer server;
    server.listen(perfectInterface.addressEntries().at(choice).ip(), 9900);
    if(server.isListening()){
        std::wcout<<L"\nListening!\n"
            <<perfectInterface.humanReadableName().toStdWString()
            <<L" ("<<perfectInterface.name().toStdWString()<<L")\n"
            <<server.serverAddress().toString().toStdWString()
            <<L" port "<<server.serverPort()<<L"\n";
    }else{
        std::wcerr<<L"Failed to start the server. "<<server.errorString().toStdWString();
        return server.serverError();
    }

    return a.exec();
}
