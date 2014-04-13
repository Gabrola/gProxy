#pragma once
#include "TCPConnection.h"

class TCPConnection;
class TCPServer
{
public:
	boost::system::error_code LastError;

	boost::function<void(TCPConnection::pointer)> OnConnect;
	boost::function<void(TCPConnection::pointer, const boost::system::error_code&)> OnDisconnect;
	boost::function<void(TCPConnection::pointer, CBuffer)> OnReceive;

	TCPServer(boost::asio::io_service& io_service, int Port) : acceptor_(io_service, tcp::endpoint(boost::asio::ip::address_v4::any(), Port))
	{
		StartAccept();
	}

private:
	tcp::acceptor acceptor_;

	void StartAccept()
	{
		TCPConnection::pointer NewConnection = TCPConnection::create(acceptor_.io_service());
		acceptor_.async_accept(NewConnection->socket(),
			boost::bind(&TCPServer::HandleAccept, this, NewConnection,
			boost::asio::placeholders::error));
	}

	void HandleAccept(TCPConnection::pointer Connection, const boost::system::error_code& Error)
	{
		if(Error == 0)
		{
			if(OnConnect)
				OnConnect(Connection);
			Connection->StartReceive(this);
			StartAccept();
		}
		else
		{
			LastError = Error;
		}
	}
};

