#include "sockets.h"

//Constructeur d'un client
SClient::SClient(std::string host, int port, bool ip) throw(std::string) : Socket()
{
    //Socket() cree la socket.

    //On paramètre notre socket
    if(ip)
        sin.sin_addr.s_addr = inet_addr(host.c_str());
    else
    {
        hostent *ht;
        ht = gethostbyname(host.c_str());
        if(ht == 0)
        {
            throw (std::string("Erreur lors de la resolution du nom d'hote"));
        }
        sin.sin_addr = *((in_addr *)ht->h_addr);
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    //On se connecte
    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
        throw(std::string("Connexion echouee"));
}
