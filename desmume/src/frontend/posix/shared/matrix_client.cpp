#include "matrix_client.h"
#include "glib.h"

matrix_client::matrix_client()
{
}
matrix_client::matrix_client(std::string addr)
{
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
		this->conn_valid = false;
    }
	//send handshake packet
	HandshakeHeader hndshk;
	hndshk.success = false;
	hndshk.req_protocol_vers = PROTOCOL_VERSION;
	valsend = send(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
	if(valsend < 0)
	{
		g_printerr("First handshake failed send to server\n");
		this->conn_valid = false;
	}
	//read handshake response from server
	valread = recv(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader),0);
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
			valsend = send(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
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
	frame_header.protocol_vers = 0x00;
	frame_header.intensity = 0.0f;
	int valsend;
	valsend = send(this->client_fd, (u8*)(&frame_header), sizeof(frame_header), 0);
	if(valsend != -1)
		ret_val = true;
	else
		g_printerr("Frame header failed to send to the server\n");
	valsend = send(this->client_fd, (u8*)buffer, height * width * 2, 0);
	if(valsend != -1)
		ret_val = true;
	else
		g_printerr("Frame failed to send to the server\n");
	return ret_val;
}

















/*
<++> matrix_client::<++>()
{

}
*/
