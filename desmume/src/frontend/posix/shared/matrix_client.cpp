#include "matrix_client.h"

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
		//TODO log this failure
		this->conn_valid = false;
    }
	//connect to server
	int valsend, valread, status;
	if ((status = connect(this->client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))) < 0)
	{
		//TODO log this failure
		this->conn_valid = false;
    }
	//send handshake packet
	HandshakeHeader hndshk;
	hndshk.success = false;
	hndshk.req_protocol_vers = PROTOCOL_VERSION;
	valsend = send(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
	//read handshake response from server
	valread = recv(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader),0);
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
			int valsend = send(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
		}
		else
		{
			//Connection incompatible.
			this->conn_valid = false;	//this line should be redundant.
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
	int valsend = send(this->client_fd, (u8*)buffer, height * width * 2, 0);
	if(valsend != -1)
		ret_val = true;
	return ret_val;
}

















/*
 matrix_client::<++>()
{

}
*/
