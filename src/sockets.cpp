#include "sockets.h"
#include <stdio.h>

int nbOfSocks = 0;

Socket::Socket() throw(std::string)
{
    start();
    sock = socket(AF_INET,SOCK_STREAM,0);

    if (sock == INVALID_SOCKET)
    {
        throw (std::string("Impossible de creer de socket"));
    }
}

Socket::Socket(SOCKET s, SOCKADDR_IN a) throw(std::string)
{
    start();
    sock = s;
    sin = a;

    if (sock == INVALID_SOCKET)
    {
        throw(std::string("Socket invalide ?"));
    }
}

Socket::~Socket()
{
    closesocket(sock);
    nbOfSocks--;
    stop();
}

void Socket::start()
{
    #if defined (WIN32)
        if(nbOfSocks == 0)
        {
            WSADATA WSAData;
            WSAStartup(MAKEWORD(2,2), &WSAData);
        }
    #endif

    nbOfSocks++;
}

void Socket::stop()
{
    #if defined (WIN32)
        WSACleanup();
    #endif
}

void Socket::_send(std::string s) const
{
    if(send(sock, (s+'\0').c_str(), s.length()+1, 0) == SOCKET_ERROR)
        throw(std::string("Erreur lors de l'envoi de ")+s);
}

std::string Socket::_recv() const
{
    char c;
    std::string s = "";

    int ret = recv(sock, &c, 1, 0);

    while(c != '\0' && ret != SOCKET_ERROR && ret != 0)
    {
        s += c;
        ret = recv(sock, &c, 1, 0);
    }

    return s;
}

void Socket::_shut() const
{
    shutdown(sock, 2);
}

std::string Socket::getIp() const
{
    return std::string(inet_ntoa(sin.sin_addr));
}

int Socket::getPort() const
{
    return htons(sin.sin_port);
}

std::string getMyName()
{
    char str[256];
    gethostname(str, sizeof(str));

    return std::string(str);
}

const Socket & operator<<(const Socket & so, const std::string & str) throw(std::string)
{
    so._send(str);
    return so;
}

const Socket & operator>>(const Socket & si, std::string & str) throw(std::string)
{
    str = si._recv();
    return si;
}

std::ostream & operator<<(std::ostream & o, const Socket & s) throw(std::string)
{
    std::string str;
    s >> str;
    o << str;
    return o;
}

std::istream & operator>>(std::istream & i, const Socket & s) throw(std::string)
{
    std::string str;
    i >> str;
    s << str;
    return i;
}
