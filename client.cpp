#include "client.h"

#define SERVER_PATH "local-host-server"
#define CLIENT_PATH "local-host-client"


Client::Client()
:m_client_socketId(-1)
{
	memset(&m_client_sockaddr, 0, sizeof(m_client_socketId));
	memset(&m_server_sockaddr, 0, sizeof(m_server_sockaddr));

	//Create client socket
	m_client_socketId = socket(AF_UNIX, SOCK_STREAM, 0);
	if(m_client_socketId == -1)
	{
		perror("CLIENT ERROR:  create client socket error\n");
		exit(1);
	}

	//Setup UNIX sockaddrr structure
	m_client_sockaddr.sun_family = AF_UNIX;
	strcpy(m_client_sockaddr.sun_path, CLIENT_PATH);
	unlink(CLIENT_PATH);

	//Binding socket
	if(bind(m_client_socketId, (struct sockaddr*) &m_client_sockaddr, sizeof(m_client_sockaddr)) == -1)
	{
		perror("CLIENT ERROR: binding client socket error\n");
		close(m_client_socketId);
		exit(1);
	}
}

Client::~Client() {}

void Client::Connect()
{
	m_server_sockaddr.sun_family = AF_UNIX;
	strcpy(m_server_sockaddr.sun_path, SERVER_PATH);
	std::cout << "Connecting to server...!\n";

	if(connect(m_client_socketId, (struct sockaddr *) &m_server_sockaddr, sizeof(m_server_sockaddr)) == -1)
	{
		perror("CLIENT ERROR: connect error\n");
		close(m_client_socketId);
		exit(1);
	}
}

void Client::SendMessage()
{
	char buf_send[256];
	pid_t pid = getpid();
	std::string msg(std::to_string(pid));
	const char* msg_send = msg.c_str();
	strcpy(buf_send, msg_send);

	std::cout << "Sending message...!\n";
	if(send(m_client_socketId, buf_send, strlen(buf_send), 0) == -1)
	{
		perror("CLIENT ERROR: send message error\n");
		close(m_client_socketId);
		exit(1);
	}
	std::cout << "Data sent.\n";
}

void Client::GetMessage()
{
	char buf_rec[256];
	memset(buf_rec, 0, sizeof(buf_rec)/sizeof(char));

	if(recv(m_client_socketId, buf_rec, sizeof(buf_rec), 0) == -1)
	{
		perror("CLIENT ERROR: retrieve message error\n");
		close(m_client_socketId);
		exit(1);
	}

	const char* msg_rev = buf_rec;
	std::cout << msg_rev;
	close(m_client_socketId);
}

int main()
{

	Client clt;
	std::cout << "Process id: " << getpid() << std::endl;
	clt.Connect();
	clt.SendMessage();
	clt.GetMessage();

	return 0;
}