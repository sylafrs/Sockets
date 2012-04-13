#ifndef SOCKETS_H_INCLUDED
#define SOCKETS_H_INCLUDED

    /*
        Inclure libws2_32.a
    */

    #if defined (WIN32)
        #include <winsock2.h>
        typedef int socklen_t;

    #elif defined (linux)
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <arpa/inet.h>
        #include <unistd.h>

        #define INVALID_SOCKET -1
        #define SOCKET_ERROR -1
        #define closesocket(s) close (s)

        typedef int SOCKET;
        typedef struct sockaddr_in SOCKADDR_IN;
        typedef struct sockaddr SOCKADDR;
    #endif

    #include <string>

    class Socket {
        private:
            static void start();
            static void stop();

        protected:
            SOCKET sock;
            SOCKADDR_IN sin;

        public:
            Socket() throw(std::string);
            Socket(SOCKET s, SOCKADDR_IN sin) throw(std::string);
            ~Socket();

            void _send(std::string s) const; // envoie une chaine
            std::string _recv() const;       // receptionne une chaine
            void _shut() const;              // arrete une connection

            std::string getIp() const;       // renvoie l'ip
            int getPort() const;             // renvoie le port
    };

    // Pour une application client
    class SClient : public Socket {
        public:
            //Constructeur d'un client
            SClient(std::string host = "127.0.0.1", int port = 23, bool ip = true) throw(std::string);
    };

    // Pour une application serveur
    class SServer : public Socket {
        public:
            //Constructeur d'un serveur
            //int max : nombre maximal de connections
            SServer(int max = 5, int port = 23) throw(std::string);

            //Attend un client
            Socket _wait() const throw(std::string);

            //Empeche d'utiliser _shut pour un serveur (affiche un message d'erreur)
            void _shut() const throw(std::string);
    };

    //renvoie le nom de l'ordi
    std::string getMyName();

    //semblables à _send et _recv
    const Socket & operator<<(const Socket & so, const std::string & str) throw(std::string);
    const Socket & operator>>(const Socket & si, std::string & str) throw(std::string);

    //de même mais envoyes dans un flux
    std::ostream & operator<<(std::ostream & o, const Socket & s) throw(std::string);
    std::istream & operator>>(std::istream & i, const Socket & s) throw(std::string);

#endif // SOCKETS_H_INCLUDED
