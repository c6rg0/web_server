#ifndef NET_H
#define NET_H

#include <string>

class Network {
public:
    Network( void );
    void listen( void );
    void read( char* buffer );
    void write( std::string payload );
    void close( void );
    ~Network( );

private:
    int sockfd;
    int clientfd;
};

#endif
