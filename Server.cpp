#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.h"


Server::Server(int port)throw (const char*) {
    this->fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        throw "socket failure";
    }
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = INADDR_ANY;
    this->server.sin_port = htons(port);
    if(bind(fd,(struct sockaddr*)&server,sizeof (server))<0){
        throw "bind failure";
    }
    if(listen(fd,3) < 0){
        throw "listen failure";
    }
    this->stopAccept = true;
}
void sigHandler(int sigNum){
    cout<<"sidH"<<endl;
}
void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch,this](){
        signal(SIGALRM,sigHandler);
        while(this->stopAccept){
            socklen_t sizeOfClient = sizeof(this->client);
            alarm(1);
            int clientX = accept(fd,(struct sockaddr*)&client,&sizeOfClient);
            if(clientX < 0){
                throw "accept failure";
            }
            ch.handle(clientX);
            close(clientX);
            alarm(0);
        }
    });
}

void Server::stop(){
    this->stopAccept = false;
	t->join(); // do not delete this!
    close(fd);
}

Server::~Server() {}

