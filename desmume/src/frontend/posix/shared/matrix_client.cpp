#include "matrix_client.h"
#include "glib.h"
#include <cerrno>
#include <sys/socket.h>

matrix_client::matrix_client()
{
}
matrix_client::matrix_client(std::string addr)
{
	//create socket
	if((this->client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		g_print("Failed to createsocket");
		this->conn_valid = false;
	}
	//from required address info with  passed string.
	//split string on ':'
	int colon = addr.find(':');
	//create ip and port substrs
	std::string ip_addr = addr.substr(0, colon);
	std::string port = addr.substr(colon + 1, addr.length() - colon);
	//make socket address struct
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons((u16)std::stoi(port));

	if (inet_pton(AF_INET, ip_addr.c_str(), &server_addr.sin_addr) <= 0)
	{
		g_printerr("Converting matrix sink ip addr, to network byte order binary failed\n");
		this->conn_valid = false;
    }
	//connect to server
	int valsend, valread, status;
	if ((status = connect(this->client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
	{
		g_printerr("Connecting to matrix sink server failed\n");
		if(errno == EACCES)
			g_printerr("EACCES");
		else if(errno == EPERM)
			g_printerr("EPERM");
		else if(errno == EADDRINUSE)
			g_printerr("EADDRINUSE");
		else if(errno == EADDRNOTAVAIL)
			g_printerr("EADDRNOTAVAIL");
		else if(errno == EAFNOSUPPORT)
			g_printerr("EAFNOSUPPORT");
		else if(errno == EAGAIN)
			g_printerr("EAGAIN");
		else if(errno == EBADF)
			g_printerr("EBADF");
		else if(errno == ECONNREFUSED)
			g_printerr("ECONNREFUSED");
		else if(errno == EFAULT)
			g_printerr("EFAULT");
		else if(errno == EINPROGRESS)
			g_printerr("EINPROGRESS");
		else if(errno == EINTR)
			g_printerr("EINTR");
		else if(errno == EISCONN)
			g_printerr("EISCONN");
		else if(errno == ENETUNREACH)
			g_printerr("ENETUNREACH");
		else if(errno == ENOTSOCK)
			g_printerr("ENOTSOCK");
		else if(errno == EPROTOTYPE)
			g_printerr("EPROTOTYPE");
		else if(errno == ETIMEDOUT)
			g_printerr("ETIMEDOUT");
		else
			g_printerr(strerror(errno));
		this->conn_valid = false;
    }
	//send handshake packet
	HandshakeHeader hndshk;
	hndshk.success = false;
	hndshk.req_protocol_vers = PROTOCOL_VERSION;
	valsend = this->send_all(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
	if(valsend < 0)
	{
		g_printerr("First handshake failed send to server\n");
		this->conn_valid = false;
	}
	//read handshake response from server
	valread = this->recv_all(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader));
	if(valread < 0)
	{
		g_printerr("Failed to receive handshake packet from server\n");
		this->conn_valid = false;
	}
	// success ? carry on : (server_protocol_version_valid ? send handshake success, carry on : send fail packet, destroy maxtrix client, emit error
	if(hndshk.success == 1)
	{
		//TODO ensure complete
		this->conn_valid = true;
	}
	else
	{
		//TODO write code to check through a list of compatible protocol versions.
		if(hndshk.req_protocol_vers == PROTOCOL_VERSION)
		{
			hndshk.success = 1;
			valsend = this->send_all(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
			if(valsend < 0)
			{
				g_printerr("Second handshake failed send to server\n");
				this->conn_valid = false;
			}
		}
		else
		{
			//Connection incompatible.
			this->conn_valid = false;	//this line should be redundant.
			g_printerr("Failed to negotiate protocol version with server\n");
			close(this->client_fd);
		}
	}
	g_printerr("Conn established\n");
}
matrix_client::~matrix_client()
{
	//TODO destroy object.
}

bool matrix_client::send_frame(u16 *buffer,int height, int width)
{
	if(!this->conn_valid)
		return false;
	bool ret_val = false;
	//TODO handle errors.
	//send header
	SinkPacketHeader frame_header;
	frame_header.bytes_per_pixel = 2;
	frame_header.color_mode = 0x01;
	frame_header.h_res = width - 1;
	frame_header.v_res = height - 1;
	frame_header.h_loc = 0;
	frame_header.v_loc = 0;
	frame_header.priority = 0xFF;
	frame_header.protocol_vers = PROTOCOL_VERSION;
	frame_header.intensity = 0.0f;
	g_printerr("X = %d, Y = %d\n", width, height);
	int valsend;
	valsend = this->send_all(this->client_fd, (u8*)(&frame_header), sizeof(frame_header), 0);
	if(valsend != -1)
		ret_val = true;
	else
		g_printerr("Frame header failed to send to the server\n");
	valsend = this->send_all(this->client_fd, (u8*)buffer, height * width * 2, 0);
	if(valsend != -1)
		ret_val = true;
	else
		g_printerr("Frame failed to, const void buf[.size], size_t size, int flags send to the server\n");
	return ret_val;
}

int matrix_client::send_all(int sock_fd, const void* buf, size_t size, int flags)
{
	long bytes_sent = 0;
	do
	{
		bytes_sent += send(sock_fd, ((u8* )buf) + bytes_sent, size - bytes_sent, flags);
		if(bytes_sent == -1)
			break;
	}while(((unsigned long)bytes_sent) < size);

	return bytes_sent;
}

int matrix_client::recv_all(int sockfd, void *buf, size_t size)
{
	long bytes_recvd = 0;
	do
	{
		bytes_recvd += read(sockfd, ((u8*)buf) + bytes_recvd, size - bytes_recvd);
		if(bytes_recvd == -1)
			break;
	}while(((unsigned long)bytes_recvd) < size);

	return bytes_recvd;
}















/*
<++> matrix_client::<++>()
{

}
*/
