#pragma once

class TCPServer;
class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:
	typedef boost::shared_ptr<TCPConnection> pointer;
	static pointer create(boost::asio::io_service& io_service);
	tcp::socket& socket();
	void StartReceive(TCPServer* Server);
	void Send(CBuffer Buf);

private:
	TCPConnection(boost::asio::io_service& io_service) : socket_(io_service){};
	void HandleWrite(const boost::system::error_code& Error, int BytesReceived);
	void HandleReceive(const boost::system::error_code& Error, int BytesReceived);

	tcp::socket socket_;
	byte data_[65535];
	TCPServer* Server;
};