#include "server.h"

#define SERVER_PATH "local-host-server"
#define CLIENT_PATH "local-host-client"
#define MAX_CLIENT 10

Server::Server()
:m_server_socketId(-1)
{
	memset(&m_server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&m_client_sockaddr, 0, sizeof(struct sockaddr_un));
    m_active_client = 0;

	//Create server socket
	m_server_socketId = socket(AF_UNIX, SOCK_STREAM, 0);
	if(m_server_socketId < 0)
	{
		perror("SERVER ERROR: create server socket failed\n");
		exit(1);
	}

	//Bind socket
	memset(&m_server_sockaddr, 0, sizeof(m_server_sockaddr));
	m_server_sockaddr.sun_family = AF_UNIX;
	strcpy(m_server_sockaddr.sun_path, SERVER_PATH);
	unlink(SERVER_PATH);

	if(bind(m_server_socketId, (struct sockaddr *)&m_server_sockaddr, sizeof(m_server_sockaddr))) 
	{
		perror("SERVER ERROR: binding socket failed\n");
		exit(1);
	}
	std::cout << "Server is running at: " << m_server_sockaddr.sun_path << std::endl;

	//start listen
	std::cout <<"Listening for connections...!\n";
	if(listen(m_server_socketId, MAX_CLIENT) == -1)
	{
		perror("SERVER ERROR: listen new connection failed\n");
		exit(1);
	}
	
}

Server::~Server() {}

void Server::HandleConnection(const unsigned int &client_socket)
{
	int bytes_rec = 0;
	char buf_recv[256];
	memset(buf_recv, 0, sizeof(buf_recv)/sizeof(char));

	//Read pid from client request
	bytes_rec = recv(client_socket, buf_recv, sizeof(buf_recv), 0);
	if(bytes_rec == -1)
	{
		perror("SERVER ERROR: receive client data failed\n");
		close(client_socket);
		exit(1);
	}

	const char* msg = buf_recv;

	this->GetMessage(msg, client_socket);
	this->SendMessage(client_socket);
	m_active_client--;
}

void Server::Run()
{
	unsigned int client_socketId = -1;
	for(;;) 
	{
		
		if(m_active_client > MAX_CLIENT)
		{
			std::cout << "Reached maximum connections, please try again later\n";
			continue;	
		}

		socklen_t len = sizeof(m_client_sockaddr);
		client_socketId = accept(m_server_socketId, (struct sockaddr *)&m_client_sockaddr, &len);
		if(client_socketId < 0)
		{
			perror("SERVER ERROR: accept new connection failed.\n");
			exit(1);
		}

		//Handle request		
		std::thread t(&Server::HandleConnection, this, client_socketId);
		t.join();
		m_active_client++;

	}
}

void Server::GetMessage(const char* message, const unsigned int & client_socket)
{
	std::cout << "Received a message from client with process id: " + std::string(message) + "\n";
}

void Server::SendMessage(const unsigned int & client_socket)
{
	char buf_send[256];
	memset(buf_send, 0, sizeof(buf_send)/sizeof(char));
	const char *msg_content = "Message received.\n";
	strcpy(buf_send, msg_content);
	
	std::cout << "Sending ACK message...!" << std::endl;
	if(send(client_socket, buf_send, strlen(buf_send), 0) == -1)
	{
		perror("SERVER ERROR: send data failed\n");
		close(client_socket);
	}
	std::cout << "ACK message sent\n";
}

int main()
{

	Server srv;
	srv.Run();

	return 0;
}
