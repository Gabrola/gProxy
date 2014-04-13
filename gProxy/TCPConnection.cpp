#include "StdAfx.h"
#include "TCPServer.h"
#include "TCPConnection.h"

boost::shared_ptr<TCPConnection> TCPConnection::create(boost::asio::io_service& io_service)
{
	return pointer(new TCPConnection(io_service));
}

tcp::socket& TCPConnection::socket()
{
	return socket_;
}

void TCPConnection::StartReceive(TCPServer* Serverz)
{
	Server = Serverz;
	socket_.async_receive(boost::asio::buffer(data_, 65535),
		boost::bind(&TCPConnection::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::Send(CBuffer Buf)
{
	socket_.async_send(boost::asio::buffer(Buf, Buf.Size()),
		boost::bind(&TCPConnection::HandleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TCPConnection::HandleWrite(const boost::system::error_code& Error, int BytesReceived)
{
	if(Error != 0)
	{
		if(Server->OnDisconnect)
			Server->OnDisconnect(shared_from_this(), Error);
		Server->LastError = Error;
	}
}

void TCPConnection::HandleReceive(const boost::system::error_code& Error, int BytesReceived)
{
	if(Error == 0)
	{
		CBuffer Buffer(BytesReceived);
		memcpy(Buffer, data_, BytesReceived);
		if(Server->OnReceive)
			Server->OnReceive(shared_from_this(), Buffer);
		socket_.async_receive(boost::asio::buffer(data_, 65535),
			boost::bind(&TCPConnection::HandleReceive, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		if(Server->OnDisconnect)
			Server->OnDisconnect(shared_from_this(), Error);
		Server->LastError = Error;
	}
}