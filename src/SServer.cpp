#include "sockets.h"

//Constructeur d'un serveur
SServer::SServer(int max, int port) throw(std::string) : Socket()
{
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    bind(sock, (SOCKADDR*)&sin, sizeof(sin));
    if(listen(sock, max) == SOCKET_ERROR)
        throw(std::string("Erreur lors de la creation de la socket (ip invalide ?)"));

}

Socket SServer::_wait() const throw(std::string)
{
    SOCKET csock;
    SOCKADDR_IN csin;
    socklen_t crecsize = sizeof(csin);

    csock = accept(sock, (SOCKADDR*)&csin, &crecsize);

    if(csock == INVALID_SOCKET)
        throw(std::string("Erreur lors de l'arrivee du client"));

    return Socket(csock, csin);
}

void SServer::_shut() const throw(std::string)
{
    throw(std::string("Cette socket n'est pas un client (shutdown)"));
}

