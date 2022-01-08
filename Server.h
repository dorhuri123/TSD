/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include "CLI.h"
#include <signal.h>
#include "commands.h"
using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file
class socketIO:public DefaultIO{
    int socketId;
public:
    socketIO(int socketId){
        this->socketId = socketId;
    }
    string read() override{
        string serverLine = "";
        char c = 0;
        recv(this->socketId,&c,sizeof(char),0);
        while(c != '\n'){
            serverLine+=c;
            recv(this->socketId,&c,sizeof(char),0);
        }
        return serverLine;
    }
    void read(float *f) override{
        recv(this->socketId,f,sizeof(float),0);
    }
    void write(string text) override{
        const char* cString = text.c_str();
        send(this->socketId,cString,text.size(),0);
    }
    void write(float f) override{
        ostringstream ostringstream1;
        ostringstream1 << f;
        string text(ostringstream1.str());
        write(text);
    }
};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        socketIO dio(clientID);
        CLI cli(&dio);
        cli.start();
    }
};
// implement on Server.cpp
class Server {
	thread* t; // the thread to run the start() method in
    int fd;
    bool stopAccept;
	// you may add data members

public:
    struct sockaddr_in server = {0}, client = {0};
    Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
