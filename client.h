#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <chrono>

class Client {
public:
	Client();
	virtual ~ Client();

	void Connect();
	void GetMessage();
	void SendMessage();

private:
	struct sockaddr_un m_server_sockaddr;
	struct sockaddr_un m_client_sockaddr;
	unsigned int m_client_socketId;
};

#endif


