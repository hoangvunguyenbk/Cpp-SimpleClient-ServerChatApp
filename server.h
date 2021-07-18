#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <mutex>
#include <chrono>
#include <thread>
#include <atomic>

class Server
{
	public:
		Server();
		virtual ~Server();

		void Run();
		void GetMessage(const char* message, const unsigned int & client_socket);
		void SendMessage(const unsigned int & client_socket);
		void HandleConnection(const unsigned int &client_socket);

	private:
		struct sockaddr_un m_server_sockaddr;
		struct sockaddr_un m_client_sockaddr;
		unsigned int m_server_socketId;
		std::atomic<int> m_active_client;
};

#endif
