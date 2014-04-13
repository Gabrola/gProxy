#include "StdAfx.h"
#include "TCPClient.h"

TCPClient::TCPClient(boost::asio::io_service& io_service) : socket_(io_service) {}

void TCPClient::Connect(char* IPAddress, u_short Port)
{
	tcp::endpoint endpoint = tcp::endpoint(boost::asio::ip::address::from_string(IPAddress), Port);
	socket_.async_connect(endpoint, boost::bind(&TCPClient::HandleConnect, this, boost::asio::placeholders::error));
}

void TCPClient::Send(CBuffer Buf)
{
	if(socket_.is_open())
	{
		socket_.async_send(boost::asio::buffer(Buf, Buf.Size()),
			boost::bind(&TCPClient::HandleSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void TCPClient::Close()
{
	if(socket_.is_open())
		socket_.close();
}

tcp::socket& TCPClient::socket()
{
	return socket_;
}

void TCPClient::HandleConnect(const boost::system::error_code& Error)
{
	if(Error == 0)
	{
		if(OnConnect)
			OnConnect();

		socket_.async_receive(boost::asio::buffer(data_, 65535), boost::bind(&TCPClient::HandeReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		LastError = Error;
	}
}

void TCPClient::HandleSend(const boost::system::error_code& Error, int BytesReceived)
{
	if(Error != 0)
	{
		if(OnDisconnect)
			OnDisconnect(Error);
		LastError = Error;
	}
}

void TCPClient::HandeReceive(const boost::system::error_code& Error, int BytesReceived)
{
	if(Error == 0)
	{
		CBuffer Buf(BytesReceived);
		memcpy(Buf, data_, BytesReceived);

		if(OnReceive)
			OnReceive(Buf);

		socket_.async_receive(boost::asio::buffer(data_, 65535), boost::bind(&TCPClient::HandeReceive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		if(OnDisconnect)
			OnDisconnect(Error);

		LastError = Error;
	}
}