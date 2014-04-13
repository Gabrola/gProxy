#pragma once

class TCPClient
{
public:
	boost::system::error_code LastError;

	boost::function<void()> OnConnect;
	boost::function<void(const boost::system::error_code&)> OnDisconnect;
	boost::function<void(CBuffer)> OnReceive;

	TCPClient(boost::asio::io_service& io_service);
	void Connect(char* IPAddress, u_short Port);
	void Send(CBuffer Buf);
	void Close();
	tcp::socket& socket();

private:
	tcp::socket socket_;
	char data_[65535];

	void HandleConnect(const boost::system::error_code& Error);
	void HandleSend(const boost::system::error_code& Error, int BytesReceived);
	void HandeReceive(const boost::system::error_code& Error, int BytesReceived);
};

