#include "matrix_client.h"

matrix_client::matrix_client()
{
}
matrix_client::matrix_client(std::string addr)
{
	matrix_client();
	//from required address info with  passed string.
	//connect to server
	//send handshake packet
	HandshakeHeader hndshk;
	hndshk.success = false;
	hndshk.req_protocol_vers = PROTOCOL_VERSION;
	int valsend = send(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader), 0);
	//read handshake response from server
	int valread = recv(this->client_fd, (u8*)(&hndshk), sizeof(HandshakeHeader),0);
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
<++> matrix_client::<++>()
{

}
*/
